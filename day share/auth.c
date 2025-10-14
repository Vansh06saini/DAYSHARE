#include<stdio.h>
#include<string.h>
#include"auth.h"
#include"friends.h"

int Register_User(){
    FILE *ptr;
    struct user newUser, tempUser;
    printf("Enter Username: ");
    scanf("%s",newUser.username);

    printf("Enter Password: ");
    scanf("%s",newUser.password);
    
    // fucn for duplicate check
    ptr=fopen("users.txt","r");
    if(ptr!=NULL)
    {
        while (fscanf(ptr,"%s",tempUser.username)!=EOF)
        {
            if(strcmp(tempUser.username,newUser.username)==0)
            {
                printf("Sorry! , This Username is not available. Please ch  oose another username. \n");
                fclose(ptr);
                return -1;
            }
        }
        
        fclose(ptr);
    }
    ptr = fopen("users.txt","a+");
    if(ptr==NULL)
    {
        printf("error\n");
        return-1;
    }
    fprintf(ptr,"%s %s\n",newUser.username,newUser.password);
    fclose(ptr);
    check(newUser.username);
    printf("registered successfully! \n");
    return 0;

}

int login_User(char *loggedInUser) {
    char username[50], password[50];
    struct user temp;

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *fp = fopen("users.txt", "r");
    if (!fp) return 0;

    while (fscanf(fp, "%s %s", temp.username, temp.password) != EOF) {
        if (strcmp(temp.username, username) == 0 && strcmp(temp.password, password) == 0) {
            printf(" Login successful! Welcome, %s\n", username);
            strcpy(loggedInUser, username); //  Copy username to main()
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf(" Invalid credentials!\n");
    return 0;
}
