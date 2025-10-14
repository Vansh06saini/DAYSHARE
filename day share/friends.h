#ifndef FRIENDS_H
#define FRIENDS_H

#define MAXUSER 100

typedef struct friendnode 
{
    char username[50];
    struct friendnode *next;
}friendnode;


typedef struct {
    char username[50];
    friendnode *head;
}userlist;

typedef struct {
    userlist user[MAXUSER];
    int usercount;
}graph;

extern graph friendgraph; //global variable for whole frinendship network

void initgraph();
void loadFriendsFromFile();
void savefriend();
int check(char *username);
void addfriend(char *user1, char *user2);
void showfriends(char *username);
int areFriends(char *user1,char *user2);

#endif




