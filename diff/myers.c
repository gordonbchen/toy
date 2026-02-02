// Myer's diff algorithm: http://www.xmailserver.org/diff2.pdf

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef enum { OP_INS, OP_DEL } OpType;

static void die(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s str1 str2", argv[0]);
        return 1;
    }
    char *str1 = argv[1];
    char *str2 = argv[2];
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // D-path: path w/ D non-diagonal edges.
    // V[k] = x endpoint of the farthest-reaching D-path on diagonal k.
    int maxD = len1 + len2;
    int Vlen = 2*maxD + 1;
    int *V = malloc(sizeof(int) * Vlen);
    if (V == NULL) die("malloc");

    // trace stores a copy of V for each D (for backtracking).
    int *trace = malloc(sizeof(int) * Vlen * (maxD+1));
    if (trace == NULL) die("malloc");

    // First move is down, checks V[1];
    V[maxD + 1] = 0;

    int D, k, x, y;
    int cont = 1;
    for (D = 0; D <= maxD; ++D) {
        for (k = -D; k <= D; k += 2) {  // Step k (diags) in 2s b/c odd D -> odd k.
            // if k == -D: force down move, could not have been right move k=-D-1 -> k=-D (-D-1 not in edit budget).
            // if k == D: force right move, could not have been down move k=D+1 -> k = D (D+1 not in edit budget).
            if ((k == -D) || ((k != D) && (V[maxD + k-1] < V[maxD + k+1]))) {
                x = V[maxD + k+1];
            }
            else {
                x = V[maxD + k-1] + 1;  // +1 because we do a right move.
            }

            y = x - k;  // y reconstructed from x and k, that's why we can just store V.

            // Consume diagonal while chars match.
            if ((x >= 0) && (y >= 0)) {
                while ((x < len1) && (y < len2) && (str1[x] == str2[y])) {
                    ++x; ++y;
                }
            }
            V[maxD + k] = x;  // Can be updated in-place b/c odd and even k's are alternated in D.

            if ((x == len1) && (y == len2)) {
                printf("Edit dist = %d\n", D);
                cont = 0;
                break;
            }
        }
        memcpy(trace + D*Vlen, V, sizeof(int) * Vlen);
        if (!cont) break;
    }
    assert((x == len1) && (y == len2));

    // Backtrack.
    int edit_dist = D;
    OpType* ops = calloc(edit_dist, sizeof(OpType));
    if (ops == NULL) die("calloc");
    int* xs = malloc(sizeof(int) * edit_dist);
    if (xs == NULL) die("malloc");

    int* Vprev;
    while (D > 0) {
        Vprev = trace + (D-1)*Vlen;
        if ((k == -D) || ((k != D) && (Vprev[maxD + k-1] < Vprev[maxD + k+1]))) {
            ++k;  // move was down.
            ops[D-1] = OP_INS;
        }
        else {
            --k;  // move was right.
            ops[D-1] = OP_DEL;
        }
        x = Vprev[maxD + k];
        xs[D-1] = x;
        y = x - k;
        
        --D;
    }

    // Show diff.
    x = 0;
    y = 0;
    for (int i = 0; i < edit_dist; ++i) {
        while (x < xs[i]) {
            printf("%c ", str1[x]);
            ++x; ++y;
        }
        if (ops[i] == OP_INS) {
            printf("+%c ", str2[y]);
            ++y;
        }
        else {
            printf("-%c ", str1[x]);
            ++x;
        }
    }
    while (x < len1) {
        printf("%c ", str1[x]);
        ++x;
    }
    putchar('\n');

    free(V);
    free(trace);
    free(ops);
    free(xs);
    return 0;
}