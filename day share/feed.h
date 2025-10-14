#ifndef FEED_H
#define FEED_H

#include "post.h"
#include "friends.h"

// Stack Node structure for feed
typedef struct FeedNode {
    struct Post post;
    struct FeedNode *next;
} FeedNode;

// Feed Stack structure
typedef struct {
    FeedNode *top;
} FeedStack;           

// Functions
void initFeed(FeedStack *stack);
void pushFeed(FeedStack *stack, struct Post post);
void showFeed(char *username);
void buildFeed(char *username, FeedStack *stack);

#endif
