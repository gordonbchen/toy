#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./edit_dist str1 str2");
        return 0;
    }
    char *str1 = argv[1];
    char *str2 = argv[2];
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    // D-path: path w/ D non-diagonal edges.
    // V[k] = x endpoint of the farthest-reaching D-path on diagonal k.
    int maxD = len1 + len2;
    int *V = malloc(sizeof(int) * (2*maxD + 1));
    if (V == NULL) {
        perror("malloc");
        return 1;
    }

    // First move is down, checks V[1];
    V[maxD + 1] = 0;

    int x, y;
    int cont = 1;
    for (int D = 0; (D <= maxD) && cont; ++D) {
        for (int k = -D; k <= D; k += 2) {  // Step k (diags) in 2s b/c odd D -> odd k.
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
                    ++x;
                    ++y;
                }
            }
            V[maxD + k] = x;  // Can be updated in-place b/c odd and even k's are alternated in D.

            if ((x == len1) && (y == len2)) {
                printf("D=%d\n", D);
                cont = 0;
            }
        }
    }

    free(V);
    V = NULL;

    return 0;
}