#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define DICTIONARY "wordlist.txt"
#define TEST "test_my_case.txt"

int main(void) {
    hashmap_t hashtable[HASH_SIZE];

    load_dictionary(DICTIONARY, hashtable);

    char *mispelled[MAX_MISSPELLED];
    int cnt = 0;
    FILE *fp = fopen(TEST, "r");
    if (fp != NULL) {
        cnt = check_words(fp, hashtable, mispelled);

        printf("\nMisspelled words number = %d\n", cnt);
        for (int i = 0; i < cnt; i++) {
            printf("\nNO.%d: %s\n", i + 1, mispelled[i]);
        }
    }
    fclose(fp);

    for (int i = 0; i < HASH_SIZE; ++i) {
        hashmap_t temp_node = hashtable[i];
        hashmap_t follow = NULL;
        while (temp_node != NULL) {
            follow = temp_node->next;
            free(temp_node);
            temp_node = follow;
        }
        free(follow);
    }
    
    for (int i = 0; i < cnt; ++i) {
        free(mispelled[i]);
    }
}