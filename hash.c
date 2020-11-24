/* * * * * * *
 * Hashing module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Shu Lok Tsang
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "hash.h"
#include "text_analysis.h"

// Implements a solution to Problem 1 (a), which reads in from stdin:
//   N M
//   str_1
//   str_2
//   ...
//   str_N
// And outputs (to stdout) the hash values of the N strings 1 per line.
void problem_1_a() {
  int m = 0, n = 0, i, hash_value;
  char** word_array = read_a_input(&n, &m);
  char** word_binary_rep = malloc(n * sizeof(char*));

  for (i = 0; i < n; i++){
    word_binary_rep[i] = malloc(MAX_STRING_LENGTH * MAX_BINARY_LENGTH * sizeof(char));
    word_to_binary(word_array[i], word_binary_rep[i]);
    hash_value = horner(word_binary_rep[i], m);
    printf("%d\n", hash_value);
  }

  free_string_array(word_array, n);
  free_string_array(word_binary_rep, n);
}

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
void problem_1_b() {
  int n = 0, m = 0, k = 0, i, hash_value = 0;
  char** word_array = read_b_input(&n, &m, &k);
  char** word_binary_rep = malloc(n * sizeof(char*));
  HashTable* table = new_hashtable(m, k);

  for (i = 0; i < n; i++){
    word_binary_rep[i] = calloc(MAX_STRING_LENGTH * MAX_BINARY_LENGTH, sizeof(char));
    word_to_binary(word_array[i], word_binary_rep[i]);
    hash_value = horner(word_binary_rep[i], table->max_size);
    insert_hashtable(table, hash_value, word_array[i]);
  }

  for (i = 0; i < table->max_size; i++){
    printf("%d: %s\n", i, table->items[i]);
  }

  free_string_array(word_array, n);
  free_string_array(word_binary_rep, n);
  free_hashtable(table);
}

// Reads input for question 1a from stdin
char** read_a_input(int* n, int* m){
  int i;

  scanf("%d %d", n, m);
  
  char** input = malloc(*n * sizeof(char*));
  assert(input);

  for (i = 0; i < *n; i++){
    input[i] = malloc(MAX_STRING_LENGTH * sizeof(char));
    scanf("%s", input[i]);
  }

  return input;
}

// Converts a word into its binary representation
void word_to_binary(char* input, char* output){
  int input_length = strlen(input), i;
  char* char_in_binary = malloc(MAX_BINARY_LENGTH * sizeof(char));
  for (i = 0; i < input_length; i++){
    char_to_binary(input[i], char_in_binary);
    strcat(output, char_in_binary);
  }

  free(char_in_binary);
}

// Converts a char into its binary representation
void char_to_binary(char input, char* output){
  // Converts char into int
  int value = 0;
  if (input >= '0' && input <= '9'){
      value = input + 4;
    }
    if (input >= 'a' && input <= 'z'){
      value = input - 'a';
    }
    if (input >= 'A' && input <= 'Z'){
      value = input - 39;
    }

  // Converts int into 6 digit binary
  int i, j = 0;
  for (i = 5; i >= 0; i--){
    if ((value - (int) pow(2,i)) >= 0){
      output[j] = '1';
      value -= pow(2,i);
    }
    else{
      output[j] = '0';
    }
    j++;
  }
}

// Calculates the value of a binary string using horners rule
int horner(char* binary, int m){
  int length = strlen(binary), x = 2, i, result = 0;

  if (binary[0] == '1'){
    result = 1;
  }
  else{
    result = 0;
  }

  for (i = 1; i < length; i++){
    if (binary[i] == '1'){
      result = (result * x + 1) % m;
    }
    else{
      result = (result * x) % m;
    }
  }

  return result;
}

// Read input for question 1b from stdin
char** read_b_input(int* n, int* m, int* k){
  int i;

  scanf("%d %d %d", n, m, k);
  
  char** input = malloc(*n * sizeof(char*));
  assert(input);

  for (i = 0; i < *n; i++){
    input[i] = malloc(MAX_STRING_LENGTH * sizeof(char));
    scanf("%s", input[i]);
  }

  return input;
}

// Creates a new hashtable
HashTable* new_hashtable(int m, int k){
  HashTable* table = malloc(sizeof(HashTable));
  assert(table);

  table->max_size = m;
  table->step_size = k;
  table->items = calloc(m, sizeof(char*));

  int i;
  for(i = 0; i < m; i++){
    table->items[i] = calloc(MAX_STRING_LENGTH, sizeof(char));
  }

  return table;
}

// Frees a hashtable
void free_hashtable(HashTable* table){
  free_string_array(table->items, table->max_size);
  free(table);
}

// Inserts a string into the hashtable
void insert_hashtable(HashTable* table, int hash_value, char* string){
  if (table->items[hash_value][0] == '\0'){
    strcpy(table->items[hash_value], string);
  }

  // Collision, try probing
  else{
    char* binary_rep = calloc(MAX_STRING_LENGTH * MAX_BINARY_LENGTH, sizeof(char));
    int need_rehash = probe(table->items, hash_value, table->step_size, table->max_size, string);

    // Probing wont do, need to rehash
    int new_hash_value = 0;
    if (need_rehash){
      rehash(table);
      word_to_binary(string, binary_rep);
      new_hash_value = horner(binary_rep, table->max_size);
      insert_hashtable(table, new_hash_value, string);
    }
    free(binary_rep);
  }
}

// Does the linear probing to determine whether rehashing is needed, 1 is needed 0 is not needed
int probe(char** table, int hash_value, int step_size, int max_size, char* string){
  int original_hash = hash_value;
  hash_value = (hash_value + step_size) % max_size;
  while (original_hash != hash_value){
    if (table[hash_value][0] == '\0'){
      strcpy(table[hash_value], string);
      return 0;
    }
    hash_value = (hash_value + step_size) % max_size;
  }
  return 1;
}

// Inserts a string into a char array with probing
void insert_char_array(char** array, int hash_value, int step_size, int max_size, char* string){
  if (array[hash_value][0] == '\0'){
    strcpy(array[hash_value], string);
  }
  else{
    probe(array, hash_value, step_size, max_size, string);
  }
}

// Doubles the size of the current array in the hash table
void rehash(HashTable* table){
  int old_size = table->max_size;
  int new_size = 2 * old_size;
  char** new_table = calloc(new_size, sizeof(char*));
  char** old_binary = calloc(old_size, sizeof(char*));

  // Prepares new tables for rehashing
  int i, new_hash_value = 0;
  for (i = 0; i < new_size; i++){
    new_table[i] = calloc(MAX_STRING_LENGTH, sizeof(char));
  }

  // Re hashes old records into new table
  for (i = 0; i < old_size; i++){
    old_binary[i] = calloc(MAX_STRING_LENGTH * MAX_BINARY_LENGTH, sizeof(char));
    if (table->items[i][0] != '\0'){
      word_to_binary(table->items[i], old_binary[i]);
      new_hash_value = horner(old_binary[i], new_size);
      insert_char_array(new_table, new_hash_value, table->step_size, new_size, table->items[i]);
    }
  }

  // Clean up
  char** tmp = table->items;
  table->items = new_table;
  table->max_size = new_size;
  free_string_array(tmp, old_size);
  free_string_array(old_binary, old_size);
}