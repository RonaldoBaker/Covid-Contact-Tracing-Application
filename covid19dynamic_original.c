/********************************************************************************************
 * File: covid19dynamic_original.c
 * Author: Ronaldo R Baker 
 * Date: Last modified 28 AUG 2022
 * Description:
 * This is the original version of the console-based application which replicates the 
 * NHS Contact Tracing App used in the COVID-19 pandemic. Two versions of the application 
 * have been implemented, which uses two data structures - a linked list or a binary search tree. 
 * The version can be chosen by the user by entering 'linkedlist' or 'binarytree' as an argument 
 * when running the executable file in the command line.

 * NOTE:
 * This program is only make comparisons with the more update version. This file can be run, but may encounter
 * a lot more exceptions and errors. 
 * The binary tree version in this program is not fully functional. In particular, the remove user function
 * does not work due to incorrect algorithm and the save function does not actually save to a file, but prints to
 * console.
 *********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct for linked list
typedef struct profile {
  char name[15];
  int place[5]; 
  struct profile *next;
} profile;

// Struct for binary tree
typedef struct tree {
    char name[15];
    int place[5];
    struct tree *left;
    struct tree *right;
} tree;

// Defining these pointers globally so all functions can access them

// Pointers for linked list
profile *head = NULL; // The first node 
profile *curr;
profile *prev;
profile *insert;

//Pointer for binary tree
tree *root = NULL;

// Functions for linked list 
void insertLinear(char *stringName) {
  int i;
    if(head==NULL)
    {
        //Insertion of the first node 
        head = (struct profile*) malloc(sizeof(struct profile));
        strcpy(head->name,stringName);
        // This for loop initialises all the places in the array to 0 to help input places later on in case 2
        for(i = 0 ; i < 5 ; i++) 
        {
          head->place[i] = 0;
        }
        head->next = NULL;
    }
    else 
    {
        insert = (struct profile*) malloc(sizeof(struct profile));
        strcpy(insert->name,stringName);
        // This for loop initialises all the places in a profile to 0 to assist inputting places later on in case 2
        for(i = 0 ; i < 5 ; i++) 
        {
          insert->place[i] = 0;
        }
        if(strcmp(stringName,head->name)==0)
        {
          printf("There is already a user with this name\nPlease enter a different name\n");
        }
        if(strcmp(stringName,head->name)<0)
        {
          //Insertion at the head
          insert->next = head; 
          head = insert;      // Making the new head the current pointer 
        }
        if(strcmp(stringName,head->name)>0)
        {
          curr = head;
          while((strcmp(stringName,curr->name) > 0) && (curr->next != NULL))
          {
            // Loop to traverse the linked list
            prev = curr; 
            curr = curr->next; 
          }
          if(strcmp(stringName,curr->name)<0) 
          {
            // Insertion in the middle
            prev->next = insert; 
            insert->next = curr; 
          }
          if(curr->next == NULL) 
          {
            // Insertion at the end
            curr->next = insert; 
            insert->next = NULL; 
          }
          if(strcmp(stringName,curr->name)==0)
          {
            printf("This user has already been entered\nPlease enter a different name\n");
          }
        }
    }
}

void removeLinear(char *stringName) {

  /* Checking if the 'inputName' corresponds to the first node
  meaning the condition is different to other cases*/
  if(strcmp(stringName,head->name)==0) 
  {
    curr = head; 
    head = curr->next;
    free(curr);
  }
  else
  {
    curr = head;
    while(strcmp(stringName,curr->name) > 0)
    {
      prev = curr; 
      curr = curr->next;
    }
    if(strcmp(stringName,curr->name)==0)
    {
      prev->next = curr->next; 
      free(curr);
    }       
  }
}

void saveLinear(FILE *filepointer) {
    curr = head;
    while(curr != NULL) //Loop to traverse the nodes
    {
        fprintf(filepointer,"Name: %s  ",curr->name);
        fprintf(filepointer,"Sites visited: ");
        for(int n = 0 ; n < 5 ; n++) //Loop to print all the sites 
        {
            if(curr->place[n] != 0)
            {
                fprintf(filepointer,"%d ",curr->place[n]);
            }
        }
        fprintf(filepointer,"\n");
        curr = curr->next; 
    }
}

// Functions for binary tree
// There are many functions for binary trees to take advantage of their recursive nature 
tree* createnode(char *string) {

    tree *newNode = malloc(sizeof(tree));
    newNode->left = newNode->right = NULL;
    strcpy(newNode->name,string);
    /* Loop to initialise all the places in the array to 0, 
    to help with logging sites later on */
    for(int i = 0 ; i < 5 ; i++)
    {
        newNode->place[i] = 0;
    }
    return newNode;
}

int insertBinary(tree **rootptr, char *string) {
    tree *root = *rootptr;

    if(root==NULL)
    {
        //tree is empty
        *rootptr = createnode(string);
        return 1;
    }
    if(strcmp(string,root->name)==0) 
    {
        //Do nothing because that value is already in the tree
        return 0;
    }
    if(strcmp(string,root->name)<0)
    {
        return insertBinary(&(root->left),string);
    }
    else
    {
        return insertBinary(&(root->right),string);
    }
}

int logsiteBinary(tree *root, char *string, int site) {
    int j = 0;
    if(root==NULL)
    {
        return 0;
    }
    if(strcmp(string,root->name)==0)
    {
        while(root->place[j] != 0)
        {
            j++;
        }
        root->place[j] = site;
    }
    if(strcmp(string,root->name)<0)
    {
        logsiteBinary(root->left,string,site);
    }
    else 
    {
        logsiteBinary(root->right,string,site);
    }
    return 1;
}

/* Seems to be an issue with this function as sometimes 
after removing a node in the binary search tree, choosing another option 
causes a segmentation fault */
tree* removeBinary(tree *root,char *string) { 
    if(root==NULL)
    {
        return root;
    }
    if(strcmp(string,root->name)<0)
    {
        root->left = removeBinary(root->left,string);
    }
    if(strcmp(string,root->name)>0)
    {
        root->right = removeBinary(root->right,string);
    }
    if(strcmp(string,root->name)==0)
    {
        //case 1 - the node is a leaf (0 children)
        if(root->left==NULL && root->right==NULL)
        {
            // free(root);
            root = NULL;
            return root;
        }
        // case 2 - the node has 1 child
        if(root->left==NULL && root->right != NULL)
        {
            root = root->right;
            return root;
        }
        if(root->right==NULL && root->left != NULL)
        {
            root = root->left;
            return root;
        }
        // case 3 - the node has 2 children
        if(root->left != NULL && root->right!=NULL)
        {
            tree *temp = root->left;
            while (temp->right != NULL) // Finding the max value in the left subtree 
            {
                temp = temp->right;
            }
            strcpy(temp->name,root->name);
            root->left = removeBinary(root->left,string);
        }
    }
}

// Used to help print out the binary tree in a more user-friendly format
char *printtabs(int num) {
    for(int i=0 ; i < num ; i++)
    {
        printf("\t");
    }
}

void printtree(tree *root, int level) {
    if(root == NULL) 
    {
        printtabs(level);
        printf("Empty\n");
        return;
    }
    printtabs(level);
    printf("Root = %s ",root->name);
    for(int i = 0 ; i < 5 ; i++)
    {
        if(root->place[i] != 0)
        {
            printf("%d ",root->place[i]);
        }
    }
    printf("\n");
    printtabs(level);

    printf("Left\n");
    printtree(root->left,level+1);
    printtabs(level);

    printf("Right\n");
    printtree(root->right,level+1);
    printtabs(level);
    printf("Done\n");
}

void checksite(tree *root, int site) { // in-order traversal
    if(root!=NULL)
    {
        checksite(root->left,site);
        for(int i = 0 ; i < 5 ; i++)
        {
            if(root->place[i] == site)
            {
                printf("%s\n",root->name);
            }
        }
        checksite(root->right,site);
    }
} 

void saveBinary(FILE *filepointer,tree *root) {
    if(root!=NULL)
    {
        saveBinary(filepointer,root->left);
        fprintf(filepointer,"Name: %s  ",root->name);
        fprintf(filepointer,"Sites visited: ");
        for(int i = 0 ; i < 5 ; i++)
        {
            if(root->place[i] != 0)
            {
                fprintf(filepointer,"%d ",root->place[i]);
            }
        }
        fprintf(filepointer,"\n");
        saveBinary(filepointer,root->right);
    }
}

int main(int argc, char *argv[]) {

    char parameter[20];
    /* This loop establishes which data structure the user passed to the main function 
    so that the program knows which set of instructions/functions to folllow for the 
    specified data structure */
    for(int i = 0 ; i <argc ; i++)
    {
        if(strcmp(argv[i],"linkedlist")==0 || strcmp(argv[i],"binarytree")==0)
        {
            strcpy(parameter,argv[i]);
        }
    }
    
    // Initialising the necessary variables
    FILE *fp;
    int j,k,l,m;
    int choice,site,index,size;
    char inputName[15];
    char file[30];

    printf("Welcome to the COVID-19 Contact Tracing App\n");

    while(choice != 8) //Loop to continuously ask the user which option they would like to choose
     {
        printf("Which option would you like to choose?\n");
        printf(" 1.Introduce new user\n 2.Log site visit\n 3.Remove user\n 4.Check users who were logged at a site\n");
        printf(" 5.Save to file\n 6.Retrieve data from a file\n 7.Display current data\n 8.Exit the app\n");
        scanf("%d",&choice);
    
        switch(choice)
        {
            case 1:
            printf("Enter name:\n"); scanf("%s",inputName);
            if(strcmp(parameter,"linkedlist")==0)
            {
                insertLinear(inputName);
            }
            else if(strcmp(parameter,"binarytree")==0)
            {
                insertBinary(&root,inputName);
            }
            printf("New user '%s' has been saved\n",inputName);
            break;


            case 2: // Log a user at a site
            printf("Which user are you logging for?\n"); scanf("%s",inputName);
            printf("Enter the number of the site this user has been to:\n"); scanf("%d",&site);
            if(strcmp(parameter,"linkedlist")==0)
            {
                 curr = head; 
                while(strcmp(inputName,curr->name) > 0)
                {
                    curr = curr->next;
                }
                if(strcmp(inputName,curr->name)==0) 
                {
                    while(curr->place[j] != 0) //While loop to check if places in the array are full or not
                    {
                        j++;
                    }
                    curr->place[j] = site;
                    printf("%s has been logged at site %d\n",curr->name,curr->place[j]);
                    j = 0; //Setting the variable back to 0 for the next user
                } 
            }
            else if(strcmp(parameter,"binarytree")==0)
            {
                if(logsiteBinary(root,inputName,site)==0)
                {
                    printf("This user does not exist\nPlease try again");
                }
                else 
                {
                    printf("%s has been logged at site %d\n",inputName,site);
                }
             }
             break; 


            case 3: // Removing a user 
            printf("Which user would you like to remove?\n"); scanf("%s",inputName);
            if(strcmp(parameter,"linkedlist")==0)
            {
                removeLinear(inputName);
            }
            else if(strcmp(parameter,"binarytree")==0)
            {
                removeBinary(root,inputName); 
            }
            printf("'%s' has been removed\n",inputName);
            break;    


            case 4: //Checking each site 
            printf("The following person(s) have been to site %d\n",site);
            printf("Which site would you like to check?\n"); scanf("%d",&site);
            if(strcmp(parameter,"linkedlist")==0)
            {
                while(curr != NULL)
                curr = head;
                {
                    for(k = 0 ; k < 5 ; k++)
                    {
                        if(curr->place[k] == site)
                        {
                             printf("%s\n",curr->name);
                        }
                    }
                    curr = curr->next;
                }
            }
            else if(strcmp(parameter,"binarytree")==0)
            {
                checksite(root,site);
            }
            printf("End of check\n");
            break;


            case 5: // Saves to a file of the users choice, this file must already exist  
            printf("Enter the name of the file that you would like to save to (including the file extension):\n");
            scanf("%s",file);
            fp = fopen(file,"r+"); //Open file to initally read
            if(fp == NULL)
            {
                printf("File not found\nPlease try again");
            }
            fseek (fp, 0, SEEK_END);
            size = ftell(fp); // Returns the size of the file in terms of bytes
            if (size == 0) // The file is empty and can simply write to the file
            {
                if(strcmp(parameter,"linkedlist")==0)
                {
                    saveLinear(fp);
                }
                else if(strcmp(parameter,"binarytree")==0)
                {
                    saveBinary(fp,root);
                }
            }
            else
            {
                fclose(fp); // Closing, then opening to append, instead of erasing the data in a pre-existing file by just writing
                fp = fopen(file,"a");
                if(strcmp(parameter,"linkedlist")==0)
                {
                    saveLinear(fp);
                }
                else if(strcmp(parameter,"binarytree")==0)
                {
                    saveBinary(fp,root);
                }
            }
            fclose(fp);
            printf("The current data in the app has been saved in the '%s' file\n",file);  
            break;
      

            case 6: //Retrieving data from a file
            printf("Enter the name of the file you would like to read (including the file extension):\n");
            scanf("%s",file);
            fp = fopen(file,"r");
            l = getc(fp); // Using 'l' to retrive data from the file
            while(l != EOF) // EOF meaning 'end of file'
            {
                printf("%c",l);
                l = getc(fp);
            }
            printf("Finish reading %s file\n",file);
            break; 
    

            case 7: // Displaying all the names in the linked list or binary tree
            if(strcmp(parameter,"linkedlist")==0)
            {
                curr = head; 
                while(curr != NULL)
                {
                    printf("%s : ",curr->name); 
                    for( m = 0 ; curr->place[m] != 0 ; m++) // For loop to check if places in the array are full or not
                    {
                        printf("%d ",curr->place[m]);
                    }
                    printf("\n");
                    curr = curr->next;
                 }
                 printf("End of list\n");
            }
            else if(strcmp(parameter,"binarytree")==0)
            {
                printtree(root,0);
                printf("End of binary tree\n");
            }
            break;


            case 8: //Exit 
            printf("Exiting COVID-19 Contact Tracing App\n");
            return 0;
            break;


            default:
            printf("An invalid number has been entered\nPlease try again\n"); 
        }
    }
    return 0;   
}
