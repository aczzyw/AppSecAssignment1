#include "dictionary.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int check_words(FILE *fp, hashmap_t hashtable[], char *misspelled[]) {
    int total = 0;
    char word[LENGTH], lower_word[LENGTH];
    char c;
    int idx;

    while ((c = fgetc(fp)) != EOF) {
        if (!ispunct(c) && !isspace(c) && !isdigit(c)) {
            idx = 0;
            for (int i = 0; i < LENGTH; ++i) {
                word[i] = '\0';
                lower_word[i] = '\0';
            }

            while (!feof(fp) && !isspace(c)) {
                if (idx < LENGTH) {
                    word[idx++] = c;
                    c = fgetc(fp);
                } else {
                    while(!feof(fp) && !isspace(c)) {
                        c = fgetc(fp);
                        ++idx;
                    }
                    break;
                }
            }

            if (idx > LENGTH) {
                misspelled[total] = (char*) malloc(sizeof(char) * (LENGTH));
                strcpy(misspelled[total++], word);
            } else {
                while ((idx > 1) && (ispunct(word[idx - 1]))) {
                    word[--idx] = '\0';
                }

                for (int i = 0; i < idx; ++i) {
                    lower_word[i] = tolower(word[i]);
                }

                if (!check_word(word, hashtable) && !check_word(lower_word, hashtable)) {
                    misspelled[total] = (char*) malloc(sizeof(char) * (LENGTH));
                    strcpy(misspelled[total++], word);
                }
            }
        }
    }
    return total;
}

bool check_word(const char *word, hashmap_t hashtable[]) {
    int hash_code = hash_function(word);
    hashmap_t temp_node = hashtable[hash_code];
    while (temp_node != NULL) {
        if (strcmp(word, temp_node->word) == 0) {
            return true;
        }
        temp_node = temp_node->next;
    }

    return false;
}

bool load_dictionary(const char *dictionary_file, hashmap_t hashtable[]) {
    if (dictionary_file == NULL || hashtable == NULL) {
        return false;
    }

    for (int i = 0; i < HASH_SIZE; ++i) {
        hashtable[i] = NULL;
    }

    FILE* fp = fopen(dictionary_file, "r");
    if (fp == NULL) {
        return false;
    }

	char line[LENGTH];
	while (fgets(line, LENGTH, fp) != NULL) {
		char *word = strtok(line,"\n");
        hashmap_t newnode = (hashmap_t) malloc(sizeof(node));
        
        //printf("%s\n", word);
        strcpy(newnode->word, word);

        int hash_code = hash_function(word);
        newnode->next = hashtable[hash_code];
        hashtable[hash_code] = newnode;
	}	

	fclose(fp);
	return true;
}
