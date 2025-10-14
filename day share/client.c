#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8088
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    char currentUser[50] = "";

    WSAStartup(MAKEWORD(2,2), &wsa);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Connection failed!\n");
        return 1;
    }

    printf("Connected to server.\n");

    while (1) {
        int choice;

        if (strncmp(currentUser, "", 1) == 0) {
            printf("\n===== AUTH MENU =====\n1. Register\n2. Login\n3. Exit\nEnter choice: ");
            scanf("%d", &choice);
            getchar();
            send(sock, (char*)&choice, sizeof(choice), 0);

            char username[50], password[50];
            if (choice == 1) {
                printf("Enter Username: ");
                scanf("%s", username);
                getchar();
                printf("Enter Password: ");
                scanf("%s", password);
                getchar();
                send(sock, username, sizeof(username), 0);
                send(sock, password, sizeof(password), 0);
                recv(sock, buffer, sizeof(buffer), 0);
                printf("%s\n", buffer);
            }
            else if (choice == 2) {
                printf("Enter Username: ");
                scanf("%s", username);
                getchar();
                printf("Enter Password: ");
                scanf("%s", password);
                getchar();
                send(sock, username, sizeof(username), 0);
                send(sock, password, sizeof(password), 0);
                recv(sock, buffer, sizeof(buffer), 0);
                if(strcmp(buffer,"SUCCESS")==0){
                    strcpy(currentUser, username);
                    printf("Login successful. Welcome %s!\n", currentUser);
                } else printf("Login failed!\n");
            }
            else if(choice == 3) break;
        } else {
            printf("\n===== MAIN MENU =====\n1. Add Friend\n2. Show Friendship\n3. Create Post\n4. View Feed\n5. Logout\n6. Exit\nEnter choice: ");
            scanf("%d", &choice);
            getchar();
            send(sock, (char*)&choice, sizeof(choice), 0);

            if(choice == 1){
                char friendName[50];
                printf("Enter friend's username: ");
                scanf("%s", friendName);
                getchar();
                send(sock, friendName, sizeof(friendName),0);
                recv(sock, buffer, sizeof(buffer),0);
                printf("%s\n",buffer);
            }
            else if(choice == 3){
                char content[256];
                printf("Enter post content: ");
                fgets(content,sizeof(content),stdin);
                content[strcspn(content,"\n")] = 0;
                send(sock, content, sizeof(content),0);
                recv(sock, buffer, sizeof(buffer),0);
                printf("%s\n",buffer);
            }
            else{
                memset(buffer,0,sizeof(buffer));
                recv(sock, buffer, sizeof(buffer),0);
                printf("%s\n",buffer);
                if(choice==5) strcpy(currentUser,"");
                else if(choice==6) break;
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
