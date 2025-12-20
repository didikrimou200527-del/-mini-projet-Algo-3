#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define WORD_LENGTH 5
#define MAX_TRIES 6
#define MAX_WORDS 5

#define GREEN_BG    "\033[42;30m"
#define YELLOW_BG   "\033[43;30m"
#define GRAY_BG     "\033[47;30m"
#define RESET_COLOR "\033[0m"

/* Allowed words */
const char *WORD_LIST[MAX_WORDS] = {
    "WORDS", "PLANE", "SHAKE", "OCEAN", "CRYPT"
};

/* ================= RANDOM WORD ================= */
const char *get_random_word() {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    return WORD_LIST[rand() % MAX_WORDS];
}

/* ================= PRINT COLORED CLUE ================= */
void print_clue_colored(const char *secret, const char *guess) {
    int count[26] = {0};
    char status[WORD_LENGTH];

    for (int i = 0; i < WORD_LENGTH; i++)
        count[secret[i] - 'A']++;

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            status[i] = 'G';
            count[guess[i] - 'A']--;
        } else {
            status[i] = 'B';
        }
    }

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (status[i] == 'B') {
            int idx = guess[i] - 'A';
            if (count[idx] > 0) {
                status[i] = 'Y';
                count[idx]--;
            }
        }
    }

    printf("   ");
    for (int i = 0; i < WORD_LENGTH; i++) {
        const char *color =
            status[i] == 'G' ? GREEN_BG :
            status[i] == 'Y' ? YELLOW_BG : GRAY_BG;
        printf("%s %c %s", color, guess[i], RESET_COLOR);
    }
    printf("\n");
}

/* ================= SOLVER FUNCTIONS ================= */
void get_feedback(const char *guess, const char *secret, int feedback[]) {
    int count[26] = {0};

    for (int i = 0; i < WORD_LENGTH; i++)
        count[secret[i] - 'A']++;

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (guess[i] == secret[i]) {
            feedback[i] = 2;
            count[guess[i] - 'A']--;
        } else {
            feedback[i] = 0;
        }
    }

    for (int i = 0; i < WORD_LENGTH; i++) {
        if (feedback[i] == 0) {
            int idx = guess[i] - 'A';
            if (count[idx] > 0) {
                feedback[i] = 1;
                count[idx]--;
            }
        }
    }
}

int same_feedback(int f1[], int f2[]) {
    for (int i = 0; i < WORD_LENGTH; i++)
        if (f1[i] != f2[i])
            return 0;
    return 1;
}

int filter_words(char current[][WORD_LENGTH + 1], int count,
                 const char *guess, int feedback[],
                 char next[][WORD_LENGTH + 1]) {

    int new_count = 0;
    int temp_feedback[WORD_LENGTH];

    for (int i = 0; i < count; i++) {
        get_feedback(guess, current[i], temp_feedback);
        if (same_feedback(feedback, temp_feedback)) {
            strcpy(next[new_count++], current[i]);
        }
    }
    return new_count;
}

/* ================= MAIN ================= */
int main() {

    int mode;
    printf(GREEN_BG " WORDLE C " RESET_COLOR "\n");
    printf("1. Human Player\n");
    printf("2. Solver (Automatic)\n");
    printf("Choose mode: ");
    scanf("%d", &mode);
    getchar();

    const char *SECRET_WORD = get_random_word();
    char secret[WORD_LENGTH + 1];

    for (int i = 0; i < WORD_LENGTH; i++)
        secret[i] = toupper(SECRET_WORD[i]);
    secret[WORD_LENGTH] = '\0';

    /* ================= HUMAN MODE ================= */
    if (mode == 1) {

        char guess[WORD_LENGTH + 2];
        int tries = 0;

        printf("\nGuess the word in %d tries\n\n", MAX_TRIES);

        while (tries < MAX_TRIES) {
            printf("Guess %d/%d: ", tries + 1, MAX_TRIES);
            fgets(guess, sizeof(guess), stdin);
            guess[strcspn(guess, "\n")] = '\0';

            if (strlen(guess) != WORD_LENGTH) {
                printf("Invalid length!\n");
                continue;
            }

            for (int i = 0; i < WORD_LENGTH; i++)
                guess[i] = toupper(guess[i]);

            print_clue_colored(secret, guess);
            printf("\n");
            if (strcmp(guess, secret) == 0) {
                printf(GREEN_BG " YOU WIN! " RESET_COLOR "\n");
                return 0;
            }
            tries++;
        }

        printf(GRAY_BG " GAME OVER " RESET_COLOR
               " Word was: %s\n", secret);
    }

    /* ================= SOLVER MODE ================= */
    else {

        char possible[MAX_WORDS][WORD_LENGTH + 1];
        char temp[MAX_WORDS][WORD_LENGTH + 1];
        int possible_count = MAX_WORDS;

        for (int i = 0; i < MAX_WORDS; i++)
            strcpy(possible[i], WORD_LIST[i]);

        int tries = 0;
        char guess[WORD_LENGTH + 1];

        printf("\nSolver is playing...\n\n");

        while (tries < MAX_TRIES && possible_count > 0) {

            strcpy(guess, possible[0]);
            printf("Solver guess %d/%d: %s\n",
                   tries + 1, MAX_TRIES, guess);

            int feedback[WORD_LENGTH];
            get_feedback(guess, secret, feedback);

            print_clue_colored(secret, guess);
            printf("\n");

            if (strcmp(guess, secret) == 0) {
                printf(GREEN_BG " SOLVER WINS " RESET_COLOR "\n");
                return 0;
            }

            possible_count = filter_words(
                possible, possible_count,
                guess, feedback, temp);

            for (int i = 0; i < possible_count; i++)
                strcpy(possible[i], temp[i]);

            tries++;
        }

        printf(GRAY_BG " SOLVER FAILED " RESET_COLOR
               " Word was: %s\n", secret);
    }

    return 0;
}