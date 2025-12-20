1. Word Selection Strategy:
The game randomly selects a secret word from a pre-defined word list.
Each word is exactly 5 letters long and is valid according to the word list.
For the solver, the first guess is a fixed, high-information word (“SLATE”), chosen because it contains common vowels and consonants to maximize feedback information. For the player, the strategy depends on their personal choice of guesses.
2. Using Feedback to Eliminate Possibilities:
After each guess, the game provides feedback for each letter:
G (Green): Correct letter in the correct position.
Y (Yellow): Correct letter in the wrong position.
B (Black/Grey): Letter not in the word.
For the player: this feedback guides their next guess, helping them avoid impossible letters or positions.
For the solver: feedback is used programmatically to filter the candidate list:
Keep words matching all G positions.
Keep words that include Y letters but in different positions.
Eliminate words containing B letters in positions where they cannot appear.
3. Effectiveness of the Approach:
The combination of structured feedback and iterative guessing allows both human players and the solver to progressively narrow down possibilities.
For the solver, the systematic filtering guarantees progress each turn.
For the player, the visual clues and color-coded hints help identify letters and positions, making it easier to solve the word in a few tries.
This approach ensures the game remains challenging but fair, while enabling the solver to efficiently find the word, usually within 6 guesses.
b. Data Structure Justification
Arrays
Usage in your code:
WORD_LIST[MAX_WORDS] — stores the list of valid words.
char secret[WORD_LENGTH + 1] — stores the secret word.
char guess[WORD_LENGTH + 2] — stores user input guesses.
int count[26] — tracks the frequency of letters in a word.
char possible[MAX_WORDS][WORD_LENGTH + 1] — stores candidate words for the solver.
int feedback[WORD_LENGTH] — stores feedback for each letter (0 = gray, 1 = yellow, 2 = green).
Why arrays?
Fixed-size collections: All words have a known length (WORD_LENGTH) and small word list (MAX_WORDS). Arrays provide simple, contiguous memory storage.
Efficient indexing: Accessing letters and words by index is O(1).
Simplicity: Easy to loop over and manipulate (copy, compare, count).
Alternatives considered:
Linked lists: Could dynamically store words, but unnecessary since the list is small and fixed-size. Would add memory overhead and complexity.
Hash tables: Could be used to quickly check allowed words. Overkill for a small, fixed dictionary.
How this supports your strategy:
Arrays allow the solver to quickly iterate through all possible words, compute feedback, and filter candidates efficiently.
2. Integer Arrays (count[26])
Usage:
Tracks the frequency of each letter in the secret word for computing feedback.
Why integer arrays?
Maps letters (A-Z) to counts using a simple index = letter - 'A'.
Very fast to increment/decrement counts.
Simple memory usage (26 ints).
Alternative considered:
Hash map/dictionary: Could map letters to counts dynamically. Not necessary since letters are limited to 26 uppercase characters.
How this supports your strategy:
Provides constant-time lookup and update for letter frequencies during feedback calculation. This is critical for efficiently handling repeated letters in guesses.
3. 2D Character Arrays for Solver (possible and temp)
Usage:
Stores all possible candidate words for the solver and a temporary filtered list.
Why 2D arrays?
Simple to copy words between arrays using strcpy.
Easy to iterate over candidates during filtering.
Maintains a fixed maximum size (MAX_WORDS) for simplicity.
Alternative considered:
Dynamic array of strings (malloc + pointers): Could handle larger dictionaries, but increases complexity for memory allocation and deallocation.
Linked list of strings: Easier for dynamic removal but slower random access and copy operations.
How this supports your strategy:
The solver repeatedly filters possible words. Arrays allow O(n) iteration and O(1) access to candidates for comparison.
(c) Complexity Analysis
1. Time Complexity
Dictionary Validation (is_valid_word)
The function scans the entire dictionary to verify if a guessed word exists. Let N be the number of
words in the dictionary.
Time Complexity: O(N)
Feedback Computation (compute_feedback)
Operates on a fixed word length (5 letters).
Time Complexity: O(1)
Candidate Matching (match_feedback)
Performs fixed-length comparisons.
Time Complexity: O(1)
Solver Filtering Step (solver_next_guess)
Filters remaining candidate words using feedback.
Time Complexity per guess: O(M)
Worst-case: O(N)
Overall Solver Complexity
Maximum guesses = 6
Total complexity: O(N)
2. Space Complexity
Dictionary Storage
Stores up to 3000 words.
Space Complexity: O(N)
Solver Candidate List
Stores remaining possible words.
Space Complexity: O(N)
Temporary Filtering Buffer
Used during candidate reduction.
Space Complexity: O(N)
3. Scalability Discussion
The solver scales linearly with dictionary size and remains efficient for Wordle-sized datasets.
(d) Code Documentation
Game Module
get_random_word: Selects a random target word from the dictionary.
is_valid_word: Verifies that a guess exists in the dictionary.
compute_feedback: Generates Wordle-style feedback using color rules.
print_clue_colored: Displays feedback using terminal colors.
Solver Module
match_feedback: Validates candidate words against feedback rules.
solver_next_guess: Filters candidates and selects the next guess.
Main Program
Handles game flow, mode selection, attempts, and win/loss detection.
Example of Commented Code Snippet
/** * Filters candidate words based on Wordle feedback. * * @param word A candidate word from
the solver list * @param guess The previous guess made by the solver * @param fb Feedback
string (G, Y, B) from the game * @return 1 if the word matches the feedback constraints, 0
otherwise */ int match_feedback(const char *word, const char *guess, const char *fb) {

