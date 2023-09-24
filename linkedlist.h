#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct list {
    char name[15];
    int place[5];
    int ID; 
    struct list *next;
} list;

list *head = NULL; // The first node 
list *curr; // Temporary current node
list *prev; // Temporary previous node
list *insert; // New node to insert
int currentID_L = 0;


list* createUserLinear(char *name) {
    insert = (list*) malloc(sizeof(list));
    strcpy(insert->name, name);
    currentID_L++;
    insert->ID = currentID_L;
    insert->next = NULL;
    // Initialising all places to 0 for later site logging
    for(int i = 0 ; i < 5 ; i++) 
    {
        insert->place[i] = 0;
    }
    return insert;
}


bool findUserLinear(char *name) {
    curr = head;
    while(curr != NULL)
    {
        if (strcmp(name, curr->name) == 0)
        {
            return true;
        }
        else
        {
            curr = curr->next;
        }
    }
    return false;
}


void insertUserLinear() {
    int i;
    char inputName [15];
    bool userExist;
    for(;;)
    {
        printf("Enter name:\n"); scanf("%s", inputName);
        if(strcmp(inputName, "back") == 0) break;
        else
        {
            userExist = findUserLinear(inputName);
            if(userExist)
            {
                printf("This user, '%s', has already been entered, please try again.\n\n", inputName);
            }
            else
            {
                if(head == NULL) 
                {
                    head = createUserLinear(inputName);
                    head->next = NULL;
                    printf("New user '%s' has been saved. This user's ID is %d.\n\n", inputName, head->ID);
                    break;
                }
                else 
                {
                    insert = createUserLinear(inputName);
                    //Insertion at the head
                    if(strcmp(inputName, head->name) < 0) 
                    {    
                        insert->next = head; 
                        head = insert;     
                        printf("New user '%s' has been saved. This user's ID is %d.\n\n", inputName, insert->ID);
                        break; 
                    }
                    if(strcmp(inputName, head->name) > 0) 
                    {
                        curr = head;
                        while((strcmp(inputName, curr->name) > 0) && (curr->next != NULL)) // Loop to traverse the linked list
                        {
                            prev = curr; 
                            curr = curr->next; 
                        }
                        // Insertion in the middle
                        if(strcmp(inputName, curr->name) < 0) 
                        {
                            prev->next = insert; 
                            insert->next = curr;
                            printf("New user '%s' has been saved. This user's ID is %d.\n\n", inputName, insert->ID);
                            break;
                        }
                        // Insertion at the end 
                        if((strcmp(inputName, curr->name) > 0) && (curr->next == NULL)) 
                        {
                            curr->next = insert; 
                            insert->next = NULL; 
                            printf("New user '%s' has been saved. This user's ID is %d.\n\n", inputName, insert->ID);
                            break;
                        }
                    }
                }
            }
        }
    }
}


void logSiteLinear() {
    int site, i = 0;
    char inputName [15];
    bool userExist;
    for(;;)
    {
        printf("Which user are you logging for?\n"); scanf("%s", inputName);
        if(strcmp(inputName, "back") == 0) break;
        else
        {
            userExist = findUserLinear(inputName);
            if(userExist)
            {
                curr = head;
                while(strcmp(inputName, curr->name) > 0 && (curr->next != NULL))
                {
                    curr = curr->next;
                }
                printf("Enter the number of the site this user has been to:\n"); scanf("%d", &site);
                while(curr->place[i] != 0)
                {
                    i++;
                }
                curr->place[i] = site;
                printf("%s has been logged at site %d.\n\n", curr->name, curr->place[i]);
                i = 0; // To log site for next user
		    	break;         
            }
            else
            {
                printf("This user, '%s', does not exist, please try again.\n\n", inputName);
            }
        }
    }
}


void removeUserLinear() {
    char inputName [15];
    bool userExist;
    for(;;)
    {
        printf("Which user would you like to remove?\n"); scanf("%s", inputName);
        if(strcmp(inputName, "back") == 0) break;
        else
        {
            userExist = findUserLinear(inputName);
            if(userExist)
            {
                if(strcmp(inputName, head->name) == 0) // Name corresponds to first node
                {
                    curr = head;
                    head = curr->next;
                    free(curr);
                    printf("The user '%s' has been removed.\n\n", inputName);
                    break;
                }
                else // Every other node
                {
                    curr = head;
                    while(strcmp(inputName, curr->name) > 0 && (curr->next != NULL))
                    {
                        prev = curr;
                        curr = curr->next;
                    }
                    prev->next = curr->next;
                    free(curr);
                    printf("The user '%s' has been removed.\n\n", inputName);
                    break;
                }
            }
            else
            {
                printf("This user, '%s', does not exist, please try again.\n\n", inputName);
            }
        }
    }  
}


void displayDataLinear() {
    curr = head;
    while(curr != NULL)
    {
        printf("[%d] %s, Locations: ", curr->ID, curr->name);
        for(int i = 0 ; curr->place[i] != 0 ; i++)
        {
            printf("%d ", curr->place[i]);
        }
        printf("\n");
        curr = curr->next;
    }
    printf("End of list\n\n");
}


int checkSiteLinear() {
    int site, i;
    char option [5];
    printf("Which site would you like to check?\n"); scanf("%s", option);
    if(strcmp(option, "back") == 0) 
    {
        return 0;
    }
    else
    {
        site = atoi(option);
        printf("The following person(s) have been to site %d:\n", site);
        curr = head;
        while(curr != NULL)
        {
          for(i = 0 ; i < 5 ; i++)
          {
            if(curr->place[i] == site)
            {
              printf("[%d] %s\n", curr->ID, curr->name);
            }
          }
          curr = curr->next;
        }
        printf("End of check\n\n");
        return 1;
    }
}


void saveDataLinear() {
    FILE *fp;
    int size, i;
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
            fp = fopen(file, "r");
            if(fp == NULL) // Checking if file exists
            {
                printf("File not found, please try again.\n\n");
            }
            else 
            {
                fclose(fp);
                fp = fopen(file, "a");
                curr = head;
                fprintf(fp, "%02d/%02d/%d %02d:%02d\n", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min);
                while(curr != NULL) 
                {    
                    fprintf(fp, "[%d] %s, Locations: ", curr->ID, curr->name);
                    for(i = 0 ; curr->place[i] != 0 ; i++) 
                    {
                        fprintf(fp,"%d ", curr->place[i]);
                    }
                    fprintf(fp, "\n");
                    curr = curr->next; 
                }
                fprintf(fp, "\n");
                fclose(fp);
                printf("The current data in the app has been saved in %s\n\n", file);
                break;
            }
        }
    }
}


void retrieveData() {
    FILE *fp;
    int i;
    char file[30];
    for(;;)
    {
        printf("Enter the name of the file you would like to read (including the file extension): \n");
        scanf("%s",file);
        if(strcmp(file, "back") == 0) break;
        else
        {
            fp = fopen(file, "r");
            if(fp == NULL)
            {
                printf("File not found, please try again.\n\n");
            }
            else
            {
                i = getc(fp);
                while(i != EOF)
                {
                    printf("%c", i);
                    i = getc(fp);
                }
                printf("Finished reading %s\n\n", file);
                break;
            }
        }
    }
}