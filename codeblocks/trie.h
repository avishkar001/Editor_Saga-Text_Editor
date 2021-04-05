#ifndef TRIE_H_INCLUDED
#define TRIE_H_INCLUDED

#define ALPHABET_SIZE (26)
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// trie node
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];

	// isEndOfWord is true if the node represents
	// end of a word
	bool isEndOfWord;
};

struct TrieNode *getNode(void);

void insert(struct TrieNode *root, const char *key);

bool search(struct TrieNode *root, const char *key);

void init_trie(struct TrieNode *root);
#endif // TRIE_H_INCLUDED
