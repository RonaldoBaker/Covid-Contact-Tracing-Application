/********************************************************************************************
 * File: CovidApp.c
 * Author: Ronaldo R Baker 
 * Date: Last modified 24 SEP 2023
 * Description:
 * This console-based application replicates the NHS Contact Tracing App used in the COVID-19
 * pandemic. Two versions of the application have been implemented, which uses two data 
 * structures - a linked list or a binary search tree. The version can be chosen by the user
 * by entering 'linkedlist' or 'binarytree' as an argument when running the executable file 
 * in the command line.
 *  
 * There are 8 options to choose from the main menu, with the ability to enter 'back'
 * to return to the main menu, if you are unable to enter a piece of data.
 * 
 * NOTE:
 * User must be careful to not enter strings as an option in the main menu, as no code has 
 * yet been written to handle this exception.
*********************************************************************************************/

#include "linkedlist.h"
#include "binarytree.h"

int main(int argc, char *argv[]) {

    // Initialise variables 
    int parameter;
    int choice, site;
    char inputName[15];
    char version;
    bool userExist;

    // Establish which data structure that will be used
    for(int i = 0 ; i < argc ; i++ )
    {
        if(strcmp(argv[i], "linkedlist") == 0) parameter = 1;
        else if(strcmp(argv[i], "binarytree") == 0) parameter = 2;
        else parameter = 0;
    }

    if (parameter == 0)
    {
        printf("Too few arguments have been passed\n");
        return 0;
    }
    else if(parameter == 1) printf("Welcome to the COVID-19 Contact Tracing App - Linked List\n");
    else printf("Welcome to the COVID-19 Contact Tracing App - Binary Tree\n");

    while(choice != 8) 
    {
        printf("Which option would you like to choose?\n");
        printf(" 1.Introduce new user\n 2.Log site visit\n 3.Display current data\n 4.Remove user\n");
        printf(" 5.Check a site\n 6.Save to file\n 7.Retrieve data from a file\n 8.Exit the app\n");
        scanf("%d", &choice);

        /*
        To implement the same capabilities as linked list version of asking for user input until it 
        is valid, or allowing user to go back to main menu, the code branch for binary tree option is
        substantially longer due to the recursive nature of the functions. 
        These capabilities could not be put inside the functions itself without unwanted recursions, 
        so is a part of the main function
        */
        switch(choice)
        {
            case 1://Introducing a new user
            if(parameter == 1) insertUserLinear();
            else
            {
                for(;;)
                {
                    printf("Enter name:\n"); scanf("%s", inputName);
                    if(strcmp(inputName, "back") == 0) break;
                    else
                    {
                        userExist = findUserBinary(root, inputName);
                        if(userExist)
                        {
                           printf("This user, '%s', has already been entered, please try again.\n\n", inputName);
                           continue;
                        }
                        else
                        {
                            insertUserBinary(&root, inputName);
                            break;
                        } 
                    }
                }
            }
            break;


            case 2: // Logging a user at a site
            if(parameter == 1) logSiteLinear();
            else
            {
                for(;;)
                {
                    printf("Which user are you logging for?\n"); scanf("%s", inputName);
                    if(strcmp(inputName, "back") == 0) break;
                    else
                    {
                        userExist = findUserBinary(root, inputName);
                        if (userExist)
                        {
                            printf("Enter the number of the site this user has been to:\n"); scanf("%d",&site);
                            logSiteBinary(root, inputName, site);
                            break;
                        }
                        else
                        {
                            printf("This user, '%s', does not exist, please try again.\n\n", inputName);
                            continue;
                        }
                    }
                }
            }
            break; 


            case 3: // Printing all the names in the binary tree
            if(parameter == 1) displayDataLinear();
            else
            {
                displayDataBinary(root, 0);
                printf("End of binary tree\n\n");
            }
            break;


            case 4:// Removing a user
            if(parameter == 1) removeUserLinear();
            else
            {
                for(;;)
                {
                    printf("Which user would you like to remove?\n"); scanf("%s", inputName);
                    if(strcmp(inputName, "back") == 0) break;
                    else
                    {
                        userExist = findUserBinary(root, inputName);
                        if (userExist)
                        {
                            root = removeUserBinary(root, inputName);
                            break;
                        }
                        else
                        {
                            printf("This user, '%s', does not exist, please try again.\n\n", inputName);
                            continue;
                        } 
                    }
                }
            }
            break;


            case 5:// Checking each site
            if(parameter == 1) checkSiteLinear();
            else
            {
                printf("Which site would you like to check?\n"); scanf("%s", inputName);
                if(strcmp(inputName, "back") == 0) break;
                else
                {
                    site = atoi(inputName);
                    printf("The following person(s) have been to site %d\n", site);
                    checkSiteBinary(root,site);
                }
            }
            break;


            case 6:// Saves to a file of the user's choice, or creates a new file if that file doesn't exist
            if(parameter == 1) saveDataLinear();
            else saveDataBinary(root);
            break;


            case 7: //Retrieves a file of the user's choice
            retrieveData();
            break;


            case 8:
            printf("Exiting COVID-19 Contact Tracing App\n");
            return 1;
            break;


            default:
            printf("An invalid number has been entered\nPlease try again\n");
        }
    }
}