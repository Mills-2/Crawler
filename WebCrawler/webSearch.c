#include "webSearch.h"

#define MAX_ADDR_LENGTH 1000
/* crawls webpages in a text file and answers quierys based on the results*/
int main(int argc, char** argv)
{
    char url[MAX_ADDR_LENGTH];
    char destURL[MAX_ADDR_LENGTH];
    char startAddr[MAX_ADDR_LENGTH];
    int hopsLimit = 0;
    int hopNum = 0;
    int n = 0;
    int res;

    long seed;

    if(argc != 4){ //ensure proper use of arguments
        fprintf(stderr, "USAGE: %s URLFile maxNumPages [rand seed]\n", argv[0]);
        return -1;
    }

    seed = atol(argv[3]);
    srand(seed); //set the seed


    int MAX_N = atoi(argv[2]); //set max number of hops
    FILE* fptr = fopen(argv[1], "r"); //open file in read only

    if(fptr == NULL) //check if file opened
    {
        printf("File could not be opened.");
        return -2;
    }

    struct trie * root; //declare trie pointer
    struct listNode* pnode = NULL; //declare trie

    int numTerms = 0;

    fprintf(stdout, "Indexing...\n");

    while(fgets(url, MAX_ADDR_LENGTH, fptr) != NULL && n < MAX_N) //gets another line if it exists and max hops has not been hit
    {
        sscanf(url, "%s %d", startAddr, &hopsLimit); //get the start address and hop limit from the retrieved line
        startAddr[strlen(startAddr)] = '\0'; //set last character of start address to null
        hopNum = 0; //set hopNum to 0

        while(1) //while(true)
        {
            numTerms = 0;
            if(!contains(pnode, startAddr)){ //check if new url is in the listNode

                if(pnode == NULL){ //check if this is the first node
                    pnode = malloc(sizeof(struct listNode)); //allocate a listNode to heap

                    if(pnode == NULL) //check if listNode was properly allocated
                    {
                        printf("Unable to allocate memory.");
                        fclose(fptr);
                        free(pnode);
                        return -4;
                    }

                    /*set first nodes initial values*/
                    pnode->next = NULL;
                    root = indexPage(startAddr, &numTerms);
                    pnode->rootNode = root;
                    pnode->numTerms = numTerms;
                    strncpy(pnode->addr, startAddr, MAX_ADDR_LENGTH); //copy start address to addedNode
                }else{
                    root = indexPage(startAddr, &numTerms); //set the trie
                    insertBack(pnode, startAddr, root, numTerms); //insert the address trie and num of terms into the listNode
                }

                n++; //increase number of jumps
            }

            hopNum++; //increase hop count

            if(hopNum <= hopsLimit && n < MAX_N) //ensure num of hops is not over its limit in number of indexed pages is not over its max
            {
                res = getLink(startAddr, destURL, MAX_ADDR_LENGTH); //copy start to dest

                if(!res){ //check if valid result
                    break;
                }
                strncpy(startAddr, destURL, MAX_ADDR_LENGTH); //copy dest address to start address
            }
            else
            {
                break; //breaks if limits are reached
            }
        }
    }

    fclose(fptr); //close file


    do{
        char input[1000]; //get input with a maximum of 1000 chars
        int i, j;

        for(i=0; i<1000; i++) //set initial values to null char
            input[i] = '\0';

        res = userHasInput(input); //solicit user input

        if(res>0){ //check if the user has valid input
            char* wordList[1000]; //create pointer with max 1000 chars

            for(i = 0; i < 500; i++){
                for(j = 0; j < 1000; j++){
                    wordList[i] = '\0'; //set maximum number of possible words and maximum number of chars to null char
                }
            }

            createWordlist(input, wordList); //create the list of words from user input
            pnode = calculateIDF(wordList, pnode); //calculate IDF scores
            printResults(pnode); //print results
        }

    } while(res); // check if user has any input


    printf("Exiting the program\n");
    destroyList(pnode); //destroy listNode and its contents include the trie
    return 0; //no errors
}
/*checks if user has input returns 0(false) if user has no input returns
  positive integer for valid input and negative for invalid input*/
int userHasInput(char *input){
    int res;
    int i = 0;

    printf("\nEnter a web query: ");
    fgets(input, 1000, stdin); //gets input from stdin

    res = strcmp(input, "\n\0"); //checks if input is only a blank line

    if(input[0] == '\0') //checks if first input val is null char
        return 0; //no input

    if(!res) //checks result of strcmp
        return 0; //no input

    for(i = 0; i<1000; i++) //the while loop goes through all char of the url
    {
        if(input[i] == '\n'){ // finds the enter char and replaces it with null char
            input[i] = '\0';
            break;
        }
    }

    for(i = 0; i < 1000 && input[i] != '\0'; i++) //loop through all characters in url
    {

        if(!isalpha(input[i]) || (!islower(input[i]))) //checks whether the string contains an alphabetic or upper-case character
        {
            if(isspace(input[i]))//ignores char if it is a space
            {
                i++;
                continue;
            }
            else
            {
                printf("Please enter a query containing only lower-case letters.\n");
                input[0] = '\0';
                return -1; //stops solicitation and returns invalid input
            }
        }
    }

    printf("Query is \"%s\".\n", input);
    return 1; //completes query valid input
}

/*creates an array of each word the user input*/
void createWordlist(char* input, char* wordList[1000]){

    int i = 0;
    int length;
    char *ptr;
    ptr = strtok(input, " "); //tokenize the string and then assigns the pointer to the read only memory of the string

    for(i = 0; ptr != NULL; i++) //checks if there is a word available
    {
        wordList[i] = ptr; //assigns word to array
        length = strlen(wordList[i]);
        wordList[i][length] = '\0'; //sets null char at end to make string
        ptr = strtok(NULL, " "); //get next available word
    }
}

/*calculates IDF scores for individual words and pages and returns head of a sorted listNode of indexed pages*/
struct listNode* calculateIDF(char* wordList[1000], struct listNode* pnode){

    int i;
    double tScore;
    double tf;
    double idf;

    struct listNode* currentNode = pnode; //set currentNode to first node

    printf("IDF scores are\n");

    for(i = 0; wordList[i] != NULL; i++) //search through each user input word and calc IDF
    {
        idf = IDF(pnode, wordList[i]);
        printf("IDF(%s): %1.7f\n", wordList[i], idf);
    }

    printf("Web pages:\n");

    while(currentNode != NULL)  //check if currentNode exists (this is the address)
    {
        i=0;
        tScore = 0; //preset the IDF score for the page to 0

        while(wordList[i] != NULL) //check if word exists
        {
            if(findOccurence(currentNode->rootNode, wordList[i])) //checks if the word is in this address
            {
                tf = TF(currentNode, wordList[i]); //calculates tf of word in this address
                idf = IDF(pnode, wordList[i]); //calculate idf of the word
                tScore += (tf*idf); //webpages total IDF score is sum of (tf*idf) of each user input word
            }

            i++; //iterate to next word
        }

        currentNode->idfScore = tScore; //set idf score of address
        currentNode = currentNode->next; //iterate to next address
    }

    pnode = sortList(pnode); //sort list based on IDF scores
    return pnode; //return head of sorted list
}

/*calculates IDF score from head node of linked list given a word*/
double IDF(struct listNode* node, char* buffer)
{
    double idf;
    int numberOfIndexedPages = 0;
    int numberOfPagesContainingBuffer = 0;

    while(node != NULL) //checks if node exists
    {
        numberOfIndexedPages++; //gets total number of indexed pages

        if(findOccurence(node->rootNode, buffer)) //if the address contains the word iterates pages containing the word
            numberOfPagesContainingBuffer++;

        node=node->next; //iterates to next address
    }

    idf = (1.0+numberOfIndexedPages)/(1.0+numberOfPagesContainingBuffer); //idf=log[(1+total_number_of_indexed_pages)/(1+number_of_pages_containing_word)]

    return (double)log(idf);
}

/*calculates Term frequence of a word in a specific page*/
double TF(struct listNode* node, char* buffer)
{
    int term;
    int wordsInPage;
    double tf;

    term = findOccurence(node->rootNode, buffer); //find number of occurences of a word in an address
    wordsInPage = node->numTerms; //gets the total number of words in address
    tf = (double)term/(double)wordsInPage; //tf=(num_of_terms_in_page)/(num_of_words_in_page)
    return tf;
}

/*finds the total number of a specific word in an addresses trie*/
int findOccurence(struct trie* root, char* buffer)
{
    if(buffer[0] == '\0') //if the char is null this is the last char
    {
      return root->wordCount;
    }

    int index = buffer[0] - 'a'; //sets the index from ref 'a'

    if(root->children[index])//looks to see if the child exists
    {
        return findOccurence(root->children[index], (buffer+1));//continue to the next level of the trie
    }
    else
    {
        return 0; //if the child does not exist the word is not in the trie
    }
}

/*sorts the list from largest IDF score to lowest*/
struct listNode *sortList(struct listNode* head){
    if(head == NULL || head->next == NULL)
        return head;//list is already sorted

    double max = head->idfScore;
    struct listNode *prev = head;
    struct listNode *move = NULL;
    struct listNode *next = head->next;

    //find maximum value in list(head->next)
    while(next != NULL) {
        if(next->idfScore > max) {
            max = next->idfScore;
            move = prev;//save previous node which will become the new head
        }
        prev = next;
        next = next->next;
    }

    if(move == NULL) {//not found in list
        head->next = sortList(head->next);
        return head;
    }

    prev = move;
    move = prev->next;//get max node
    prev->next = prev->next->next;//remove max node from link
    move->next = sortList(head);
    return move;
}

/*prints the results of the IDF scores*/
void printResults(struct listNode* pnode){

    int i;
    struct listNode* currentNode = pnode;

    for(i = 0; i < 3 ; i++) //prints only the top three
    {
        if(currentNode != NULL) //checks if node exists
        {
            if(currentNode->idfScore > 0) //only prints scores that are greater than 0
                printf("%d. %s (score: %1.4f)\n", (i+1), currentNode->addr, currentNode->idfScore);
        }
        else
        {
            break;
        }

        currentNode = currentNode->next; //iterates to next node
    }
}


