/* ========================================================================== */
/* File: hashtable.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Indexer
 *
 * This file contains the definitions for a hashtable of strs.
 *
 */
/* ========================================================================== */
#ifndef HASHTABLE_H
#define HASHTABLE_H

// ---------------- Prerequisites e.g., Requires "math.h"

// ---------------- Constants
#define MAX_HASH_SLOT 10000                  // number of "buckets"

// ---------------- Structures/Types
typedef struct DocumentNode {
  struct DocumentNode *next;         // pointer to the next member of the list
  char *pos;                        // document identifier
  int freq;                          // number of occurrences of the word
} DocumentNode;

typedef struct HashTableNode {
    char *str;                               // str previously seen
    struct HashTableNode *next;              // pointer to next node
    int count;								 // counter for how many instances of 
    										 // *moreinfo are connected to the current node
    int count_x;
    DocumentNode *moreinfo;					 // pointer to first doc node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
    int count;
} HashTable;

// ---------------- Public Variables

// ---------------- Prototypes/Macros

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

/* 
 * HashTableInit - initialize a hash table
 * 		memory allocation and pointer initializations
 * @**htable: reference pointer to pointer *htable
 * returns 0 if successful, returns 1 otherwise
 */
int HashTableInit(HashTable **htable);

/* 
 * HashTableNodeInit - initialize a hash table node
 * 		memory allocation and pointer initializations
 * @**htable: reference pointer to pointer *htable
 * @*str: identifier string for the node
 * @*moreinfo: first DocumentNode for the word
 *			   can be already allocated or NULL
 * returns 0 if successful, returns 1 otherwise
 */
int HashTableNodeInit(HashTableNode **node, char *str, DocumentNode *moreinfo);

/* 
 * AddToHashTable - Add a string to the hash table
 * 		*str will be appended to the HashTable slot 
 * 		corresponding to its JenkinsHash result
 * @*htable: pointer to htable
 * @*str: identifier string for the node
 * @*moreinfo: generic pointer to the additional information,
 *			   can be already allocated or NULL
 * returns 0 if successful, 1 otherwise 
 */
int AddToHashTable(HashTable *htable, char *str, DocumentNode *moreinfo);

/* 
 * InHashTable - Checks if a string is already in the 
 *               hash table
 * @*htable: pointer to htable, needs to be not NULL
 * @*str: identifier string to be checked, needs to be not NULL
 *      Computes the JenkensHash result of *str,
 *      then goes through all the nodes in the corresponding
 *      hash table slot to to see if *str is already hashed
 * returns NULL if not in hash table
 * returns a pointer to the corresponding HashTableNode if in hash table
 */
HashTableNode *InHashTable (HashTable *htable, char *str); 

/* 
 * HashTableFree - free a dynamically allocated hash table
 * @**htable: reference pointer to pointer *htable
 * including the nodes in the hash table
 * results in *htable pointing to NULL
 * Note: this function may not be fail safe,
 *       as it assumes that moreinfo is always dynamically allocated
 *		 and frees moreinfo along with the node
 *		 however, if moreinfo contains pointers that are dynamically allocated
 *		 those pointers need to be freed separately
 *		 or if moreinfo is not dynamically allocated, an error will occur
 * returns 0 if successful
 */
int HashTableFree(HashTable **htable);

/*
 * PrintHashTable - for debugging purposes only
 * @*htable: pointer to *htable
 * Prints the hash table nodes that are not null
 * returns 0 if successful
 */
int PrintHashTable(HashTable *htable);

#endif // HASHTABLE_H
