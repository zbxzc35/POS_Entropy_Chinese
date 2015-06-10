#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "update.h"
#include "hashtable.h"

/*
 * UpdateIndex - updates the data structure containing the index
 * @word: index word
 * @documentID: the identifier of the document
 * @index: HashTable for checking and storing indices
 * returns 0 if successful, 1 otherwise
 */
int UpdateIndex(char *word, char *pos, HashTable *index) {
	if (word == NULL || pos == NULL || index == NULL) {
		printf("Invalid UpdateIndex input.\n");
		return(1);
	}
	HashTableNode *result = InHashTable(index, word);
	if (result == NULL) { 
		// word not in hash table: add new node to hash table
		DocumentNode *newdoc = NULL;
		newdoc = malloc(sizeof(DocumentNode));
		newdoc->next = NULL;
		newdoc->pos = pos;
		newdoc->freq = 1;
		if (AddToHashTable(index, word, newdoc) == 0)
			return(0);
		else {
			printf("Adding word to HashTable failed: %s\n", word);
			free(pos);
			free(newdoc);
			return(1);
		}
	} else {
		result->count_x++;
		// word in hashtable already
		// if document already exists in the list for the word
		// increment the count for that document
		DocumentNode *current = result->moreinfo;
		DocumentNode *prev = NULL;
		while (current != NULL) {
			if (strcmp(current->pos, pos) == 0) {
				current->freq++;
				return(0);
			}
			prev = current;
			current = current->next;
		}
		// if document does not exist in the list for the word
		// increment the doc count for that word
		// create a new DocumentNode for the word
		// add it to the proper place in the list of DocumentNodes
		// the DocumentNodes are ordered by documentID, low to high
		result->count++;
		DocumentNode *newdoc = NULL;
		newdoc = malloc(sizeof(DocumentNode));
		newdoc->next = NULL;
		newdoc->pos = pos;
		newdoc->freq = 1;
		if (prev != NULL) {
				prev->next = newdoc;
		} else {
				result->moreinfo = newdoc;
		}
		return(0);
	}
}
