// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#include "auth.h"
#include "friends.h"
#include "post.h"
#include "feed.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 8088
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

CRITICAL_SECTION cs; // For thread-safe file/global access

typedef struct {
    SOCKET sock;
    char username[50];
} ClientInfo;

// Function prototypes
DWORD WINAPI ClientHandler(LPVOID param);

int main() {
    WSADATA wsa;
    SOCKET listenSock, clientSock;
    struct sockaddr_in server, client;
    int clientLen = sizeof(client);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error Code : %d\n", WSAGetLastError());
        return 1;
    }

    // Initialize data
    InitializeCriticalSection(&cs);
    initgraph();
    loadFriendsFromFile();
    loadPosts();

    // Create socket
    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock == INVALID_SOCKET) {
        printf("Could not create socket : %d\n", WSAGetLastError());
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_PORT);

    if (bind(listenSock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d\n", WSAGetLastError());
        return 1;
    }

    listen(listenSock, MAX_CLIENTS);
    printf("Server listening on port %d...\n", SERVER_PORT);

    while ((clientSock = accept(listenSock, (struct sockaddr *)&client, &clientLen))) {
        printf("Client connected.\n");

        ClientInfo *cinfo = (ClientInfo *)malloc(sizeof(ClientInfo));
        cinfo->sock = clientSock;
        strcpy(cinfo->username, "");

        CreateThread(NULL, 0, ClientHandler, (LPVOID)cinfo, 0, NULL);
    }

    closesocket(listenSock);
    DeleteCriticalSection(&cs);
    WSACleanup();
    return 0;
}

// ========================= Client Handler =========================
DWORD WINAPI ClientHandler(LPVOID param) {
    ClientInfo *cinfo = (ClientInfo *)param;
    SOCKET sock = cinfo->sock;
    char buffer[BUFFER_SIZE];
    int choice;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int recvSize = recv(sock, (char*)&choice, sizeof(choice), 0);
        if (recvSize <= 0) break; // client disconnected

        EnterCriticalSection(&cs); // protect file/global access

        if (strcmp(cinfo->username, "") == 0) {
            // Auth Menu
            if (choice == 1) { // Register
                char username[50], password[50];
                recv(sock, username, sizeof(username), 0);
                recv(sock, password, sizeof(password), 0);

                FILE *fp = fopen("users.txt", "r");
                int duplicate = 0;
                if (fp) {
                    struct user temp;
                    while (fscanf(fp, "%s %s", temp.username, temp.password) != EOF) {
                        if (strcmp(temp.username, username) == 0) {
                            duplicate = 1;
                            break;
                        }
                    }
                    fclose(fp);
                }

                if (duplicate) {
                    send(sock, "Username already exists!", 25, 0);
                } else {
                    fp = fopen("users.txt", "a");
                    fprintf(fp, "%s %s\n", username, password);
                    fclose(fp);
                    check(username); // initialize friendgraph
                    send(sock, "Registered successfully!!!!", 25, 0);
                }
            } 
            else if (choice == 2) { // Login
                char username[50], password[50];
                recv(sock, username, sizeof(username), 0);
                recv(sock, password, sizeof(password), 0);

                FILE *fp = fopen("users.txt", "r");
                int success = 0;
                if (fp) {
                    struct user temp;
                    while (fscanf(fp, "%s %s", temp.username, temp.password) != EOF) {
                        if (strcmp(temp.username, username) == 0 && strcmp(temp.password, password) == 0) {
                            success = 1;
                            break;
                        }
                    }
                    fclose(fp);
                }
                if (success) {
                    strcpy(cinfo->username, username);
                    send(sock, "SUCCESS", 8, 0);
                } else send(sock, "FAILURE", 8, 0);
            }
            else if (choice == 3) { // Exit
                LeaveCriticalSection(&cs);
                break;
            }
        } else {
            // Main Menu
            if (choice == 1) { // Add Friend
                char friendName[50];
                recv(sock, friendName, sizeof(friendName), 0);
                addfriend(cinfo->username, friendName);
                send(sock, "Friend added successfully!", 27, 0);
            } 
            else if (choice == 2) { // Show friends
                char friendList[BUFFER_SIZE] = "";
                for (int i=0;i<friendgraph.usercount;i++){
                    if(strcmp(friendgraph.user[i].username, cinfo->username)==0){
                        friendnode *temp=friendgraph.user[i].head;
                        while(temp){
                            strcat(friendList,temp->username);
                            strcat(friendList,"\n");
                            temp=temp->next;
                        }
                        break;
                    }
                }
                if(strlen(friendList)==0) strcpy(friendList,"No friends yet.");
                send(sock, friendList, strlen(friendList)+1,0);
            } 
            else if (choice == 3) { // Create Post
                char content[256];
                recv(sock, content, sizeof(content), 0);
                createPost(cinfo->username, content);
                send(sock, "Post created successfully!", 27, 0);
            } 
            else if (choice == 4) { // View Feed
                FeedStack feed;
                buildFeed(cinfo->username, &feed);
                char feedStr[BUFFER_SIZE*5] = "";
                FeedNode *temp = feed.top;
                if(!temp) strcat(feedStr, "No posts in feed.\n");
                while(temp){
                    char line[512];
                    snprintf(line,sizeof(line),"%s: %s\n", temp->post.author,temp->post.content);
                    strcat(feedStr,line);
                    temp=temp->next;
                }
                send(sock, feedStr, strlen(feedStr)+1, 0);
            }
            else if (choice == 5) { // Logout
                strcpy(cinfo->username, "");
                send(sock, "Logged out successfully.", 25, 0);
            }
            else if (choice == 6) { // Exit
                LeaveCriticalSection(&cs);
                break;
            }
            else {
                send(sock, "Invalid choice!", 16, 0);
            }
        }

        LeaveCriticalSection(&cs);
    }

    printf("Client disconnected.\n");
    closesocket(sock);
    free(cinfo);
    return 0;
}
