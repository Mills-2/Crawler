/* File: indexPage.c */
/* Author: Jacob and Vinamra */
/* Date: March 19th, 2019 */

/* This program indexes a web page, printing out the counts of words on that page */
#ifndef _INDEXPAGE_
#define _INDEXPAGE_

#include "crawler.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define ALPHABET_SIZE 26

/*create trie structure*/
struct trie
{
  struct trie *children[ALPHABET_SIZE]; //this is a pointer to a pointer
  int wordCount;  //this keeps a count for the word
};

struct trie* indexPage(const char* url, int *numTerms);
int addWordOccurrence(char* word, const int wordLength, struct trie * root);
void printTrieContents(struct trie * root, char * buffer, int depth);
int freeTrieMemory(struct trie * root);
int getText(const char* srcAddr, char* buffer, const int bufSize);
struct trie* newNode();

#endif