/* ========================================================================== */
/* File: hashtable.c
 *
 * Author: Yachen Sun
 * Hashtable implementation
 */
/* ========================================================================== */

// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <string.h>                          // strlen

// ---------------- Local includes  e.g., "file.h"
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // malloc functionality

// ---------------- Constant definitions

// ---------------- Macro definitions

// Print error to fp and return 1 if out of memory
#define MALLOC_CHECK_hash(s)  do {                                        \
        if((s) == NULL)   {                                             \
            printf("Error: [%s:%d] out of memory.\n", __FILE__, __LINE__);  \
            return(1);                                               \
        }                                                               \
    } while(0)

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod)
{
    size_t len = strlen(str);
    unsigned long hash, i;

    for(hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

/* 
 * HashTableInit - initialize a hash table
 *      memory allocation and pointer initializations
 * @**htable: reference pointer to pointer *htable
 * returns 0 if successful, returns 1 otherwise
 */
int HashTableInit(HashTable **htable) {
    if (htable == NULL)
        return(1);
    *htable = NEW(HashTable);
    MALLOC_CHECK_hash(*htable);
    for (int i = 0; i < MAX_HASH_SLOT; i ++) {
        (*htable)->table[i] = NULL;
    }
    (*htable)->count = 0;
    return(0);
}

/* 
 * HashTableNodeInit - initialize a hash table node
 *      memory allocation and pointer initializations
 * @**htable: reference pointer to pointer *htable
 * @*str: identifier string for the node
 * @*moreinfo: first DocumentNode for the word
 *             can be already allocated or NULL
 * returns 0 if successful, returns 1 otherwise
 */
int HashTableNodeInit(HashTableNode **node, char *str, DocumentNode *moreinfo) {
    if (node == NULL || str == NULL)
        return(1);
    *node = NEW(HashTableNode);
    MALLOC_CHECK_hash(*node);
    (*node)->str = NULL;
    (*node)->next = NULL;
    (*node)->count = 1;
    (*node)->count_x = 1;
    (*node)->moreinfo = moreinfo;
    int len = strlen(str);
    (*node)->str = malloc((len + 1) * sizeof(char));
    strncpy((*node)->str, str, len);
    (*node)->str[len] = '\0';
    return(0);
}

/* 
 * AddToHashTable - Add a string to the hash table
 *      *str will be appended to the HashTable slot 
 *      corresponding to its JenkinsHash result
 * @*htable: pointer to htable
 * @*str: identifier string for the node
 * @*moreinfo: generic pointer to the additional information,
 *             can be already allocated or NULL
 * returns 0 if successful, 1 otherwise 
 */
int AddToHashTable(HashTable *htable, char *str, DocumentNode *moreinfo) {
    if (htable == NULL || str == NULL)
        return(1);
    int slot = (int) JenkinsHash(str, MAX_HASH_SLOT);
    HashTableNode *newNode;
    if (HashTableNodeInit(&newNode, str, moreinfo)) {
        return(1);
    }
    if (htable->table[slot] == NULL) {
        htable->table[slot] = newNode;
    } else {
        HashTableNode *tail;
        tail = htable->table[slot];
        while (tail->next != NULL)
            tail = tail->next;
        tail->next = newNode;
    }
    htable->count++;
    return(0);
}

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
HashTableNode *InHashTable (HashTable *htable, char *str) {
    int slot = (int) JenkinsHash(str, MAX_HASH_SLOT);
    if (htable->table[slot] == NULL) {
        return(NULL);
    } else {
        HashTableNode *current;
        current = htable->table[slot];
        if (strcmp(str, current->str) == 0)
            return(current);
        while (current->next != NULL) {
            current = current->next;
            if (strcmp(str, current->str) == 0)
                return(current);
        }
        return(NULL);
    }
}

/* 
 * HashTableFree - free a dynamically allocated hash table
 * @**htable: reference pointer to pointer *htable
 * including the HashTableNodes and DocumentNodes in the hash table
 * results in *htable pointing to NULL
 * returns 0 if successful
 */
int HashTableFree(HashTable **htable) {
    if (*htable == NULL)
        return(0);
    for (int i = 0; i < MAX_HASH_SLOT; i++) {
        HashTableNode *current = (*htable)->table[i];
        while (current != NULL) {
            HashTableNode *tmp = current->next;
            if (current->str != NULL)
                free(current->str);
            DocumentNode *tmpdoc = current->moreinfo;
            while (tmpdoc != NULL) {
                DocumentNode *tmpdocx;
                tmpdocx = tmpdoc->next;
                if (tmpdoc->pos)
                    free(tmpdoc->pos);
                free(tmpdoc);
                tmpdoc = tmpdocx;
            }
            free(current);
            current = tmp;
        }
    }
    free(*htable);
    *htable = NULL;
    return(0);
}


/*
 * PrintHashTable - for debugging purposes only
 * @*htable: pointer to *htable
 * Prints the hash table nodes that are not null
 * returns 0 if successful
 */
int PrintHashTable(HashTable *htable) {
    if (htable == NULL)
        return(1);
    printf("HashTable content:\n");
    for (int i = 0; i < MAX_HASH_SLOT; i++) {
        HashTableNode *current = htable->table[i];
        if (current != NULL) {
            printf("Slot: %d \n", i);
            while (current != NULL) {
                HashTableNode *tmp = current->next;
                printf("%s\n", current->str);
                current = tmp;
            }
        }
    }
    return(0);
}
