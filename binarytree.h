#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct tree{
    char name[15];
    int place[5];
    int ID;
    struct tree *left;
    struct tree *right;
} tree;

tree *root = NULL;
tree *leaf;
int currentID_B = 0;


tree* createUserBinary(char *name) {
    leaf = (tree*) malloc(sizeof(tree));
    strcpy(leaf->name, name);
    currentID_B++;
    leaf->ID = currentID_B;
    leaf->left = leaf->right = NULL;
    // Initialising all places to 0 for later site logging
    for(int i = 0 ; i < 5 ; i++)
    {
        leaf->place[i] = 0;
    }
    return leaf;
}


bool findUserBinary(tree *root, char *name) {
    if (root == NULL) 
    {
        return false;
    }
    if (strcmp(name, root->name) == 0)
    {
        return true;
    } 
    if (strcmp(name,root->name) < 0)
    {
        return findUserBinary(root->left, name);
    }
    else if(strcmp(name, root->name) > 0)
    {
        return findUserBinary(root->right, name);
    }
    return false;
}


int insertUserBinary(tree **rootptr, char *name) {
    tree *root = *rootptr;
    if (root == NULL)
    {
        *rootptr = createUserBinary(name);
        printf("New user '%s' has been saved. This user's ID is %d.\n\n", name, currentID_B);
        return 1;
    }
    if (strcmp(name, root->name) < 0)
    {
        return insertUserBinary(&(root->left), name);
    }
    if (strcmp(name, root->name) > 0)
    {
        return insertUserBinary(&(root->right), name);
    }
    return 0;
}


int logSiteBinary(tree *root, char *name, int site) {
    int i = 0;
    if(root == NULL)
    {
        return 0;
    }
    if(strcmp(name, root->name) == 0)
    {
        while(root->place[i] != 0)
        {
            i++;
        }
        root->place[i] = site;
        printf("%s has been logged at site %d.\n\n", name, site);
    }
    if(strcmp(name, root->name) < 0)
    {
        logSiteBinary(root->left, name, site);
    }
    else 
    {
        logSiteBinary(root->right, name, site);
    }
    return 1;
}


tree* getRMin(tree *root) {
    tree *temp = root;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}


tree* removeUserBinary(tree *root, char *name) { 
    if(root == NULL)
    {
        return root;
    }
    else 
    {
        if(strcmp(name, root->name) < 0)
        {
            root->left = removeUserBinary(root->left, name);
        }
        else if(strcmp(name, root->name) > 0)
        {
            root->right = removeUserBinary(root->right, name);
        }
        else if(strcmp(name,root->name) == 0)
        {
            //case 1 - the node is a leaf (0 children)
            if((root->left == NULL) && (root->right == NULL))
            {
                root = NULL;
                printf("This user, '%s', has been removed.\n\n", name);
                return root;
            }

            // case 2 - the node has 1 child on right subtree
            else if((root->left == NULL) && (root->right != NULL))
            {
                root = root->right;
                printf("This user, '%s', has been removed.\n\n", name);
                return root;
            }
            else if((root->left != NULL) && (root->right == NULL))
            {
                root = root->left;
                printf("This user, '%s', has been removed.\n\n", name);
                return root;    
            }

            // case 3 - the node has 2 children
            else if((root->left != NULL) && (root->right != NULL))
            {
                tree *temp = getRMin(root->right);
                strcpy(root->name, temp->name);
                root->right = removeUserBinary(root->right, root->name);
            }
        }
    }
}

// Function to print tabs to help see the binary tree better
void printTabs(int num) {
    for(int i = 0 ; i < num ; i++)
    {
        printf("\t");
    }
}

// Pre-order traversal
void displayDataBinary(tree *root, int level) {
    int i;
    if(root == NULL) 
    {
        printTabs(level);
        printf("Empty\n");
        return;
    }
    printTabs(level);
    printf("Root = [%d] %s, Locations: ", root->ID, root->name);
    for(i = 0 ; root->place[i] != 0 ; i++)
    {
        printf("%d ", root->place[i]);
    }
    printf("\n");
    printTabs(level);

    printf("Left\n");
    displayDataBinary(root->left,level+1);
    printTabs(level);

    printf("Right\n");
    displayDataBinary(root->right,level+1);
    printTabs(level);
    printf("\n");
}


void checkSiteBinary(tree *root, int site) { // in-order traversal
    if(root != NULL)
    {
        checkSiteBinary(root->left, site);
        for(int i = 0 ; i < 5 ; i++)
        {
            if(root->place[i] == site)
            {
                printf("%s\n", root->name);
            }
        }
        checkSiteBinary(root->right, site);
    }
} 


void printTabsToFile(FILE *fp, int num) {
    for(int i=0 ; i < num ; i++)
    {
        fprintf(fp, "\t");
    }
}

void printTreeToFile(tree *root, FILE *fp, int level) {
    int i;
    if(root == NULL) 
    {
        printTabsToFile(fp, level);
        fprintf(fp, "Empty\n");
        return;
    }
    printTabsToFile(fp, level);
    fprintf(fp, "Root = [%d] %s, Locations: ", root->ID, root->name);
    for(i = 0 ; i < 5 ; i++)
    {
        fprintf(fp, "%d, ", root->place[i]);
    }
    fprintf(fp, "\n");
    printTabsToFile(fp, level);

    fprintf(fp, "Left\n");
    printTreeToFile(root->left, fp, level+1);
    printTabsToFile(fp, level);

    fprintf(fp, "Right\n");
    printTreeToFile(root->right, fp, level+1);
    printTabsToFile(fp, level);
    fprintf(fp, "end\n");
}

void saveDataBinary(tree *root) {
    FILE *fp;
    int size;
    char file [30];
    time_t t = time(NULL);
    struct tm date = *localtime(&t);
    for(;;)
    {
        printf("Enter the name of the file that you would like to save to (including the file extension):\n");
        scanf("%s", file);
        if(strcmp(file, "back") == 0) break;
        else
        {
            fp = fopen(file, "r+");
            if (fp == NULL)
            {
                printf("File not found, please try again,\n\n");
            }
            else
            {
                fclose(fp);
                fp = fopen(file, "a");
                fseek(fp, 0, SEEK_END);
                fprintf(fp, "%02d/%02d/%d %02d:%02d\n", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min);
                printTreeToFile(root, fp, 0);
                fprintf(fp, "\n");

                fclose(fp);
                printf("The current data in the app has been saved in %s\n\n", file);
                break;
            }
        }
    }
}