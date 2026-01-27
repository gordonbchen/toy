#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int min(int a, int b) {
    return (a <= b) ? a : b;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./edit_dist str1 str2");
        return 0;
    }
    char *str1 = argv[1];
    char *str2 = argv[2];
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    // Fill edit dist table.
    int *E = malloc(sizeof(int) * (len1+1) * (len2+1));  // (len1+1, len2+1).
    for (int i = 0; i < len1+1; ++i) E[i * (len2+1)] = i;
    for (int j = 0; j < len2+1; ++j) E[j] = j;

    int del, insert, match;
    for (int i = 1; i < len1+1; ++i) {
        for (int j = 1; j < len2+1; ++j) {
            del = E[(i-1)*(len2+1) + j] + 1;
            insert = E[i*(len2+1) + j-1] + 1;
            match = E[(i-1)*(len2+1) + j-1] + (str1[i-1] != str2[j-1]);
            E[i*(len2+1) + j] = min(min(del, insert), match);
        }
    }
    printf("Edit dist: %d\n", E[(len1+1)*(len2+1)-1]);

    // Build gapped strings.
    int maxlen = len1 + len2;
    char *gap1 = malloc(maxlen + 1);
    char *gap2 = malloc(maxlen + 1);
    gap1[maxlen] = '\0';
    gap2[maxlen] = '\0';

    int i = len1;
    int j = len2;
    int k = maxlen;
    int e;
    while (i > 0 || j > 0) {
        --k;
        e = E[i*(len2+1) + j];
        if ((i > 0 && j > 0) && e == E[(i-1)*(len2+1) + j-1] + (str1[i-1] != str2[j-1])) {
            gap1[k] = str1[i-1];
            gap2[k] = str2[j-1];
            --i;
            --j;
        }
        else if ((j > 0) && e == E[i*(len2+1) + j-1] + 1) {
            gap1[k] = '-';
            gap2[k] = str2[j-1];
            --j;
        }
        else {
            gap1[k] = str1[i-1];
            gap2[k] = '-';
            --i;
        }
    }

    // Print gapped strings.
    for (int idx = k; idx < maxlen; ++idx) {
        if (gap1[idx] == gap2[idx]) printf(ANSI_COLOR_GREEN);
        else printf(ANSI_COLOR_RED);
        putchar(gap1[idx]);
    }
    putchar('\n');

    for (int idx = k; idx < maxlen; ++idx) {
        if (gap1[idx] == gap2[idx]) printf(ANSI_COLOR_GREEN);
        else printf(ANSI_COLOR_RED);
        putchar(gap2[idx]);
    }
    putchar('\n');

    // Free memory.
    free(E);
    E = NULL;
    free(gap1);
    gap1 = NULL;
    free(gap2);
    gap2 = NULL;
}
