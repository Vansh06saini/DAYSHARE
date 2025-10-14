#include "feed.h"
#include "post.h"
#include "like.h"
#include "friends.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Initialize the feed stack
void initFeed(FeedStack *stack) {
    stack->top = NULL;
}

// Push a post
void pushFeed(FeedStack *stack, struct Post post) {
    FeedNode *newNode = (FeedNode *)malloc(sizeof(FeedNode));
    newNode->post = post;                                       // storing the data from post structure 
    newNode->next = stack->top;
    stack->top = newNode;
}

//  feed stack using user's posts and friends' posts
void buildFeed(char *username, FeedStack *stack) {
    initFeed(stack);

    for (int i = 0; i < postCount; i++) {
        if (strcmp(posts[i].author, username) == 0 || areFriends(username, posts[i].author)) {
            pushFeed(stack, posts[i]);
        }
    }
}

// Display feed from stack (
void showFeed(char *username) {
    FeedStack feed;
    buildFeed(username, &feed);

    if (feed.top == NULL) {
        printf("\n No posts to show in feed.\n");
        return;
    }

    printf("\n===== %s's Feed =====\n", username);
    FeedNode *temp = feed.top;
    while (temp != NULL) {
        printf(" %s: ", temp->post.author);
        printf(" %s\n", temp->post.content);
        printf("--------------------------\n");
        temp = temp->next;
    }
}
