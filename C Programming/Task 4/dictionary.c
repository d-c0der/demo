// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root;

// Declare prototypes
int chartoint_dict(char c);
int chartoint_spell(char c);
// void clear_buffer(char *buffer);
void erase(node *ptr);
// void size2(node *ptr);

// Global variable for loaded words counter
unsigned int LOADED_WORDS = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Clear buffer
    // clear_buffer(word);

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // TODO - completed

        node *ptr = root;
        for (int i = 0; word[i] != '\0'; i++)
        {
            // Find int value for children[]
            int arrayslot = chartoint_dict(word[i]);

            if (ptr->children[arrayslot] != NULL)
            {
                ptr = ptr->children[arrayslot];
            }
            else
            {
                ptr->children[arrayslot] = malloc(sizeof(node));
                if (!ptr)
                {
                    printf("error allocating space for a node\n");
                    return false;
                }

                // Shift pointer to new node
                ptr = ptr->children[arrayslot];

                // Set to zero values
                ptr->is_word = false;
                for (int j = 0; j < N; j++)
                {
                    ptr->children[j] = NULL;
                }

            }
        }
        ptr->is_word = true;
        // clear_buffer(word);

        // Update loaded words counter
        LOADED_WORDS++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //size2(root);
    return LOADED_WORDS;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *ptr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Find int value for children[]
        int arrayslot = chartoint_spell(word[i]);

        if (ptr->children[arrayslot] != NULL)
        {
            // Shift pointer to new node
            ptr = ptr->children[arrayslot];
        }
        else
        {
            return false;
        }
    }

    // Check for word termination record being in the dictionary (next node is_word is set to true).
    if (ptr->is_word)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    erase(root);
    return true;

}

// Convert dictionary letter ascii value to array slot number
int chartoint_dict(char c)
{
    if ((int) c == 39)
    {
        return 26;
    }
    else
    {
        return (int) c - 97;
    }

}

// Convert spelled letter ascii value to array slot number
int chartoint_spell(char c)
{
    int letter = tolower((int) c);
    if (letter == 39)
    {
        return 26;
    }
    else
    {
        return letter - 97;
    }

}

// Clear buffer
//void clear_buffer(char *buffer)
//{
//    for (int i = 0, n = LENGTH + 1; i < n; i++)
//  {
//        buffer[i] = '\0';
//    }
//
//}

// Erase nodes
void erase(node *ptr)
{
    for (int i = 0; i < N; i++)
    {
        if (ptr->children[i])
        {
            erase(ptr->children[i]);
        }

    }
    free(ptr);
}

// Alternate version of loaded words counter
// Returns number of words in dictionary if loaded else 0 if not yet loaded
// void size2(node *ptr)
// {
//    for (int i = 0; i < N; i++)
//    {
//        {
//            size2(ptr->children[i]);
//        }
//    }
//    if (ptr->is_word)
//    {
//        LOADED_WORDS++;
//    }
//}