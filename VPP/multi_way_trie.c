/**
 * @author Yash Jain
 * m-way trie for IPv6 lookup
 */

#include <sys/socket.h>
#include <rpc/types.h>
#include "multi_way_trie.h"
#include <arpa/inet.h>
#include <math.h>

struct address {
    unsigned long long a0;
    unsigned long long a1;
    unsigned long long a2;
};
/*
struct address ad[510050];
int idx = 0;
int input[510050];
int output[510050];
int pre[40];
static int pre_index = 0;
*/


static int prefix, ct = 1, pref;
static int f_prefix = 0;
struct node_mt *root_bg = NULL;
int numberOfChildern = numberOfChildren;
static int countOfNodes = 0;

struct node_mt *create_node_mt() {
    struct node_mt *root = (struct node_mt *) malloc(sizeof(struct node_mt));
    root->isleaf = false;
    root->isBroken = false;
    root->brokenPrefix = -1;
    root->isBrokenLeaf = false;
    int i = 0;
    for (i = 0; i < numberOfChildern; i++) {
        root->children[i] = NULL;
    }
    return root;
}

//convert into unsigned int

/*
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
    // printf("\n%d %d",idx,prefixlen);
    return ip6_ret;
}
*/


/**
 * Masking method
 * @param adr
 * @param pref
 * @return
 */

/*__uint64_t *create_mask(uint64_t *adr, int pref) {
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
}*/


struct node_mt *insert_mt(char *a, struct node_mt *root) {
    struct node_mt *nd = root;
    struct node_mt *prev = NULL;
    int i = 0;
    for (i = 0; a[i] != '\0' && pref > 0; i = i + numberOfBits) {
        // printf("I= %d\n",i);
        char *temp = malloc(sizeof(char) * numberOfBits);
        int k = 0;
        int c = -1;
        int t = numberOfBits - 1;
        while (k < numberOfBits && pref > 0) {
            if (a[i + k] == '\0' || (i + k) > 127) {
                break;
            }
            temp[k] = a[i + k];
            k++;
            pref--;
            c++;
            t--;
        }
        while (k < numberOfBits) {
            temp[k] = '0';
            k++;
        }

        int decimal = converToDecimal(temp);
        if (nd->children[decimal] == NULL) {
            struct node_mt *n = create_node_mt();
            countOfNodes++;
            if (t != -1) {
                nd->children[decimal] = n; //CHECK WITH RAHUL
                prev = nd;
                nd = nd->children[decimal];
                nd->brokenPrefix = c + 1;
                prev->isBroken = true;
                if (pref == 0) {
                    nd->isBrokenLeaf = true;
                    break;
                }
            } else {
                nd->children[decimal] = n;
                nd = nd->children[decimal];
                if (pref == 0) {
                    nd->isleaf = true;
                }
            }
        } else {
            if (t != -1) {
                prev = nd;
                nd = nd->children[decimal];
                nd->brokenPrefix = c + 1;
                prev->isBroken = true;
                nd->isBrokenLeaf = true;
                break;
            } else {
                nd = nd->children[decimal];
                if (pref == 0) {
                    nd->isleaf = true;
                }
            }
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
    int k = 0;
    int l = 0;
    int t = 0;

    char b[65] = "";
    int j = 0;
    char rev1[65] = "";
    __uint64_t m = key[1];

     n = ((n >> 56) & 0xFF) | ((n >> 40) & 0xFF00) | ((n >> 24) & 0xFF0000) | ((n >> 8) & 0xFF000000) |
         ((n << 56) & 0xFF00000000000000) | ((n << 40) & 0x00FF000000000000) | ((n << 24) & 0x0000FF0000000000) |
         ((n << 8) & 0x000000FF00000000);
     m = ((m >> 56) & 0xFF) | ((m >> 40) & 0xFF00) | ((m >> 24) & 0xFF0000) | ((m >> 8) & 0xFF000000) |
         ((m << 56) & 0xFF00000000000000) | ((m << 40) & 0x00FF000000000000) | ((m << 24) & 0x0000FF0000000000) |
         ((m << 8) & 0x000000FF00000000);

    //root creation
    if (ct == 1) {
        root_bg = create_node_mt();
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
        for (k = i; k < 64; k++)
            a[k] = 48;
    }
    for (l = strlen(a) - 1, t = 0; l >= 0; --l, t++) {
        rev[t] = a[l];

    }

    //if (pref > 0) {
    while (m != 0) {
        int k = m % 2;
        b[j] = k + 48;
        m = m / 2;
        j++;
    }
    if (j < 64) {
        for (k = j; k < 64; k++)
            b[k] = 48;
    }
    for (l = strlen(b) - 1, t = 0; l >= 0; --l, t++) {
        rev1[t] = b[l];

    }
    for (i = 64; i < 129; i++) {
        rev[i] = rev1[i - 64];
    }
    /* FILE *pFile2 = fopen("input.txt", "a");
     //fprintf(pFile2,"%s", strcat(rev,'\0'));
     fputs(  rev, pFile2);
     fputs("\n",pFile2);
     fclose(pFile2);*/
    //}
    insert_mt(rev, root_bg);
}

int lookup_mt(char *a, struct node_mt *root) {
    struct node_mt *nd = root;
    int i = 0;
    for (i = 0; a[i] != '\0'; i = i + numberOfBits) {
        char *temp = malloc(sizeof(char) * numberOfBits);
        int k = 0;
        while (k < numberOfBits) {
            temp[k] = '0';
            k++;
        }
        k = 0;
        int c = -1;
        while (k < numberOfBits) {
            if (a[i + k] == '\0' || (i + k) > 127) {
                break;
            }
            c++;
            temp[k] = a[i + k];
            k++;
        }

        int decimal = converToDecimal(temp);
        if (nd == NULL) {
            //return f_prefix;
            break;
        }


        if (nd->children[decimal] != NULL && nd->children[decimal]->isleaf) {
            int bit = numberOfBits;
            f_prefix = prefix + bit;
            nd = nd->children[decimal];
            prefix += numberOfBits;
            continue;
        }else{
            struct node_mt *res;
            if (nd->isBroken) {
                res = fineTunePrefix(nd, temp, c);
                if (res != NULL) {
                    f_prefix = prefix + res->brokenPrefix;
                }
                nd = nd->children[decimal];
                prefix += numberOfBits;
            } else {
                nd = nd->children[decimal];
                prefix += numberOfBits;
            }
        }
    }
    return f_prefix;
}

struct node_mt *fineTunePrefix(struct node_mt *prev, char *temp, int bits) {
    //int out = -1;
    int i = 0;
    int j = 0;
    int max = -1;
    int k = 0;
    struct node_mt *res = NULL;

    for (i = 0; i <= bits; i++) {
        char *t = malloc(sizeof(char) * numberOfBits);
        for (k = 0; k < numberOfBits; ++k) {
            t[k] = '0';
        }
        int bitsConsidered = 0;
        for (j = 0; j < i + 1; j++) {
            t[j] = temp[j];
            bitsConsidered = j + 1;
        }
        int dec = converToDecimal(t);
        if (prev->children[dec] != NULL && prev->children[dec]->isBrokenLeaf &&
            prev->children[dec]->brokenPrefix == bitsConsidered) {
            res = prev->children[dec]->brokenPrefix > max ? prev->children[dec] : res;
            max = prev->children[dec]->brokenPrefix > max ? prev->children[dec]->brokenPrefix : max;

        }
    }
    return res;
}

int lookup_in_multi_trie(__uint64_t *key) {
    int tt;
    __uint64_t n = *(key + 0);
    char a[65] = "";
    char rev[129] = "";
    int i = 0;
    int k = 0;
    int t = 0;
    int l = 0;

    char b[65] = "";
    char rev1[65] = "";
    int j = 0;
    __uint64_t m = *(key + 1);

    n = ((n >> 56) & 0xFF) | ((n >> 40) & 0xFF00) | ((n >> 24) & 0xFF0000) | ((n >> 8) & 0xFF000000) |
        ((n << 56) & 0xFF00000000000000) | ((n << 40) & 0x00FF000000000000) | ((n << 24) & 0x0000FF0000000000) |
        ((n << 8) & 0x000000FF00000000);
    m = ((m >> 56) & 0xFF) | ((m >> 40) & 0xFF00) | ((m >> 24) & 0xFF0000) | ((m >> 8) & 0xFF000000) |
        ((m << 56) & 0xFF00000000000000) | ((m << 40) & 0x00FF000000000000) | ((m << 24) & 0x0000FF0000000000) |
        ((m << 8) & 0x000000FF00000000);
    prefix = 0;


    if (m == 0 && n == 0) {
        return 0;
    }
    while (n != 0) {
        int r = n % 2;
        a[i] = r + 48;
        n = n / 2;
        i++;
    }

    if (i < 64) {
        for (k = i; k < 64; k++)
            a[k] = 48;
    }
    for (l = strlen(a) - 1, t = 0; l >= 0; --l, t++) {
        rev[t] = a[l];
    }

    while (m != 0) {
        int k = m % 2;
        b[j] = k + 48;
        m = m / 2;
        j++;
    }
    if (j < 64) {
        for (k = j; k < 64; k++)
            b[k] = 48;
    }

    for (l = strlen(b) - 1, t = 0; l >= 0; --l, t++) {
        rev1[t] = b[l];
    }

    for (i = 64; i < 129; i++) {
        rev[i] = rev1[i - 64];
    }
    f_prefix = 0;
    prefix = 0;
    tt = lookup_mt(rev, root_bg);
    return tt;

}


/*
int main() {
    bool t;
    struct node_mt nnn;
    //sizeOfOneNode = (int) sizeof(nnn);
    printf("%d\n", sizeof(nnn));
    int n_way = numberOfBits;
    char *line = NULL;
    size_t len = 0;
    int n;
    double elap;
    struct timeval t0, t1;

    //----------------------CREATING m-way trie----------------------------
    // FILE *ft1 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_input.txt", "r");
    // FILE *ft2 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_output.txt", "r");
    FILE *ft1 = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/5_john.txt", "r");
    //int c = 0
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
        if (i == 499018) {
            printf("");
        }
        //printf("Inserting: %d\n", i);
        uint64_t *net_prefix = create_mask(&ad[i], ad[i].a2);
        //insertion on trie
        input[i] = ad[i].a2;
        insert_route_in_multi_trie(net_prefix, ad[i].a2);
        int ind = 0;
        bool isPresent = false;
        while (ind < pre_index) {
            if (pre[ind] == ad[i].a2) {
                isPresent = true;
                break;
            }
            ind++;
        }
        if (!isPresent) {
            pre[pre_index++] = ad[i].a2;
        }
    }

    //--------------------LOOKUP IN m-way trie
    // FILE *ft = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/poptrie_output.txt", "r");
    FILE *ft = fopen("/Users/YASH/Documents/spring-18/capstone/inputdata/6_john_t.txt", "r");
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
    FILE *fil = fopen("incorrect.txt", "w");
    for (i = 0; i < idx; i = i + 1) {
        //  printf("Lookup: %d\n", i);
        if (i == 425898) {
            printf("");
        }
        int p = lookup_in_multi_trie(&ad[i]);
        int ind = 0;
        bool isPresent = false;
        while (ind < pre_index) {
            if (pre[ind] == p) {
                isPresent = true;
                break;
            }
            ind++;
        }
        if (!isPresent) {
            fprintf(fil, "\n%d %d", i, p);
        }
        output[i] = p;
    }
    fclose(fil);

    gettimeofday(&t1, NULL);
    elap = (t1.tv_sec * 1e6 + t1.tv_usec - t0.tv_sec * 1e6 - t0.tv_usec) / (1e6);
    printf("\ntime elapse in second=%.10f", elap);
    printf("\nNumber of nodes: %d\n", countOfNodes);
    // printf("\nOverall size %d MB\n", ((countOfNodes * sizeof(nnn))) / (1024 * 1024));

    char numb[10];
    sprintf(numb, "%d", numberOfBits);
    FILE *f = fopen(strcat(numb, "file.txt"), "w");
    FILE *fincorrect = fopen("file_incorrect.txt", "w");
    for (int i = 0; i < idx; i++) {
        if (input[i] != output[i])
            fprintf(fincorrect, "\n%d\t %d \t %d", i, input[i], output[i]);
        fprintf(f, "\n%d\t%d\t%d", i, input[i], output[i]);
    }
    fclose(f);


    return 0;
}
*/
