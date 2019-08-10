#include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <ctype.h>
  
  #define QUIT -1
  #define ADD 1
  #define PRINT 2
  #define DELETE 3
  #define LOAD 4
  
  #define SAVE 5
  #define DISPLAYALL 6

//------------------------------------------------------  
//-- struct making a phonebook with the following info
//------------------------------------------------------
typedef struct phoneBook {
  char firstName[20];
  char lastName[20];
  char phoneNum[15];
  
} PhoneBook;

//------------------------------------------------------
//-- struct represents BST to store the the Phone Book
//------------------------------------------------------
typedef struct bst_node {
  PhoneBook *record;
  struct bst_node *left;
  struct bst_node *right;
} bst_node;

//-----------------------------------------------------
//-- struct represents a Linked-List used for sorting
//-----------------------------------------------------
typedef struct linked_node {
  PhoneBook *record;
  struct linked_node *next;
} linked_node;

//-- make the list accessible to the entire program
linked_node *sortedList = NULL;

/***********************************************/
/************** F U N C T I O N S **************/
/***********************************************/

/*********************************************************
  * CREATE a record structure & initial the phone record *
  ********************************************************/
PhoneBook* AddPhoneBook(char firstName[20], char lastName[20], char phoneNum[15])
{
  PhoneBook *ptr = (PhoneBook *) malloc (sizeof(PhoneBook));
  strcpy(ptr->firstName, firstName);
  strcpy(ptr->lastName, lastName);
  strcpy(ptr->phoneNum, phoneNum);
  return ptr;
}
/************************************************
  * INSERT record into BST via the phone number *
  ***********************************************/
bst_node* insert (bst_node *tree, PhoneBook *record)
{
  bst_node *tmp = NULL;
  if (tree == NULL) {
    tmp = (bst_node *)malloc(sizeof(bst_node));
    tmp->left = tmp->right = NULL;
    
    // ADD record here
    tmp->record = (PhoneBook *) malloc (sizeof(PhoneBook));
    strcpy(tmp->record->firstName, record->firstName);
    strcpy(tmp->record->lastName, record->lastName);
    strcpy(tmp->record->phoneNum, record->phoneNum);
    return tmp;
  }
  else {
    if (strcmp(record->phoneNum, tree->record->phoneNum) < 0)
      tree->left  = insert(tree->left, record);
    else
      tree->right = insert(tree->right, record);
    return tree;
  }
}

/****************************************************
  * PRINT an individual record via the phone number *
  ***************************************************/
void printNode(PhoneBook *record)
{
  printf("Phone Number: %s", record->phoneNum);
  printf("\nFirst Name: %s", record->firstName);
  printf("\nLast Name: %s", record->lastName);
  printf("\n\n"); 
}

/***************************************************
 * used as a helper function for the sort algorithm*
 * get & extract the field given the user's choice *
 * and return the field back as the key to compare *
 ***************************************************/
char* get_choice_key(PhoneBook *record, int choice)
{    
    switch (choice) {
       case 1: return record->phoneNum;
               break; 
       case 2: return record->firstName;
               break;
       case 3: return record->lastName;
               break;
     }
}

/********************************************************
 * make a new node dynamically based on the give record *
 * used as a helper function to build the sorted list   *
 ********************************************************/
linked_node* make_node(PhoneBook *record)
{
   linked_node *node = (linked_node *)malloc(sizeof(linked_node));
   node->record = (PhoneBook *) malloc (sizeof(PhoneBook));
   strcpy(node->record->phoneNum, record->phoneNum);
   strcpy(node->record->firstName, record->firstName);
   strcpy(node->record->lastName, record->lastName);
   node->next = NULL;
   
   return node;            
}

/******************************************************
 * create the sorted Linked-List (on the given field) *
 ******************************************************/
void add_to_sorted_list(PhoneBook *record, int choice)
{
   char key[30], other_key[30];
   linked_node *tmp = NULL, *prev = NULL, *new_node = NULL;
   
   new_node = make_node(record);
   
   if (!sortedList) {
     sortedList = new_node;
   }
   else { //-- place the new node it the ascending position
          //-- based on choice...[phone #=1, first name=2 or last name =3]
     //--  set the key to build the list based on the user's field preference
     strcpy(key, get_choice_key(record, choice));
     tmp = sortedList;
       
     while (tmp != NULL) {
        //-- other_key = get_choice_key(tmp->record, choice);
        strcpy(other_key, get_choice_key(tmp->record, choice));
        if (strcmp(key, other_key) < 0) 
          break;
        prev = tmp;
        tmp = tmp->next;
     }
     if (prev == NULL) {
       new_node->next = sortedList;
       sortedList = new_node;
     }
     else {
       new_node->next = tmp; 
       prev->next = new_node;
     }
   }    
}

/****************************************************
 * clear the linked list dynamically free each node *
 ****************************************************/
void clear_list()
{
  linked_node *n = sortedList;
  
   while (n != NULL) {
     linked_node *n1 = n;
     n = n->next;
     free(n1->record);
     free(n1);
   }     
   sortedList = NULL; 
}

/*************************************************************************
 * get the selected field of choice and return its string representation *
 *************************************************************************/
char *get_choice(int choice)
{
   if (choice == 1)
     return "Phone Number";
   else if(choice == 2)
     return "First Name";
   else 
     return "Last Name";
}

/*****************************************************************************
 * print the sorted linked-list in ascending order given the choice of field *
 *****************************************************************************/
void printList(int choice)
{
   linked_node *tmp = sortedList;
   
   printf("\nPrint PhoneBook in ascending order based on \"%s\"\n", get_choice(choice));
   printf("=========================================================\n");
   while (tmp != NULL) {
     printf("Phone #: %s\n", tmp->record->phoneNum);       
     printf("First Name: %s\n", tmp->record->firstName);
     printf("Last Name: %s\n\n", tmp->record->lastName);
     tmp = tmp->next;
   }    
}

/******************************************************
  * SORT the Phone Book on given field, if choice is: *
  *  [1] sort on phone number                         *
  *  [2] sort on first name                           *
  *  [3] sort on last name                            *
  *                                                   *
  * We use an inorder travesal to get each record     *
  * from the BST and insert into the linked-list.     *
  * each insertion is positional give the choice key  *
  *****************************************************/
void sort(bst_node *tree, int choice)
{
  if (tree != NULL ) {
    sort(tree-> left, choice);
    add_to_sorted_list(tree->record, choice);
    sort(tree->right, choice);
  }
}



bst_node* getSuccessor(bst_node *node)
{
  while(node->right != NULL)
    node = node->right;
  return node;
}

/***********************************************
  * DELETE record into BST via the phone number *
  ***********************************************/
bst_node* delete_record(bst_node *node, char *data)
{
  bst_node *retval, *successor;
 
  if(node == NULL)
  {
    printf("\nERROR: Phone Record does not exists\n");
    return node;
  }
 
  if(strcmp(data, node->record->phoneNum) == 0)
  {
    retval = NULL;

  
    if(node->left == NULL)
    {
      retval = node->right;
			printf("\Phone Record containing Phone Number %s has been deleted\n",data);
      free(node);
      return retval;
    }
    else if(node->right == NULL)
    {
      retval = node->left;
			printf("\Phone Record containing Phone Number %s has been deleted\n",data);
      free(node);
      return retval;
    }
    else
    {
       successor = (bst_node *)getSuccessor(node->left);
       strcpy(node->record->phoneNum, successor->record->phoneNum);
       strcpy(node->record->firstName, successor->record->firstName);
       strcpy(node->record->lastName, successor->record->lastName);
       node->left = delete_record(node->left, successor->record->phoneNum);
    }
  }
  else if(strcmp(data, node->record->phoneNum) < 0)
  {
    node->left = delete_record(node->left, data);
  }
  else
  {
    node->right = delete_record(node->right, data);
  }
 
  return node;
}
 
/***********************************************
  * SEARCH record into BST via the phone number *
  ***********************************************/
bst_node* search(bst_node *tree, char val[]) {
  
  bst_node *tmp = tree;
  
  while (tmp != NULL)
  {
    if(strcmp(val, tmp->record->phoneNum) == 0)
      return tmp;
    else if (strcmp(val, tree->record->phoneNum) < 0)
      tmp = tmp->left;      
    else 
      tmp = tmp->right;
  }
  
  return tmp;
}

/**********************************************************
  * MENU selection - valid choices from [1-6] respectively *
  **********************************************************/
int menu()
{
  int choice;
  // ask user an option to take an action
  printf("Enter 1 to Add a New Phone Book Record OR \n");
  printf("Enter 2 to Print a Phone Book Record based on Phone Number OR \n");
  printf("Enter 3 to Delete a Phone Book Record based on Phone Number OR \n");
  printf("Enter 4 to Load Data From File OR \n");
  printf("Enter 5 to Save Data Into File OR \n");
  printf("Enter 6 to Display all Records in Ascending Order based on Phone Number OR \n");
  printf("\nEnter any other character to Quit > ");
  scanf("%d", &choice);
  printf("\n");   
  
  if (choice < 1 || choice > 6)
    choice = -1;
  
  return choice; 
}

/********************************************
  * MAIN - drives the phone book operations *
  *******************************************/
int main(int argc, char * argv[])
{
  // Declaration
  char firstName[20], lastName[20], phoneNum[15], fileName[30];
  char * input;
  FILE *fp = NULL, *fp_load = NULL;
  fp = fopen("ProjectFile.txt", "r");
  PhoneBook *ptr = NULL;
  
  bst_node *tree = NULL, *search_node = NULL;
  linked_node *tmp = NULL;
  int choice = -1;
  
  if (fp) {
    //-- automatically load the Phone Book via text file
    //-- the loop reads three string per line, if EOF it'll break loop
    while (fscanf(fp, "%s %s %s", firstName, lastName, phoneNum) == 3) { 
      //-- add phone book record to the BST
      ptr = AddPhoneBook(firstName, lastName, phoneNum);
      tree = insert(tree, ptr);
    }
    fclose(fp);
  }
  
  //-- menu returned -1 as sentinel value for an invalid choice or quit
  while ((choice = menu()) != QUIT)
  {
    if (choice == ADD)
    {
      // Take input for a new phone book record
      printf("Insert a new phone record into the phone book\n");
      printf("Enter First Name > ");
      scanf("%s", firstName);
      printf("Enter Last Name > ");
      scanf("%s", lastName);
      printf("Enter Phone Number [format: xxx-xxx-xxxx] > ");
      scanf("%s", phoneNum);
      
      // add a new record at tail of linkedlist
      ptr = AddPhoneBook(firstName, lastName, phoneNum);
      tree = insert(tree, ptr);
    }
    else if (choice == PRINT)
    {
      ptr = NULL;
      phoneNum[0] = '\0';
      printf("Search the phone book for a Phone Number\n");
      printf("Enter Phone Number [format: xxx-xxx-xxxx] >: ");
      scanf("%s", phoneNum);
      
      // search a record to print
      search_node = search(tree, phoneNum);
      if (search_node != NULL)
      {
        printNode(search_node->record);
      }
      else
        printf("This Phone Number does not exist in PhoneBook. \n");
    }
    else if (choice == DELETE)
    {
      ptr = NULL;
      phoneNum[0] = '\0';
      printf("Delete a record from the phone book\n");
      printf("Enter Phone Number [format: xxx-xxx-xxxx] >: ");
      scanf("%s", phoneNum);
      tree = delete_record(tree, phoneNum);
    }
    else if (choice == LOAD)
    {
      printf("Loading --- \n");
      printf("Enter the data file to load > ");
      scanf("%s", fileName);
      
      fp_load = fopen(fileName, "r");
      if (fp_load) {
        while (fscanf(fp_load, "%s %s %s", firstName, lastName, phoneNum) == 3) { 
          //-- add phone book record to the BST
          ptr = AddPhoneBook(firstName, lastName, phoneNum);
          tree = insert(tree, ptr);
        }
        fclose(fp);
        printf("\n");             
      }
    }
    else if (choice == SAVE)
    {
      // Save the phonebook to file
      printf("Save to File --- \n");
      if (tree != NULL) {
        fp = fopen("ProjectFile.txt", "w+");
        if (fp) {
          clear_list();
          sort(tree, 1);
          tmp = sortedList;
          while (tmp != NULL) {
            fprintf(fp, "%s %s %s\n", 
               tmp->record->firstName, 
                  tmp->record->lastName, 
                   tmp->record->phoneNum);
            tmp = tmp->next;
          }
          fclose(fp);
          printf("\n   The Phone Book has been saved to \"ProjectFile.txt\"\n");
        }
      } 
      else {
        printf("\n   The Phone Book is EMPTY and was not saved to \"ProjectFile.txt\"\n");
      }
    }
    else if (choice == DISPLAYALL)
    {
      // Display all Phone Book Records in ascending order
      if (tree) {
        printf("Printing all records in ascending order --- \n");
        printf("  Sort on the following fields:\n");
        printf("  type 1 to sort on Phone Numbers\n");
        printf("  type 2 to sort on First Name\n");
        printf("  type 3 to sort on Last Name\n\n");
      
        do {
          printf("  Please Enter [1, 2 or 3] to sort > ");
          scanf("%d", &choice);
        } while (choice < 1 || choice > 3);
        
        clear_list();
        sort(tree, choice);
        printList(choice);
      }
      else
        printf("\nThe Phone Book is EMPTY\n");
    } 
    printf("\n");
  }
  printf("\n");
  
  return 0;
}
