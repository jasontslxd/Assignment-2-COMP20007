/* * * * * * *
 * Text Analysis module for Assignment 2.
 *
 * created for COMP20007 Design of Algorithms 2020
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Shu Lok Tsang
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "text_analysis.h"

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
void problem_2_a() {
  int n = 0, k = 0;
  char** word_array = read_input(&n, &k);
  Node* root = new_node();
  root->value = '^';

  for(int i = 0; i < n; i++){
    insert(root, word_array[i]);
  }

  preorder_traverse(root);

  free_string_array(word_array, n);
  free_trie(root);
}

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
void problem_2_b() {
  int n = 0, k = 0, i;
  char** word_array = read_input(&n, &k);
  Node* root = new_node();
  root->value = '^';

  for (i = 0; i < n; i++){
    insert(root, word_array[i]);
  }
  
  char* prefix = malloc(k * sizeof(char));
  traverse_height(root, k, 0, prefix);

  free(prefix);
  free_string_array(word_array, n);
  free_trie(root);
}

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
void problem_2_c() {
  int n = 0, i;
  char** word_array = read_c_input(&n);
  Node* root = new_node();
  root->value = '^';

  for (i = 1; i <= n; i++){
    insert(root, word_array[i]);
  }

  print_possible_words(root, word_array[0], n);

  free_string_array(word_array, n+1);
  free_trie(root);
}

// Reads input from stdin for 2a and 2b
char** read_input(int* n, int* k){
  int i;

  scanf("%d %d", n, k);
  
  char** input = malloc(*n * sizeof(char*));
  assert(input);

  for (i = 0; i < *n; i++){
    input[i] = malloc(MAX_LENGTH * sizeof(char));
    scanf("%s", input[i]);
  }

  return input;
}

// Reads input from stdin for 2c
char** read_c_input(int* n){
  int i;

  scanf("%d", n);
  char** input = malloc((*n + 1) * sizeof(char*));
  assert(input);

  for (i = 0; i < *n + 1; i++){
    input[i] = malloc(MAX_LENGTH * sizeof(char));
    scanf("%s", input[i]);
  }

  return input;
}

// Creates a new node in a trie
Node* new_node(){
  Node* node = malloc(sizeof(Node));
  assert(node);

  int i;
  node->value = '\0';
  node->frequency = 0;
  for (i = 0; i < MAX_CHILDREN; i++){
    node->children[i] = NULL;
  }
  return node;
}

// Inserts a word into a trie
void insert(Node* root, char* word){
  int word_len = strlen(word), i, char_index;
  root->frequency++;
  Node* curr_node = root;

  for (i = 0; i < word_len; i++){
    char_index = char_to_int(word[i]);
    if (curr_node->children[char_index] == NULL){
      curr_node->children[char_index] = new_node();
    }
    curr_node->children[char_index]->value = word[i];
    curr_node->children[char_index]->frequency++;
    curr_node = curr_node->children[char_index];
  }

  // Mark end of word
  if (!curr_node->children[LEAF_NODE]){
    curr_node->children[LEAF_NODE] = new_node();
  }
  curr_node->children[LEAF_NODE]->value = '$';
  curr_node->children[LEAF_NODE]->frequency++;
}

// Converts a character to an int corresponding to its position in the children array of the trie
int char_to_int(char c){
  if (c == '$'){
    return 0;
  }
  if (c >= 'a' && c <= 'z'){
    return (int) c - 'a' + 1;
  }
  else{
    fprintf(stderr, "error: encountered unexpected character\n");
    exit(EXIT_FAILURE);
  }
}

// Preorder traversal of the trie
void preorder_traverse(Node* node){
  int i;
  printf("%c\n", node->value);
  for (i = 0; i < MAX_CHILDREN; i++){
    if(node->children[i] != NULL){
      preorder_traverse(node->children[i]);
    }
  }
}

// Traverse k levels of the trie and prints the prefix of length k
void traverse_height(Node* node, int k, int level, char* prefix){
  if (level == k){
    prefix[level - 1] = node->value;
    printf("%s %d\n", prefix, node->frequency);
  }

  int i;
  for (i = 1; i < MAX_CHILDREN; i++){
    if(node->children[i] != NULL){
      if (level != 0 && level <= k){
        prefix[level - 1] = node->value;
      }
      traverse_height(node->children[i], k, level + 1, prefix);
    }
  }
}

// Prints the top 5 most probable words and their frequencies given a stub and a trie
void print_possible_words(Node* root, char* stub, int n){
  int prefix_exist = 1, i, char_index;

  // Check if stub exists
  for (i = 0; i < strlen(stub); i++){
    char_index = char_to_int(stub[i]);
    if (root->children[char_index] == NULL){
      prefix_exist = 0;
    }
    else{
      root = root->children[char_index];
    }
  }

  int stub_freq = root->frequency, index = 0;
  char** possible_words = malloc(n * sizeof(char*));
  double* frequencies = malloc(n * sizeof(double));
  for (i = 0; i < n; i++){
    possible_words[i] = malloc(MAX_LENGTH * sizeof(char));
  }

  // Stub exists, get all children of the stub and store into array of strings and frequencies
  if (prefix_exist){
    get_children(root, stub, stub_freq, possible_words, &index, frequencies);
  }

  // Print 5 highest probable strings in the array of strings and frequencies
  int max_index = 0, j;
  double max_freq = 0;
  for (j = 0; j < 5; j++){
    for (i = 0; i < n; i++){
      if (frequencies[i] > max_freq){
        max_index = i;
        max_freq = frequencies[i];
      }
    }
    if (max_freq != 0){
      printf("%.2f %s\n", max_freq, possible_words[max_index]);
      strcpy(possible_words[max_index], "");
      frequencies[max_index] = 0;
      max_freq = 0;
      max_index = 0;
    }
    else{
      break;
    }
  }

  free(frequencies);
  free_string_array(possible_words, n);
}

// Gets the words that can be formed from the stub and stores it. This gets the children in the required order
// as we are preorder traversing the children
void get_children(Node* root, char* stub, int stub_freq, char** array, int* index, double* freq_array){
  // If current prefix is a word 
  if (root->children[LEAF_NODE] != NULL){
    strcpy(array[*index], stub);
    freq_array[*index] = (double) root->children[LEAF_NODE]->frequency / stub_freq;
    *index += 1;
  }

  // Get all children of the current prefix
  int i, stub_len = strlen(stub);
  for (i = 1; i < MAX_CHILDREN; i++){
    if (root->children[i] != NULL){
      stub[stub_len] = root->children[i]->value;
      get_children(root->children[i], stub, stub_freq, array, index, freq_array);
      stub[stub_len] = '\0';
    }
  }
}

// Frees a string array
void free_string_array(char** array, int n){
  int i;
  for (i = 0; i < n; i++){
    free(array[i]);
  }
  free(array);
}

// Frees a trie
void free_trie(Node* root){
  int i;
  for(i = 0; i < MAX_CHILDREN; i++){
    if(root->children[i] != NULL){
      free_trie(root->children[i]);
    }
  }
  free(root);
}