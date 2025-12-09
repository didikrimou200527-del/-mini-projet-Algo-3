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

// Allowed words
const char *WORD_LIST[MAX_WORDS] = {
    "WORDS", "PLANE", "SHAKE", "OCEAN", "CRYPT"
};

// Pick random word
const char *get_random_word() {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    int index = rand() % MAX_WORDS;
    return WORD_LIST[index];
}

// Check if guess is in word list
int is_valid_word(const char *guess) {
    for (int i = 0; i < MAX_WORDS; i++) {
        if (strcasecmp(guess, WORD_LIST[i]) == 0) {
            return 1;  
        }
    }
    return 0;  
}

// Print colored clue
void print_clue_colored(const char *secret, const char *guess) {
    int secret_counts[26] = {0};
    char clue_status[WORD_LENGTH + 1];
    int i;

    for (i = 0; i < WORD_LENGTH; i++) {
        if (toupper(guess[i]) == toupper(secret[i])) {
            clue_status[i] = 'G';
        } else {
            clue_status[i] = 'B';
            secret_counts[toupper(secret[i]) - 'A']++;
        }
    }

    for (i = 0; i < WORD_LENGTH; i++) {
        if (clue_status[i] == 'B') {
            int guess_index = toupper(guess[i]) - 'A';
            if (secret_counts[guess_index] > 0) {
                clue_status[i] = 'Y';
                secret_counts[guess_index]--;
            }
        }
    }

    printf("   ");
    for (i = 0; i < WORD_LENGTH; i++) {
        const char *color;

        switch (clue_status[i]) {
            case 'G': color = GREEN_BG; break;
            case 'Y': color = YELLOW_BG; break;
            default:  color = GRAY_BG; break;
        }
        printf("%s %c %s", color, toupper(guess[i]), RESET_COLOR);
    }
    printf("\n");
}

// Main program
int main() {
    const char *SECRET_WORD = get_random_word();
    char guess[WORD_LENGTH + 2];
    int tries = 0;

    printf("\n" GREEN_BG " WORDLE C " RESET_COLOR " - Guess the %d-letter word in %d tries.\n\n",
           WORD_LENGTH, MAX_TRIES);

    while (tries < MAX_TRIES) {
        printf("Guess %d/%d: ", tries + 1, MAX_TRIES);

        if (fgets(guess, sizeof(guess), stdin) == NULL) {
            printf("\nExiting...\n");
            return 0;
        }

        size_t len = strlen(guess);
        if (len > 0 && guess[len - 1] == '\n')
            guess[--len] = '\0';

        if (len != WORD_LENGTH) {
            printf("   " YELLOW_BG " ERROR " RESET_COLOR " Please enter a %d-letter word.\n",
                   WORD_LENGTH);
            continue;
        }

        // Convert to uppercase
        for (int i = 0; i < WORD_LENGTH; i++)
            guess[i] = toupper(guess[i]);

        // Validate guess
        if (!is_valid_word(guess)) {
            printf("   " YELLOW_BG " INVALID WORD " RESET_COLOR " Word not in list.\n");
            continue;
        }

        // Print clues
        print_clue_colored(SECRET_WORD, guess);
        printf("\n");

        // Check win
        if (strcmp(guess, SECRET_WORD) == 0) {
            printf(GREEN_BG " CONGRATULATIONS! " RESET_COLOR
                   " You guessed the word: %s\n\n", SECRET_WORD);
            return 0;
        }

        tries++;
    }

    printf(GRAY_BG " GAME OVER " RESET_COLOR " You ran out of tries.\n");
    printf("The secret word was: " YELLOW_BG " %s " RESET_COLOR "\n\n", SECRET_WORD);

    return 0;
}
