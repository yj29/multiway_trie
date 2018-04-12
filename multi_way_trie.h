//
// Created by Yash on 2/24/18.
//

#ifndef THREEWAY_TRIE_BINARY_TRIE_H
#define THREEWAY_TRIE_BINARY_TRIE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define numberOfChildren (2)
#define numberOfBits (1)

struct node_mt {
    struct node_mt *children[numberOfChildren];
    bool isBroken;
    bool isleaf;
    int brokenPrefix;
    bool isBrokenLeaf;
};

struct node_mt *insert_mt(char *a, struct node_mt *root);

void insert_route_in_multi_trie(__uint64_t *key, int p);

int lookup_mt(char *a, struct node_mt *root);

int lookup_in_multi_trie(__uint64_t *key);

struct node_mt *create_node_mt();

int converToDecimal(char *temp);

struct node_mt *fineTunePrefix(struct node_mt *prev, char *temp, int bits);


#endif //THREEWAY_TRIE_BINARY_TRIE_H