#include "crawler.h"

#define MAX_ADDR_LENGTH 1000

/*
 * returns 1 if the list starting at pNode contains the address addr,
 *    and returns 0 otherwise
 */
int contains(const struct listNode *pNode, const char *addr){

 if(pNode == NULL){ //if pNode is NULL we are at the end of the list
    return 0;
 }

 int comparison = strncmp(pNode->addr, addr, MAX_ADDR_LENGTH); //copies the int value of string compare

 if(!comparison){ //string compare returns 0 if there are no matches
   return 1;

 }

 //recursive call to contains to evaluate the full listNode
 return contains(pNode->next, addr);
}


/*
 * inserts the address addr as a new listNode at the end of
 *    the list
 */
void insertBack(struct listNode *pNode, const char *addr, struct trie *root, int numTerms){

 if(pNode->next == NULL){ //if the next pNode is null then the current pNode is the end of the list

   struct listNode* addedNode = malloc(sizeof(struct listNode)); //create addedNode with allocated memory to be placed in heap

   if(addedNode == NULL){ //if addedNode is null then memory could not be allocated
     printf("memory could not be allocated");
     return;
   }

   addedNode->next = NULL; //set the new nodes next to null to signal it is the end of the list
   strncpy(addedNode->addr, addr, MAX_ADDR_LENGTH); //copy address to addedNode's address
   pNode->next = addedNode; //link the new node to the end of the list
   addedNode->rootNode = root;
   addedNode->numTerms = numTerms;

 } else {
    insertBack(pNode->next, addr, root, numTerms); //recursive call to insertBack to find the end of the list
 }
}


/*
 * prints the addresses from pNode to the end of the list,
 *   one on each line
 * precondition: stops if pNode is NULL
 */

void printAddresses(const struct listNode *pNode)
{
    if(pNode == NULL)
    {
        return;
    }

    char *buffer = malloc(sizeof(char)*1000000); //malloc to char size of 1000000 to ensure enough memory to


    printf("%s\nNumber of Terms: %d\n", pNode->addr, pNode->numTerms);
    printTrieContents(pNode->rootNode, buffer, 0);//ability to print trie contents
    free(buffer);
    printAddresses(pNode->next);
}

/*
 * frees the memory associated with this node and all subsequent nodes
 */
void destroyList(struct listNode *pNode){
 //TODO: complete this
 if(pNode->next == NULL){ //don't start deleting until the last pNode the last pNode->nest is NULL
    freeTrieMemory(pNode->rootNode);
    free(pNode);//free heap memory
 } else {
    destroyList(pNode->next);//recursive call to destroyList to find the end of the list
    freeTrieMemory(pNode->rootNode);
    free(pNode); //memory is free'd after all the nodes further down the list are removed first
 }
}



int getLink(const char* srcAddr, char* link, const int maxLinkLength){
  const int bufSize = 1000;
  char buffer[bufSize];

  int numLinks = 0;

  FILE *pipe;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python getLinks.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
     buffer);
    return 0;
  }

  fscanf(pipe, "%d\n", &numLinks);

  if(numLinks > 0){
    int linkNum;
    double r = (double)rand() / ((double)RAND_MAX + 1.0);

    for(linkNum=0; linkNum<numLinks; linkNum++){
      fgets(buffer, bufSize, pipe);

      if(r < (linkNum + 1.0) / numLinks){
		break;
      }
    }

    /* copy the address from buffer to link */
    strncpy(link, buffer, maxLinkLength);
    link[maxLinkLength-1] = '\0';

    /* get rid of the newline */
    {
      char* pNewline = strchr(link, '\n');
      if(pNewline != NULL){
		*pNewline = '\0';
      }
    }
  }

  pclose(pipe);

  if(numLinks > 0){
    return 1;
  }
  else{
    return 0;
  }
}
