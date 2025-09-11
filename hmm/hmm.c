#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define N_STATES 2
#define N_LETTERS 4

const int NO_REG = 0;
const int REG = 1;
const double P_LETTERS[N_STATES][N_LETTERS] = {
    {0.25, 0.25, 0.25, 0.25},  // probs for no_reg.
    {0.15, 0.35, 0.35, 0.15}   // probs for reg.
};
const double P_START = 0.5;
const double P_STATES[N_STATES][N_STATES] = {
    {0.9, 0.1},  // p(next state | no_reg).
    {0.1, 0.9}   // p(next_state | reg).
};

char* max_product(char* dna, int len) {
    double* log_p = malloc(len * N_STATES * sizeof(double));
    assert(log_p != NULL);

    // p(s_1 | x_1) = prob of starting at s_1 * emission prob.
    for (int s = 0; s < N_STATES; ++s) {
        log_p[s] = log(P_START) + log(P_LETTERS[s][dna[0]]);
    }

    char* max_idx = malloc((len - 1) * N_STATES);
    assert(max_idx != NULL);

    // p(s_t | x_1..t) = max(p(s_t-1 | x_1..t-1) * p(s_t | s_t-1) * p(x_t | s_t))
    // probability of state t = max of (prob of prev state at t-1 * prob of transition) * emission prob
    for (int t = 1; t < len; ++t) {
        for (int s = 0; s < N_STATES; ++s) {
            double max_log_p = -DBL_MAX;
            char this_max_idx = 0;
            for (int prev_s = 0; prev_s < N_STATES; ++prev_s) {
                double this_log_p = log_p[(t - 1) * N_STATES + prev_s] + log(P_STATES[prev_s][s]);
                if (this_log_p > max_log_p) {
                    max_log_p = this_log_p;
                    this_max_idx = prev_s;
                }
            }
            max_log_p += log(P_LETTERS[s][dna[t]]);
            log_p[t * N_STATES + s] = max_log_p;
            max_idx[(t - 1) * N_STATES + s] = this_max_idx;
        }
    }

    char* pred_states = malloc(len);
    assert(pred_states != NULL);

    char max_last_idx = 0;
    double max_last_log_p = -DBL_MAX;
    for (int s = 0; s < N_STATES; ++s) {
        if (log_p[(len - 1) * N_STATES + s] > max_last_log_p) {
            max_last_idx = s;
        }
    }
    pred_states[len - 1] = max_last_idx;

    for (int t = len - 2; t >= 0; --t) {
        pred_states[t] = max_idx[t * N_STATES + pred_states[t + 1]];
    }

    free(log_p);
    free(max_idx);
    return pred_states;
}

char dna_to_idx(char c) {
    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
    }
    return 3;
}

int main(int argc, char* argv[]) {
    assert(argc == 2);
    FILE *fptr = fopen(argv[1], "r");
    assert(fptr != NULL);

    char c;
    while ((c = fgetc(fptr)) != '\n') {}

    int len = 0;
    int max_len = 2048;

    char* dna = malloc(max_len);
    assert(dna != NULL);

    while ((c = fgetc(fptr)) != EOF) {
        if (c == '\n') {
            continue;
        }
        if (len == max_len) {
            max_len *= 2;
            dna = realloc(dna, max_len);
            assert(dna != NULL);
        }

        dna[len] = dna_to_idx(c);
        ++len;
    }

    char* pred_states = max_product(dna, len);
    for (int i = 0; i < len; ++i) {
        putchar('0' + pred_states[i]);
    };
    putchar('\n');

    free(dna);
    free(pred_states);

    fclose(fptr);
}

