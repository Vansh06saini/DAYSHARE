#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"friends.h"

graph friendgraph;

void initgraph(){                                                                           //Initializing from 0 to stop it from crashing (by grabage value)
    friendgraph.usercount=0;
}

int check(char *username){   
    char temp[50];
    int found=0;
    FILE *ptr=fopen("users.txt","r");
     while(fscanf(ptr,"%s",temp)!=EOF){
        if(strcmp(temp,username)==0){
            found=1;
            break;
        }
     }            
     fclose(ptr);    
     if(found==0){
        printf("User '%s' is not registered on Day Share.\n", username);
        return 0 ;
     }
        // checking for user exist in system or not 
    for (int i = 0; i < friendgraph.usercount; i++)
    {
        if(strcmp(friendgraph.user[i].username,username)==0)
        return 1;
    }
    strcpy(friendgraph.user[friendgraph.usercount].username,username);                      // copying the name of user (friendgraph.user[friendgraph.usercount]= )
    friendgraph.user[friendgraph.usercount].head=NULL;                                      // set head to null
    friendgraph.usercount++;                                                                // incerment by 1 for new user  
}

void addfriend(char *user1, char *user2){
    check(user1);
    check(user2);

    if(areFriends(user1,user2))
    {
        return;                                                                 // help in to not create duplicate 
    }
    
    friendnode *newnode= malloc(sizeof(friendnode));
    strcpy(newnode->username,user2);
  
    for(int i=0;i<friendgraph.usercount;i++)
    {
        if(strcmp(friendgraph.user[i].username,user1)==0)
        {
            newnode->next=friendgraph.user[i].head;                                           // inserting newfrined to startig of list 
            friendgraph.user[i].head=newnode;
        }
    }
    friendnode *newnode2= malloc(sizeof(friendnode));
    strcpy(newnode2->username,user1);
  
    for(int i=0;i<friendgraph.usercount;i++)
    {
        if(strcmp(friendgraph.user[i].username,user2)==0)                                     // same for user 2 to make bidirectional graph     
        {
            newnode2->next=friendgraph.user[i].head;
            friendgraph.user[i].head=newnode2;
        }
    }
    savefriend();
    printf("%s and %s are now friend!\n",user1,user2);

}
int areFriends(char *user1, char *user2) {
    for (int i = 0; i < friendgraph.usercount; i++) {
        if (strcmp(friendgraph.user[i].username, user1) == 0) {                               // find the username in friendgraph
            friendnode *temp = friendgraph.user[i].head;                                      //  temp to store the first name of the friend of the user 
            while (temp) {
                if (strcmp(temp->username, user2) == 0)                                        // trevering to find the user 2 as friend 
                    return 1;                                                                  // yess friends 
                temp = temp->next;
            }
        }
    }
    return 0;                                                                                  // no friends
}

void savefriend(){
    FILE *fp=fopen("friends.txt","w");
    if(!fp)return;

    for(int i=0;i<friendgraph.usercount;i++)
    {
        friendnode *temp=friendgraph.user[i].head;
        while(temp){
          if (strcmp(friendgraph.user[i].username, temp->username) < 0) {
                fprintf(fp, "%s %s\n", friendgraph.user[i].username, temp->username);
            }
            temp = temp->next;
        }
    }
    fclose(fp);
}
void loadFriendsFromFile() {
    FILE *fp = fopen("friends.txt", "r");
    if (!fp) return;

    char u1[50], u2[50];
    while (fscanf(fp, "%s %s", u1, u2) != EOF) {
        addfriend(u1, u2);   // Make sure it's lowercase 'addfriend'
    }
    fclose(fp);
}

void showfriends(char *username) {
    // Find the user in the graph
    for (int i = 0; i < friendgraph.usercount; i++) {
        if (strcmp(friendgraph.user[i].username, username) == 0) {
            friendnode *temp = friendgraph.user[i].head;
            if (!temp) {
                printf("%s has no friends yet.\n", username);
                return;
            }

            printf("%s's friends:\n", username);
            while (temp) {
                printf("- %s\n", temp->username);
                temp = temp->next;
            }
            return;
        }
    }
    printf("User %s not found.\n", username);
}

