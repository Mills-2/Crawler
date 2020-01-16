#ifndef _CRAWLER_
#define _CRAWLER_

#include "indexPage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ADDR_LENGTH 1000

/*
 * a node in our linked-list of web addresses
 */
struct listNode{
  char addr[MAX_ADDR_LENGTH];
  int numTerms;
  double idfScore;
  struct listNode *next;
  struct trie *rootNode;
};


int contains(const struct listNode *pNode, const char *addr);
void insertBack(struct listNode *pNode, const char *addr, struct trie *root, int numTerms);
void printAddresses(const struct listNode *pNode);
void destroyList(struct listNode *pNode);
int getLink(const char* srcAddr, char* link, const int maxLinkLength);

#endif
