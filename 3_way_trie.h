//
// Created by Yash on 2/24/18.
//

#ifndef THREEWAY_TRIE_BINARY_TRIE_H
#define THREEWAY_TRIE_BINARY_TRIE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define numberOfChildren (32)
#define numberOfBits (5)

struct node {
    //struct node *left;

    struct node *children[numberOfChildren];
    bool isBroken;
    bool isleaf;
};

struct node *insert_mt(char *a, struct node *root);

void insert_route_in_multi_trie(__uint64_t *key, int p);

int * lookup_mt(char *a, struct node *root);

int lookup_in_multi_trie(__uint64_t *key);

struct node *create_node();


#endif //THREEWAY_TRIE_BINARY_TRIE_H