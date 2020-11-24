/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Shu Lok Tsang
 */

#ifndef HASH_H
#define HASH_H

#define MAX_STRING_LENGTH 256
#define MAX_BINARY_LENGTH 6

typedef struct hash_t HashTable;

struct hash_t{
  char** items;
  int step_size;
  int max_size;
};

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a();

// Implements a solution to Problem 1 (b), which reads in from stdin:
//   N M K
//   str_1
//   str_2
//   ...
//   str_N
// Each string is inputed (in the given order) into a hash table with size
// M. The collision resolution strategy must be linear probing with step
// size K. If an element cannot be inserted then the table size should be
// doubled and all elements should be re-hashed (in index order) before
// the element is re-inserted.
//
// This function must output the state of the hash table after all insertions
// are performed, in the following format
//   0: str_k
//   1:
//   2: str_l
//   3: str_p
//   4:
//   ...
//   (M-2): str_q
//   (M-1):
void problem_1_b();

// Reads input for question 1a from stdin
char** read_a_input(int* n, int* m);

// Converts a word into its binary representation
void word_to_binary(char* input, char* output);

// Converts a char into its binary representation
void char_to_binary(char input, char* output);

// Calculates the value of a binary string using horners rule
int horner(char* binary, int m);

// Read input for question 1b from stdin
char** read_b_input(int* n, int* m, int* k);

// Creates a new hashtable
HashTable* new_hashtable(int m, int k);

// Frees a hashtable
void free_hashtable(HashTable* table);

// Inserts a string into the hashtable
void insert_hashtable(HashTable* table, int hash_value, char* string);

// Does the linear probing to determine whether rehashing is needed, 1 is needed 0 is not needed
int probe(char** table, int hash_value, int step_size, int max_size, char* string);

// Inserts a string into a char array with probing
void insert_char_array(char** array, int hash_value, int step_size, int max_size, char* string);

// Doubles the size of the current array in the hash table
void rehash(HashTable* table);

#endif
