#include "post.h"
#include <stdlib.h>

#define MAX_POSTS 1000

struct Post posts[MAX_POSTS];
int postCount = 0;


// Load all posts from file when program starts

void loadPosts() {
    FILE *fp = fopen("posts.txt", "r");
    if (!fp) return; // no posts yet

    while (fscanf(fp, "%d %s %[^\n]", 
                  &posts[postCount].id, 
                  posts[postCount].author, 
                  posts[postCount].content) != EOF) {
        postCount++;
    }
    fclose(fp);
}

// Save a single post permanently to posts.txt

void savePost(struct Post post) {
    FILE *fp = fopen("posts.txt", "a");
    if (!fp) {
        printf("Error: Unable to open posts.txt\n");
        return;
    }
    fprintf(fp, "%d %s %s\n", post.id, post.author, post.content);
    fclose(fp);
}

// Generate next Post ID (auto-increment)

int getNextPostID() {
    if (postCount == 0) return 1;
    return posts[postCount - 1].id + 1;
}

// Create new post and save it

void createPost(char *username, char *content) {
    struct Post newPost;
    newPost.id = getNextPostID();
    strcpy(newPost.author, username);
    strcpy(newPost.content, content);

    posts[postCount++] = newPost;
    savePost(newPost);

    printf("✅ Post created successfully! (Post ID: %d)\n", newPost.id);
}
