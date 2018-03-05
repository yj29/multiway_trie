//
// Created by Yash on 2/24/18.
//

#ifndef THREEWAY_TRIE_BINARY_TRIE_H
#define THREEWAY_TRIE_BINARY_TRIE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#define numberOfChildren (64)
#define numberOfBits (6)

struct node {
    //struct node *left;

    struct node *children[numberOfChildren];

    bool isleaf;
};

struct node *insert(char *a, struct node *root, int n_way);

void insert_route_in_trie(__uint64_t *key, int p, int n_way);

int * lookup(char *a, struct node *root, int n_way);

int lookup_in_trie(__uint64_t *key,int n_way);

struct node *create_node();


#endif //THREEWAY_TRIE_BINARY_TRIE_H