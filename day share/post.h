#ifndef POST_H
#define POST_H

#define MAX_POSTS 1000
#include <stdio.h>
#include <string.h>

// Structure for each Post
struct Post {
    int id;                   // Unique post ID
    char author[50];          // Username of who created post
    char content[256];        // Post text content
};
// global post array access
extern struct Post posts[MAX_POSTS];
extern int postCount;


// Function Declarations
void createPost(char *username, char *content);
void loadPosts();  // loads posts from file into memory
void savePost(struct Post post);
int getNextPostID();


#endif
