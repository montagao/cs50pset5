/***************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "dictionary.h"
#define HASH_SIZE 190313 

// implementation of the node struct used for hash table and linked list

typedef struct node
{
    char word[LENGTH+1];
    struct node* next;
} node;

// creating simple hash table where each bin represents the first letter
node* hashtable[HASH_SIZE];

// total words in dictionary
int words = 0;
/**
 * Elaborate hash function should
 * go here
*/
int hash(const char* str)
{
    // gets a string
    // returns an index deterministically
    unsigned long hashlong = 5381;
    int c;
    while ((c = *str++))
        hashlong = ((hashlong << 5) + hashlong) + c; // hash * 33 + c 
    return (hashlong % HASH_SIZE) ;
}
/**
 * Returns true if word is in dictionary else false.
* */
bool check( const char* someWord )
{
    char cword[LENGTH+1];
    memcpy( cword, someWord, strlen(someWord)+1);
    for (int i = 0; i < strlen(cword); i++)
    {
        cword[i] = tolower(cword[i]);
    }
    if ( hashtable[hash(cword)] == NULL)
    {
        return false;
    }
    node* cursor = hashtable[hash(cword)];
    while (cursor != NULL)
    {
        if( (strcmp(cursor->word,cword)) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
// Should try to implement this two ways, one with a hash table , and one with a trie
bool load(const char* dictionary)
{
    FILE* inptr = fopen(dictionary, "r");
    if (!inptr)
    {
        printf("Could not open the dictionary file!\n");
        return 1;
    }
   
    while ( !feof(inptr))
    {
        // assume all dictionary words are lowercase by default
        node * newNode = calloc(1,sizeof(node));
        fscanf(inptr, "%s", newNode->word);
        int hashIndex = hash(newNode->word); 
        
        // checks to see if bin is currently occupied
        if ( hashtable[hashIndex] == NULL) 
        {
            // if empty, make the bucket point immediately to the
            // newnode we just created
            hashtable[hashIndex] = newNode;
            
            // point the newNode next's pointer to NULL
            newNode->next = NULL;
        }
        else // if the bin is already occupied
        {
            // add newnode to beginning of the bin,
            // by pointing it's next pointer
            // to the old head and setting the array to 
            // point to the newnode
            newNode->next = hashtable[hashIndex];
            hashtable[hashIndex] = newNode;
        }
        words++;
    }
    /*
    // prints out contents of hashtable just for 
    // deubgging/optimization purposes
    for ( int i = 0 ; i < HASH_SIZE;i++)
    {
        printf("Now in hashindex : %i\n", i);
        if (hashtable[i] != NULL)
        {
            node* cursor = hashtable[i];
            while (cursor != NULL)
            {
                printf("Found node! Word = %s\n", cursor->word);
                cursor = cursor->next;
            }
        } 
    }
    
    */
    fclose(inptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
**/
bool unload(void)
{
    node* cursor;
    for ( int i = 0; i < HASH_SIZE; i++)   
    {
        if (hashtable[i] != NULL)
        {
            cursor = hashtable[i];
            while (cursor != NULL)
            {
                node* temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    }
    return true;
}

/*
int main(void)
{
    const char* dictionary = "/home/jharvard/Dropbox/pset5/dictionaries/large";
    load(dictionary);
    char* testString = "a";
    if (check(testString))
    {
        printf("Found word \"%s\" \n", testString);
    }
    return 0;

}
*/
