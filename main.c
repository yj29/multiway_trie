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
int input[5000050];
int output[5000050];


struct node *lookup_node;
static int prefix, ct = 1, pref;
static int f_prefix = 0;
struct node *root_bg = NULL;
int numberOfChildern = numberOfChildren;
static int countOfNodes = 0;

struct node *create_node() {
    struct node *root = (struct node *) malloc(sizeof(struct node));
    root->isleaf = false;
    root->isBroken = false;
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

struct node *insert_mt(char *a, struct node *root) {
    struct node *nd = root;
    struct node *prev = NULL;
    for (int i = 0; a[i] != '\0' && pref > 0; i = i + numberOfBits) {
        // printf("I= %d\n",i);
        char *temp = malloc(sizeof(char) * numberOfBits);
        int k = 0;
        while (k < numberOfBits) {
            temp[k] = '0';
            k++;
        }
        k = numberOfBits - 1;
        //k = 0;
        int c =-1;
        int t =numberOfBits -1;
        while (k >= 0 && pref > 0) {
            if (a[i + k] == '\0' || (i + k) > 128) {
                k--;
                //c++;
                //pref--; //crosscheck
                continue;
            }
            temp[k] = a[i + k];
            k--;
            pref--;
            c++;
            t--;
        }

        int decimal = converToDecimal(temp);
        if (nd->children[decimal] == NULL) {
            struct node *n = create_node();
            if (t != -1) {
                nd->children[c] = n;
                //prev->children[decimal] = NULL;
                prev =nd;
                nd = nd->children[c];
                prev->isBroken = true;
                break;
            }else{
                nd->children[decimal] = n;
                prev = nd;
                nd = nd->children[decimal];
            }
            countOfNodes++;
        } else {
            prev = nd;
            nd = nd->children[decimal];
        }
    }
    return nd;
}

int converToDecimal(char *temp) {
    int p = 0;
    int i;
    int dec = 0;
    for (i = numberOfBits - 1; i >= 0; --i) {
        dec += (temp[i] - 48) * (int) pow((double) 2, p);
        p++;
    }
    return dec;
}

void insert_route_in_multi_trie(__uint64_t *key, int p) {
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
    struct node *nd1 = insert_mt(rev, root_bg);
    nd1->isleaf = true;
}

int *lookup_mt(char *a, struct node *root) {
    struct node *nd = root;
    struct node *prev = NULL;
    for (int i = 0; a[i] != '\0'; i = i + numberOfBits) {
        char *temp = malloc(sizeof(char) * numberOfBits);
        int k = 0;
        while (k < numberOfBits) {
            temp[k] = '0';
            k++;
        }
        k = numberOfBits - 1;
        //k = 0;
        while (k >= 0) {
            if (a[i + k] == '\0' || (i + k) > 128) {
                k--;
                break;
            }
            temp[k] = a[i + k];
            k--;
        }
        int decimal = converToDecimal(temp);
        if(nd == NULL){
            return f_prefix;
        }
        if (nd->isBroken && numberOfBits > 1) { //&& nd->children[decimal] == NULL) {
            //if (numberOfBits > 1) {
            //if (prev != NULL) {
            int prefix_1 = fineTunePrefix(nd, temp, numberOfBits, 0);
            if (prefix_1 != -1) {
                f_prefix += prefix + prefix_1;
            } else {
                f_prefix += prefix;
            }
            nd =nd->children[prefix_1-1];
            //break;
        } else {
            prefix += numberOfBits;
            if (nd->children[decimal]!=NULL && nd->children[decimal]->isleaf) {
                f_prefix = prefix;
            }
            prev = nd;
            nd = nd->children[decimal];

        }

    }
    lookup_node = nd;
    return f_prefix;
}

int fineTunePrefix(struct node *prev, char *temp, int bits, int startBit) {
    int out = -1;
    int i = 0;
    int j = 0;
    int max = -1;


    for (i = 0; i < numberOfChildren; i++) {
        if (prev->children[i] != NULL && prev->children[i]) {
            max = i + 1;// > max ? index : max;
        }
    }
    /*for (i = startBit; i < bits; i++) {
        int index = 0;
        char *t = malloc(sizeof(char) * numberOfBits);
        for (int k = 0; k < bits; ++k) {
            t[k] = '0';
        }

        for (j = 0; j < i + 1; j++) {
            t[j] = temp[j];
            index++;
        }
        int dec = converToDecimal(t);
        if (prev->children[dec] != NULL && prev->children[dec]->isleaf) {
            max = index > max ? index : max;
        }
        int t1 = fineTunePrefix(prev, temp, bits - 1, startBit + 1);
        return max < t1 ? t1 : max;
    }*/
    //out = (max != -1) ? max : out;
    return max;
}

int lookup_in_multi_trie(__uint64_t *key) {
    int tt;
    __uint64_t n = *(key + 0);
    char a[65] = "";
    char rev[129] = "";
    int i = 0;

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
    for (int l = l = strlen(a) - 1, t = 0; l >= 0; --l, t++) {
        rev[t] = a[l];
    }

    char b[65] = "";
    char rev1[65] = "";
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
    f_prefix = 0;
    prefix = 0;
    tt = lookup_mt(rev, root_bg);
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
    //FILE *ft1 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_input.txt", "r");
    // FILE *ft2 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_output.txt", "r");
    FILE *ft1 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/test1.txt", "r");
    //int c = 0;
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
        input[i] = ad[i].a2;
        insert_route_in_multi_trie(net_prefix, ad[i].a2);
    }

    //--------------------LOOKUP IN m-way trie
    //FILE *ft = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_output.txt", "r");
    FILE *ft = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/test_out1.txt", "r");
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
        printf("Lookup: %d\n", i);
        int pref = lookup_in_multi_trie(&ad[i]);
        output[i] = pref;
        //if (i < 1005 && i >= 805)
        //printf("prefix=%d\n", pref);
    }

    gettimeofday(&t1, NULL);
    elap = (t1.tv_sec * 1e6 + t1.tv_usec - t0.tv_sec * 1e6 - t0.tv_usec) / (1e6);
    printf("\ntime elapse in second=%.10f", elap);
    printf("\nNumber of nodes: %d\n", countOfNodes);
    // printf("\nOverall size %d MB\n", ((countOfNodes * sizeof(nnn))) / (1024 * 1024));


    FILE *f = fopen("file.txt", "w");
    for (int i = 0; i < idx; i++) {
        fprintf(f, "\n%d \t %d", input[i], output[i]);
    }
    fclose(f);
    return 0;
}