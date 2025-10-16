#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;
    static char passwordBuffer[50];  // Static buffer persists after function returns

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            strcpy(passwordBuffer, userChecker.password);  // Copy to static buffer
            return passwordBuffer;
        }
    }

    fclose(fp);
    return "no user found";
}

// Get user ID by username - needed to properly populate User struct during login
int getUserId(const char *username)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, username) == 0)
        {
            fclose(fp);
            return userChecker.id;
        }
    }

    fclose(fp);
    return -1;  // User not found
}

// Check if username already exists - returns 1 if exists, 0 if available
int usernameExists(const char *username)
{
    FILE *fp;
    struct User userChecker;

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        // File doesn't exist yet, so username is available
        return 0;
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, username) == 0)
        {
            fclose(fp);
            return 1;  // Username already exists
        }
    }

    fclose(fp);
    return 0;  // Username is available
}

// Get next available user ID by finding the highest existing ID
int getNextUserId()
{
    FILE *fp;
    struct User userChecker;
    int maxId = -1;

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        // File doesn't exist yet, start with ID 0
        return 0;
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (userChecker.id > maxId)
        {
            maxId = userChecker.id;
        }
    }

    fclose(fp);
    return maxId + 1;  // Return next available ID
}

// Register a new user
void registerMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;
    char confirmPass[50];
    FILE *fp;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration\n\n");

    // Get username
    printf("\t\t\t\tEnter a username: ");
    scanf("%s", a);

    // Check if username already exists
    if (usernameExists(a))
    {
        printf("\n\n\t\t\t\t✖ Username '%s' already exists!\n", a);
        printf("\t\t\t\tPlease try again with a different username.\n");
        printf("\n\t\t\t\tPress Enter to continue...");
        getchar();  // Clear input buffer
        getchar();  // Wait for Enter
        a[0] = '\0';  // Clear username to signal registration failure
        return;
    }

    // Get password (with echo disabled)
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    // Password input loop - continue until passwords match
    do {
        printf("\n\t\t\t\tEnter a password: ");
        scanf("%s", pass);

        printf("\n\t\t\t\tConfirm password: ");
        scanf("%s", confirmPass);

        if (strcmp(pass, confirmPass) != 0)
        {
            printf("\n\n\t\t\t\t✖ Passwords do not match! Please try again.\n");
        }
    } while (strcmp(pass, confirmPass) != 0);

    // Restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    // Save new user to file
    fp = fopen("./data/users.txt", "a");
    if (fp == NULL)
    {
        printf("\n\n\t\t\t\t✖ Error! Could not open users file\n");
        exit(1);
    }

    int newUserId = getNextUserId();
    fprintf(fp, "%d %s %s\n", newUserId, a, pass);
    fclose(fp);

    printf("\n\n\t\t\t\t✔ Registration successful!\n");
    printf("\t\t\t\tYour user ID is: %d\n", newUserId);
    printf("\t\t\t\tYou can now login with your credentials.\n");
    printf("\n\t\t\t\tPress Enter to continue...");
    getchar();  // Clear input buffer
    getchar();  // Wait for Enter
}
