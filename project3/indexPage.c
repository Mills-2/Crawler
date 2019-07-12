/* File: indexPage.c */
/* Author: Jacob and Vinamra */
/* Date: March 19th, 2019 */

/* This program indexes a web page, printing out the counts of words on that page */

#include "indexPage.h"
#define ALPHABET_SIZE 26


/*helper method used for creating a newNode in the trie this method returns a new node*/
struct trie* newNode()
{
  struct trie* node = malloc(sizeof(struct trie)); //allocate memory for the node

  if(node == NULL) //check if the node was allocated memory
  {
    return NULL;
  }

  int i = 0;
  if(node){
    for (i=0; i < ALPHABET_SIZE; i++)
    {
      node->children[i] = NULL; //set new nodes children to null
    }
  }
  node->wordCount = 0; //set new Nodes word count to 0
  return node;
}

/*Index page reads the web page and breaks it into readable lower case character*/
struct trie* indexPage(const char* url, int *numTerms)
{
  struct trie * root = newNode(); //the root node

  //buffer to fill url text
  char buffer[300000];
  getText(url, buffer, 300000);
  printf("%s\n", url);
  int i = 0;
  while(i<300000 && buffer[i] != '\0') //the while loop goes through the string of the url
  {
    if(isalpha(buffer[i])) //it checks whether the string contains an alphabetic character
    {
      buffer[i] = tolower(buffer[i]);//if it is a alphabet then it is converted into lowercase
    }
    else
    {
      buffer[i] = ' '; //if it is not an alphabet it is converted into empty string
    }

    i++; //updates the counter
  }

  char *ptr;
  ptr = strtok(buffer, " !#$@^&*()+=-`~{[]}|?.>,<"); //tokenize the string and then assigns the pointer to the read only memory of the string
  while(ptr != NULL)
  {
    printf("\t%s\n", ptr);  //prints the word
    (*numTerms)++;
    addWordOccurrence(ptr, strlen(ptr), root); //passes the char pointer to the word with its length and the pointer to the structure root
    ptr = strtok(NULL, " ");
  }

  return root; //returns the root of the trie created from the web page
}

int addWordOccurrence(char* word, const int wordLength, struct trie * node)
{

  /*option1: run out of characters in the word being added
    option2: child is null but there is still characters to add to the trie
    option3: the characters child equivalent exitsts
   */

  int index = word[0]-'a'; //takes every character and indexes it from 0-26 using 'a' as a reference

  if(word[0]=='\0' || wordLength == 0) //if the end of the word has been reached
  {
    node->wordCount++; //add one to word count
    return 0;
  }

  if(node->children[index] == NULL) //if there are no children then adds a node
  {
    node->children[index] = newNode();
    addWordOccurrence((word+1), (wordLength-1), node->children[index]);
  }
  else
  {
    addWordOccurrence((word+1), (wordLength-1), node->children[index]); //advance a level of the trie if the character is found
  }
  return 0;
}

void printTrieContents(struct trie * root, char * buffer, int depth)
{

  /*
  this implementation is for the void method
  but I am gonna returning something
  pseudo code

  --check if there is a child node
    --if false then the null character is added and string is displayed

  --for loop( i < 26)
    --if(root->children[i]){
      --characters are added at the buffer depth  = buffer[depth]
      --printTrieContents(root->children[i], buffer, depth+1)
  */

    if(root->wordCount > 0)
    {
      buffer[depth] = '\0'; //end null character to create the printable string
      fprintf(stdout, "%s: %d\n", buffer, root->wordCount);
    }

    int i;
    for(i = 0; i < ALPHABET_SIZE; i++)
    {

      if(root->children[i])//looks to see if the child exists
      {
        buffer[depth] = i + 'a'; //dereference the index created in addWordOccurence by adding back 'a'
        printTrieContents(root->children[i], buffer, depth+1);//continue to the next level of the trie
      }
    }
}

/*removes the trie from the heap memory*/
int freeTrieMemory(struct trie * root)
{
  int i;
  for(i = 0; i < ALPHABET_SIZE; i++) //increments through the node starting with the left most node and removing a node last
  {
    if(root->children[i]) //checks if the child exists
    {
      freeTrieMemory(root->children[i]); //moves a level into the trie
    }
  }

  free(root); // frees the node
  return 0;


}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
     buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
