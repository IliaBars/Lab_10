#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define ALPHABET_SIZE 26

// Trie structure
struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    int word_count;
};

// Create tri node
struct Trie *createTrieNode() {
    struct Trie *node = malloc(sizeof(struct Trie));
    if (!node) return NULL;
    node->word_count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) node->children[i] = NULL;
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    if (!pTrie) {
        pTrie = createTrieNode();
    }


    int l = strlen(word);
    int indexes[l];
    for (int i = 0; i < l; i++) {
        indexes[i] = word[i] - 'a';
    }

    struct Trie * tmp = pTrie;
    for (int i = 0; i < l; i++) {
        // New node creation
        if (!tmp->children[indexes[i]]) tmp->children[indexes[i]] = createTrieNode();
        // Traversing
        tmp = tmp->children[indexes[i]];
    }
    tmp->word_count++; // Increasing word count
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    if (!pTrie) return 0;

    int l = strlen(word);
    int indexes[l];
    for (int i = 0; i < l; i++) {
        indexes[i] = word[i] - 'a';
    }

    struct Trie * temp = pTrie;
    for (int i = 0; i < l; i++) {
        if (!temp->children[indexes[i]]) return 0;
        temp = temp->children[indexes[i]];
    }
    return temp ? temp->word_count : 0;
}


// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (!pTrie) return NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    return createTrieNode();
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE* inputFile = fopen(filename, "r");
    if (!inputFile) return 0;

    int words_count = 0;
    fscanf(inputFile, "%d", &words_count);

    char word[100];
    for (int i = 0; i < words_count; i++){
        // Allocate memory
        pInWords[i] = malloc(sizeof(char) * 100);
        // Scan the word
        fscanf(inputFile, "%s", word);
        // Add to the list
        strcpy(pInWords[i], word);
    }

    // Close file
    fclose(inputFile);

    return words_count;
}

int main(void)
{
    char *inWords[256];

    //read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i=0;i<numWords;++i)
    {
        printf("%s\n",inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i=0;i<numWords;i++)
    {
        insert(pTrie, inWords[i]);
    }

    // parse lineby line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i=0;i<5;i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");
    return 0;
}