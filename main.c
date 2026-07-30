#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 5

// ANSI Escape Codes for Colors & Effects
#define RESET       "\033[0m"
#define BLUE_BG     "\033[44m"
#define RED_TEXT    "\033[1;31m"
#define GREEN_TEXT  "\033[1;32m"
#define YELLOW_TEXT "\033[1;33m"
#define BLUE_TEXT   "\033[1;34m"
#define WHITE_TEXT  "\033[1;37m"

void clear_screen() {
    // Clears the terminal screen for a smooth frame-by-frame look
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n    ");
    for (int i = 0; i < BOARD_SIZE; i++) printf("%d ", i);
    printf("\n   -----------\n");

    for (int r = 0; r < BOARD_SIZE; r++) {
        printf(" %d |", r);
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c] == '~') {
                printf(BLUE_TEXT "~ " RESET); // Blue ocean
            } else if (board[r][c] == 'X') {
                printf(RED_TEXT "X " RESET);  // Red explosion Hit
            } else if (board[r][c] == 'O') {
                printf(YELLOW_TEXT "O " RESET); // Yellow Miss
            } else {
                printf("%c ", board[r][c]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void play_explosion_effect() {
    clear_screen();
    printf(RED_TEXT "\n\n      💥 BOOM!!! 💥\n\n" RESET);
    for(volatile long long i=0; i<190000000; i++); // Dramatic delay
}

int main() {
    char board[BOARD_SIZE][BOARD_SIZE];
    int ship_r, ship_c;
    int guess_r, guess_c;
    int attempts = 0;
    int max_attempts = 10;

    srand(time(0));
    ship_r = rand() % BOARD_SIZE;
    ship_c = rand() % BOARD_SIZE;

    // Initialize board with waves
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            board[r][c] = '~';
        }
    }

    clear_screen();
    printf(GREEN_TEXT "===================================\n" RESET);
    printf(WHITE_TEXT "      WELCOME TO BATTLESHIP!       \n" RESET);
    printf(GREEN_TEXT "===================================\n" RESET);
    printf("Find and sink the hidden enemy ship.\n");

    while (attempts < max_attempts) {
        print_board(board);
        printf("Torpedoes left: " RED_TEXT "%d" RESET "\n", max_attempts - attempts);
        
        printf("Enter Target Row (0-%d): ", BOARD_SIZE - 1);
        if (scanf("%d", &guess_r) != 1) break;
        printf("Enter Target Col (0-%d): ", BOARD_SIZE - 1);
        if (scanf("%d", &guess_c) != 1) break;

        if (guess_r < 0 || guess_r >= BOARD_SIZE || guess_c < 0 || guess_c >= BOARD_SIZE) {
            clear_screen();
            printf(YELLOW_TEXT "Torpedo strayed out of bounds! Try again.\n" RESET);
            continue;
        }

        if (board[guess_r][guess_c] != '~') {
            clear_screen();
            printf(YELLOW_TEXT "You already fired at that coordinates!\n" RESET);
            continue;
        }

        attempts++;

        if (guess_r == ship_r && guess_c == ship_c) {
            board[guess_r][guess_c] = 'X';
            play_explosion_effect();
            clear_screen();
            print_board(board);
            printf(GREEN_TEXT "🎯 DIRECT HIT! You sank the enemy flagship in %d shots!\n\n" RESET, attempts);
            return 0;
        } else {
            board[guess_r][guess_c] = 'O';
            clear_screen();
            printf(BLUE_TEXT "💦 Splash... It's a miss!\n" RESET);
        }
    }

    clear_screen();
    board[ship_r][ship_c] = 'S'; // Reveal ship location
    print_board(board);
    printf(RED_TEXT "GAME OVER! You ran out of torpedoes.\n" RESET);
    printf("The ship was hiding at Row %d, Col %d marked as [" GREEN_TEXT "S" RESET "].\n\n", ship_r, ship_c);

    return 0;
}
