/**
 * @author Yash Jain
 * m-way trie for IPv6 lookup
 */

#include <sys/socket.h>
#include <rpc/types.h>
#include "3_way_trie.h"
#include <arpa/inet.h>
#include <math.h>

struct address {
    unsigned long long a0;
    unsigned long long a1;
    unsigned long long a2;
};
struct address ad[500050];
int idx = 0;

struct node *lookup_node;
static int prefix, ct = 1, pref;
static int f_prefix;
struct node *root_bg = NULL;
int numberOfChildern = numberOfChildren;
static int countOfNodes = 0;

struct node *create_node() {
    struct node *root = (struct node *) malloc(sizeof(struct node));
    root->isleaf = false;
    for (int i = 0; i < numberOfChildern; i++) {
        root->children[i] = NULL;
    }
    return root;
}

//convert into unsigned int
static __inline__ __uint128_t in6_addr_to_uint128(struct in6_addr *in6) {
    __uint128_t a;
    int i;

    a = 0;
    for (i = 0; i < 16; i++) {
        a <<= 8;
        a |= in6->s6_addr[i];
    }

    return a;
}

unsigned long long *binary_ipv6(char *char_ip) {
    __uint128_t addr1 = 0;
    int prefixlen;
    char v6str1[256] = "";
    char v6str2[256] = "";
    unsigned long long *ip6_ret = (unsigned long long *) malloc(sizeof(unsigned long long *) * 3);
    struct in6_addr result;

    sscanf(char_ip, "%255[^'/']/%d %255s", v6str1, &prefixlen, v6str2);

    inet_pton(AF_INET6, v6str1, &result);
    addr1 = in6_addr_to_uint128(&result);

    ip6_ret[1] = addr1;
    ip6_ret[0] = addr1 >> 64;
    ip6_ret[2] = prefixlen;
    ad[idx].a0 = addr1 >> 64;
    ad[idx].a1 = addr1;
    ad[idx++].a2 = prefixlen;
    return ip6_ret;
}

/**
 * Masking method
 * @param adr
 * @param pref
 * @return
 */
__uint64_t *create_mask(uint64_t *adr, int pref) {
    uint64_t mask1 = 0xFFFFFFFFFFFFFFFF, mask2 = 0xFFFFFFFFFFFFFFFF;
    if (pref < 65) {

        if (pref == 0)
            mask1 = 0;
        else
            mask1 = mask1 << (64 - pref);

        *(adr) = *(adr) & mask1;
        *(adr + 1) = 0;
    } else { //mask 1 as it is
        //mask2
        mask2 = mask2 << (128 - pref);

        *(adr + 1) = *(adr + 1) & mask2;
    }
    return adr;
}

struct node *insert(char *a, struct node *root, int n_way) {
    struct node *nd = root;

    for (int i = 0; a[i] != '\0' && pref > 0; i = i + n_way) {
        // printf("I= %d\n",i);
        char *temp = malloc(sizeof(char) * n_way);
        int k = 0;
        while (k < n_way) {
            temp[k] = '0';
            k++;
        }
        k = n_way - 1;
        while (k >= 0 && pref > 0) {
            if (a[i + k] == '\0' || (i + k) > 128) {
                k--;
                continue;
            }
            temp[k] = a[i + k];
            k--;
            pref--;
        }
        int decimal = converToDecimal(temp, n_way);
        if (nd->children[decimal] == NULL) {
            nd->children[decimal] = create_node();
            nd = nd->children[decimal];
            countOfNodes++;
        } else {
            nd = nd->children[decimal];
        }
    }
    return nd;
}

int converToDecimal(char *temp, int n_way) {
    int p = 0;
    int i;
    int dec = 0;
    for (i = n_way - 1; i >= 0; --i) {
        dec += (temp[i] - 48) * (int) pow((double) 2, p);
        p++;
    }
    return dec;
}

void insert_route_in_trie(__uint64_t *key, int p, int n_way) {
    __uint64_t n = key[0];
    char a[65] = "";
    int i = 0;
    char rev[129] = "";
    pref = p;
    //root creation
    if (ct == 1) {
        root_bg = create_node();
        countOfNodes++;
        ct = 0;
    }

    while (n != 0) {
        int r = n % 2;
        a[i] = r + 48;
        n = n / 2;
        i++;
    }
    //PROBLEM
    if (i < 64) {
        for (int k = i; k < 64; k++)
            a[k] = 48;
    }
    for (int l = strlen(a) - 1, t = 0; l >= 0; --l, t++) {
        rev[t] = a[l];

    }
    char b[65] = "";
    int j = 0;
    char rev1[65] = "";
    __uint64_t m = key[1];
    if (pref > 0) {
        while (m != 0) {

            int k = m % 2;
            b[j] = k + 48;
            m = m / 2;
            j++;
        }
        if (j < 64) {
            for (int k = j; k < 64; k++)
                b[k] = 48;
        }
        for (int l = strlen(b) - 1, t = 0; l >= 0; --l, t++) {
            rev1[t] = b[l];

        }
        for (int i = 64; i < 129; i++) {
            rev[i] = rev1[i - 64];
        }
    }
    struct node *nd1 = insert(rev, root_bg, n_way);
    nd1->isleaf = true;
}

int *lookup(char *a, struct node *root, int n_way) {
    struct node *nd = root;
    struct node *prev = NULL;
    for (int i = 0; a[i] != '\0'; i = i + n_way) {
        char *temp = malloc(sizeof(char) * n_way);
        int k = 0;
        while (k < n_way) {
            temp[k] = '0';
            k++;
        }
        k = n_way - 1;
        while (k >= 0) {
            if (a[i + k] == '\0' || (i + k) > 128) {
                k--;
                continue;
            }
            temp[k] = a[i + k];
            k--;
        }
        int decimal = converToDecimal(temp, n_way);
        if (nd->children[decimal] == NULL) {
            if (prev != NULL) {
                int prefix_1 = fineTunePrefix(prev, temp, n_way);
                f_prefix = prefix + prefix_1;
            }
            f_prefix = prefix;
            break;
        } else {
            prefix += n_way;
            if (nd->children[decimal]->isleaf) {
                f_prefix = prefix;
            }

            nd = nd->children[decimal];
            prev = nd;
        }
    }
    lookup_node = nd;
    return f_prefix;
}

int fineTunePrefix(struct node *prev, char *temp, int n_way) {
    int out = 0;
    int i = 0;
    int j = 0;
    int max = -1;

    for (i = 0; i < n_way; i++) {
        int index = 0;
        char *t = malloc(sizeof(char) * n_way);
        for (int k = 0; k < n_way; ++k) {
            t[k] = '0';
        }

        for (j = 0; j < i + 1; j++) {
            t[j] = temp[j];
            index++;
        }
        int dec = converToDecimal(t, n_way);
        if (prev->children[dec] != NULL && prev->children[dec]->isleaf) {
            max = index > max ? index : max;
        }
    }
    out = (max != -1) ? max : out;
    return out;
}

int lookup_in_trie(__uint64_t *key, int n_way) {
    int tt;
    __uint64_t n = *(key + 0);
    char a[65] = "";
    char rev[65] = "";
    int i = 0;
    char rev1[65] = "";
    prefix = 0;
    while (n != 0) {
        int r = n % 2;
        a[i] = r + 48;
        n = n / 2;
        i++;
    }

    if (i < 64) {
        for (int k = i; k < 64; k++)
            a[k] = 48;
    }
    for (int l = strlen(a) - 1, t = 0; l >= 0; --l, t++) {
        rev[t] = a[l];
    }

    char b[65] = "";
    int j = 0;
    __uint64_t m = *(key + 1);
    while (m != 0) {
        int k = m % 2;
        b[j] = k + 48;
        m = m / 2;
        j++;
    }
    if (j < 64) {
        for (int k = j; k < 64; k++)
            b[k] = 48;
    }
    for (int l = strlen(b) - 1, t = 0; l >= 0; --l, t++) {
        rev1[t] = b[l];
    }

    for (int i = 64; i < 129; i++) {
        rev[i] = rev1[i - 64];
    }

    tt = lookup(rev, root_bg, n_way);
    /* if (rev1[0] != '\0') {
         // tt = lookup(rev1, lookup_node);
     }*/
    return tt;

}

int main() {
    bool t;
    struct node nnn;
    //sizeOfOneNode = (int) sizeof(nnn);
    printf("%d\n", sizeof(nnn));
    int n_way = numberOfBits;
    char *line = NULL;
    size_t len = 0;
    int n;
    double elap;
    struct timeval t0, t1;

    //----------------------CREATING m-way trie----------------------------
    FILE *ft1 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_input.txt", "r");
    // FILE *ft2 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_output.txt", "r");
    int c = 0;
    while (getline(&line, &len, ft1) != -1) {

        n = strlen(line) - 1;
        if (line[0] == '\n')
            break;
        line[n] = '\0';

        binary_ipv6(line);
    }
    printf("Insertion File read!\n");
    int i = 0;
    for (i = 0; i < idx; i++) {
        //printf("Inserting: %d\n", i);
        uint64_t *net_prefix = create_mask(&ad[i], ad[i].a2);
        //insertion on trie
        insert_route_in_trie(net_prefix, ad[i].a2, n_way);
    }

    //--------------------LOOKUP IN m-way trie
    FILE *ft = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_output.txt", "r");
    idx = 0;
    while (getline(&line, &len, ft) != -1) {
        n = strlen(line) - 1;
        if (line[0] == '\n')
            break;
        line[n] = '\0';
        binary_ipv6(line);
    }
    printf("Lookup File read!\n");
    gettimeofday(&t0, NULL);
    for (i = 0; i < idx; i = i + 1) {
        // printf("Lookup: %d\n",i);
        int pref = lookup_in_trie(&ad[i], n_way);
        //if (i < 1005 && i >= 805)
        // printf("prefix=%d\n", pref);
    }

    gettimeofday(&t1, NULL);
    elap = (t1.tv_sec * 1e6 + t1.tv_usec - t0.tv_sec * 1e6 - t0.tv_usec) / (1e6);
    printf("\ntime elapse in second=%.10f", elap);
    printf("\nNumber of nodes: %d\n", countOfNodes);
    printf("\nOverall size %d MB\n", ((countOfNodes * sizeof(nnn))) / (1024 * 1024));
    return 0;
}