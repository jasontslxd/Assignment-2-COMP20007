/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Shu Lok Tsang
 */

#ifndef TEXT_ANALYSIS_H
#define TEXT_ANALYSIS_H

#define MAX_CHILDREN 27
#define MAX_LENGTH 99
#define LEAF_NODE 0

// Struct for a trie
typedef struct TrieNode Node;

struct TrieNode{
  struct TrieNode* children[MAX_CHILDREN];
  char value;
  int frequency;
};

// Build a character level trie for a given set of words.
//
// The input to your program is an integer N followed by N lines containing
// words of length < 100 characters, containing only lowercase letters.
//
// Your program should built a character level trie where each node indicates
// a single character. Branches should be ordered in alphabetic order.
//
// Your program must output the pre-order traversal of the characters in
// the trie, on a single line.
void problem_2_a();

// Using the trie constructed in Part (a) this program should output all
// prefixes of length K, in alphabetic order along with their frequencies
// with their frequencies. The input will be:
//   n k
//   str_0
//   ...
//   str_(n-1)
// The output format should be as follows:
//   an 3
//   az 1
//   ba 12
//   ...
//   ye 1
void problem_2_b();

// Again using the trie data structure you implemented for Part (a) you will
// provide a list (up to 5) of the most probable word completions for a given
// word stub.
//
// For example if the word stub is "al" your program may output:
//   0.50 algorithm
//   0.25 algebra
//   0.13 alright
//   0.06 albert
//   0.03 albania
//
// The probabilities should be formatted to exactly 2 decimal places and
// should be computed according to the following formula, for a word W with the
// prefix S:
//   Pr(word = W | stub = S) = Freq(word = W) / Freq(stub = S)
//
// The input to your program will be the following:
//   n
//   stub
//   str_0
//   ...
//   str_(n-1)
// That is, there are n + 1 strings in total, with the first being the word
// stub.
//
// If there are two strings with the same probability ties should be broken
// alphabetically (with "a" coming before "aa").
void problem_2_c();

// Reads input from stdin for 2a and 2b
char** read_input(int* n, int* k);

// Reads input from stdin for 2c
char** read_c_input(int* n);

// Creates a new node in a trie
Node* new_node();

// Inserts a word into a trie
void insert(Node* root, char* word);

// Converts a character to an int corresponding to its position in the children array of the trie
int char_to_int(char c);

// Preorder traversal of the trie
void preorder_traverse(Node* node);

// Traverse k levels of the trie and prints the prefix of length k
void traverse_height(Node* root, int k, int level, char* prefix);

// Prints the top 5 most probable words and their frequencies given a stub and a trie
void print_possible_words(Node* root, char* stub, int n);

// Gets the words that can be formed from the stub and stores it. This gets the children in the required order
// as we are preorder traversing the children
void get_children(Node* root, char* stub, int stub_freq, char** array, int* index, double* freq_array);

// Frees a string array
void free_string_array(char** array, int n);

// Frees a trie
void free_trie(Node* root);

#endif
