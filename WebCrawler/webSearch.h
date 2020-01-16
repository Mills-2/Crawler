#ifndef _WEBSEARCH_
#define _WEBSEARCH_

#include "indexPage.h"
#include "crawler.h"
#include <math.h>
#define MAX_ADDR_LENGTH 1000

int userHasInput(char* input);
void createWordlist(char* input, char* wordList[1000]);
struct listNode* calculateIDF(char* wordList[1000], struct listNode* pnode);
int findOccurence(struct trie* root, char* buffer);
double IDF(struct listNode* node, char* buffer);
double TF(struct listNode* node, char* buffer);
struct listNode* sortList(struct listNode* list);
void printResults(struct listNode* pnode);

#endif