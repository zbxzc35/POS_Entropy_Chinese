#define _GNU_SOURCE			// required for "getline"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "hashtable.h"                       // hashtable functionality
#include "update.h"

char utf8_look_for_table[] = 
{
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

#define UTFLEN(x)  utf8_look_for_table[(x)]

HashTable *table = NULL;

int main(int argc, char* argv[])
{
	HashTableInit(&table);

	FILE *fp = NULL;
	FILE *outf = NULL;
	if((fp = fopen(argv[1], "r")) == NULL) {
    	printf("Error opening file: %s\n", argv[1]);
    	return(1);
    }
    if((outf = fopen(argv[2], "w")) == NULL) {
    	printf("Error opening file: %s\n", argv[2]);
    	return(1);
    }

    char *line = NULL;
    size_t nbytes = 100;
    int read;
    line = (char *) malloc (nbytes + 1);
    while ((read = getline(&line, &nbytes, fp)) != -1) {
    	char *ptr = line;
    	char *word = NULL;
    	char *pos = NULL;
    	int stage = 0; // 0: reading words; 1: reading pos; 2: blank 
		while (*ptr!=0) {
			unsigned char h = (unsigned char) *ptr;
			size_t len = (size_t) UTFLEN(h);
			char c[10];
        	memcpy(c, ptr, len);
        	c[len] = '\0'; 
			if (stage == 2 && (strcmp(c, " ") != 0) && (strcmp(c, "\t") != 0))
				stage = 0;
			switch (stage) {
				case 0:
					if (pos)
						free(pos);
					pos = NULL;
					if ((strcmp(c, "#") == 0)) {
						stage = 1;
					} else {
						if ((strcmp(c, " ") == 0) || (strcmp(c, "\t") == 0)) {
							if (word)
								free(word);
							word = NULL;
							stage = 2;
							break;
						} else {
							if (!word) {
								word = calloc(150, sizeof(char));
								strncpy(word, c, len);
							} else
								strncat(word, c, len);
						}
					}
					break;
				case 1:
					if ((strcmp(c, " ") == 0) || (strcmp(c, "\t") == 0)) {
						if (pos) {
							if (!word) {
								free(word);
								word = NULL;
							} else {
								UpdateIndex(word, pos, table);
								stage = 2;
								word = NULL;
								pos = NULL;
							}
						} else {
							if (word)
								free(word);
							word = NULL;
							stage = 2;
						}
					} else {
						if (!pos) {
								pos = calloc(60, sizeof(char));
								strncpy(pos, c, len);
							} else
								strncat(pos, c, len);
					}
					break;
				case 2:
					if (!word)
						free(word);
					word = NULL;
					if (!pos)
						free(pos);
					word = NULL;
					break;
				default:
					break;
			}
			ptr += len;
		}
    }
    free(line);

    for (int i = 0; i < MAX_HASH_SLOT; i++) {
		HashTableNode *current = table->table[i];
        while (current != NULL) {
        	if (current->count_x > 1) {
	        	double shannon = 0;
	        	fprintf(outf, "%s %d ", current->str, current->count_x);
	            DocumentNode *tmpdoc = current->moreinfo;
	            while (tmpdoc != NULL) {
	                double pi = tmpdoc->freq;
	                pi /= current->count_x;
	                printf("freq: %d, pi: %f\n", tmpdoc->freq, pi);
	                shannon -= pi * log(pi);
	                tmpdoc = tmpdoc->next;
	            }
	            fprintf(outf, "%f\n", shannon);
	        }
            current = current->next;
        }
	}



	if (outf)
    	fclose(outf);

    if (fp)
    	fclose(fp);
    HashTableFree(&table);
}