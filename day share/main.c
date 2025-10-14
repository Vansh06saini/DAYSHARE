#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "friends.h"
#include "post.h"
#include "feed.h"
#include "like.h"

int main() {
    int authChoice;
    char currentUser[50] = "";

    // Initialize data
    initgraph();
    loadFriendsFromFile();
    loadPosts();

    while (1) {
        printf("\n===== AUTH MENU =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &authChoice);
        getchar(); // consume newline

        if (authChoice == 1) {
            Register_User();
        } 
        
        else if (authChoice == 2) {
            // login_User now fills in the username
            if (login_User(currentUser)) {
                printf("Welcome, %s!\n", currentUser);

                int mainChoice;
                do {
                    printf("\n===== MAIN MENU =====\n");
                    printf("1. Add Friend\n");
                    printf("2. Show Friendship\n");
                    printf("3. Create Post\n");
                    printf("4. View Feed\n");
                    printf("5. Logout\n");
                    printf("Enter choice: ");
                    scanf("%d", &mainChoice);
                    getchar();

                    if (mainChoice == 1) {
                        char friendName[50];
                        printf("Enter friend's username to add: ");
                        scanf("%s", friendName);
                        getchar();
                        addfriend(currentUser, friendName);
                    } 
                    
                    else if (mainChoice == 2) {
                        showfriends(currentUser);
                    } 
                    
                    else if (mainChoice == 3) {
                        char content[256];
                        printf("Enter post content: ");
                        fgets(content, sizeof(content), stdin);
                        content[strcspn(content, "\n")] = 0; // remove newline
                        createPost(currentUser, content);
                    } 
                    
                    else if (mainChoice == 4) {
                        FeedStack feed;
                        buildFeed(currentUser, &feed);

                        if (feed.top == NULL) {
                            printf("\n No posts in your feed.\n");
                        } else {
                            FeedNode *temp = feed.top;
                            while (temp != NULL) {
                                printf("\n--------------------------\n");
                                printf(" %s:", temp->post.author);
                                printf(" %s\n", temp->post.content);
                                temp = temp->next;
                            }
                        }
                    } 
                    
                    else if (mainChoice == 5) {
                        printf(" Logging out...\n");
                    } 
                    
                    else {
                        printf(" Invalid choice! Try again.\n");
                    }

                } while (mainChoice != 5);
            }
        } 
        
        else if (authChoice == 3) {
            printf(" Saving data and exiting...\n");
            savefriend();
            exit(0);
        } 
        
        else {
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
