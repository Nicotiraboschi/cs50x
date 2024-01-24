// Implements a dictionary's functionality
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];
int words_loaded = 0;

unsigned int hash(const char *word);


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    char lower_word[LENGTH + 1];
    for (int i = 0; i < strlen(word) + 1; i++)
    {
        lower_word[i] = tolower(word[i]);
    }
    unsigned int index = hash(lower_word);
    node *cursor = table[index]->next;
    while (cursor != NULL && strcmp(cursor->word, lower_word) != 0)
    {
        cursor = cursor->next;
    }
    if (cursor == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int index = 0;
    for (int i = 0; i < strlen(word) && i < 9; i++)
    {
        if (word[i] >= 97 && word[i] <= 122)
        {
            index += (word[i] - 'a') * (word[i] - 'a');
            index += word[i] * i;
        }
        else
        {
            index += word[i] * i * i;
        }

        if ((i == (word[i] - 'a')) && strlen(word) > 3)
        {
            index += i * i * (word[i] - 'a');
        }
    }
    return index;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = NULL;
    file = fopen(dictionary, "r");
    char word[LENGTH + 1];
    bool load_bool = false;
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        table[i]->next = NULL;
    }
    int arr[N];
    /* for (int i = 0; i < N; i++)
    {arr[i] = 0;} */
    while (fscanf(file, "%s", word) != EOF)
    {
        unsigned int index = hash(word);
        // arr[index]++;
        words_loaded ++;
        node *new = malloc(sizeof(node));
        new->next = table[index]->next;
        strcpy(new->word, word);
        table[index]->next = new;
        load_bool = true;
    }
    fclose(file);
    if (load_bool == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words_loaded;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *cursor = table[i]->next;
            free(table[i]);
            table[i] = cursor;
        }
    }
    return true;
}
