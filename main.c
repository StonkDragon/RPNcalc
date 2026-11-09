#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#include "calc.h"

void makeInputRawAndHideCursor(void) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    printf("\033[?25l"); // Hide cursor
}

void resetInput(void) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    printf("\033[?25h"); // Show cursor
}

char* font[256] = {
    ['0'] = " ...  "
            ".  .. "
            ". . . "
            "..  . "
            " ...  "
            "      ",

    ['1'] = "  .   "
            " ..   "
            "  .   "
            "  .   "
            " ...  "
            "      ",

    ['2'] = " ...  "
            "    . "
            " ...  "
            ".     "
            " ...  "
            "      ",

    ['3'] = " ...  "
            "    . "
            "  ..  "
            "    . "
            " ...  "
            "      ",

    ['4'] = ".   . "
            ".   . "
            " .... "
            "    . "
            "    . "
            "      ",

    ['5'] = " ...  "
            ".     "
            " ...  "
            "    . "
            " ...  "
            "      ",

    ['6'] = " ...  "
            ".     "
            " ...  "
            ".   . "
            " ...  "
            "      ",

    ['7'] = "..... "
            "    . "
            "   .  "
            "  .   "
            "  .   "
            "      ",

    ['8'] = " ...  "
            ".   . "
            " ...  "
            ".   . "
            " ...  "
            "      ",

    ['9'] = " ...  "
            ".   . "
            " .... "
            "    . "
            " ...  "
            "      ",

    ['.'] = "      "
            "      "
            "      "
            "      "
            "  .   "
            "      ",

    ['-'] = "      "
            "      "
            " ...  "
            "      "
            "      "
            "      ",

    ['+'] = "      "
            "  .   "
            " ...  "
            "  .   "
            "      "
            "      ",

    ['*'] = "      "
            ". . . "
            " ...  "
            ". . . "
            "      "
            "      ",

    ['/'] = "  .   "
            "      "
            "..... "
            "      "
            "  .   "
            "      ",

    ['^'] = "  .   "
            " . .  "
            ".   . "
            "      "
            "      "
            "      ",
    
    ['='] = "      "
            " ...  "
            "      "
            " ...  "
            "      "
            "      ",

    ['_'] = "      "
            "      "
            "      "
            "      "
            "..... "
            "      ",

    ['a'] = "  .   "
            " . .  "
            "..... "
            ".   . "
            ".   . "
            "      ",

    ['b'] = "....  "
            ".   . "
            "....  "
            ".   . "
            "....  "
            "      ",

    ['c'] = " .... "
            ".     "
            ".     "
            ".     "
            " .... "
            "      ",

    ['d'] = "....  "
            ".   . "
            ".   . "
            ".   . "
            "....  "
            "      ",

    ['e'] = "..... "
            ".     "
            "....  "
            ".     "
            "..... "
            "      ",

    ['f'] = "..... "
            ".     "
            "....  "
            ".     "
            ".     "
            "      ",

    ['g'] = " .... "
            ".     "
            ".  .. "
            ".   . "
            " .... "
            "      ",

    ['h'] = ".   . "
            ".   . "
            "..... "
            ".   . "
            ".   . "
            "      ",

    ['i'] = " ...  "
            "  .   "
            "  .   "
            "  .   "
            " ...  "
            "      ",

    ['j'] = "  ... "
            "    . "
            "    . "
            "    . "
            "....  "
            "      ",

    ['k'] = ".   . "
            ".  .  "
            "...   "
            ".  .  "
            ".   . "
            "      ",

    ['l'] = ".     "
            ".     "
            ".     "
            ".     "
            "..... "
            "      ",

    ['m'] = ".   . "
            ".. .. "
            ". . . "
            ".   . "
            ".   . "
            "      ",

    ['n'] = ".   . "
            "..  . "
            ". . . "
            ".  .. "
            ".   . "
            "      ",

    ['o'] = " ...  "
            ".   . "
            ".   . "
            ".   . "
            " ...  "
            "      ",

    ['p'] = "....  "
            ".   . "
            "....  "
            ".     "
            ".     "
            "      ",

    ['q'] = " ...  "
            ".   . "
            ".   . "
            ".  .. "
            " .... "
            "      ",

    ['r'] = "....  "
            ".   . "
            "....  "
            ".  .  "
            ".   . "
            "      ",

    ['s'] = " .... "
            ".     "
            " ...  "
            "    . "
            "....  "
            "      ",

    ['t'] = "..... "
            "  .   "
            "  .   "
            "  .   "
            "  .   "
            "      ",

    ['u'] = ".   . "
            ".   . "
            ".   . "
            ".   . "
            " ...  "
            "      ",

    ['v'] = ".   . "
            ".   . "
            ".   . "
            " . .  "
            "  .   "
            "      ",

    ['w'] = ".   . "
            ".   . "
            ". . . "
            ".. .. "
            ".   . "
            "      ",

    ['x'] = ".   . "
            " . .  "
            "  .   "
            " . .  "
            ".   . "
            "      ",

    ['y'] = ".   . "
            " . .  "
            "  .   "
            "  .   "
            "  .   "
            "      ",

    ['z'] = "..... "
            "   .  "
            "  .   "
            " .    "
            "..... "
            "      ",

    [' '] = "      "
            "      "
            "      "
            "      "
            "      "
            "      ",

    [ 0 ] = "      "
            "      "
            "      "
            "      "
            "      "
            "      ",

    [255] = "..... "
            "..... "
            "..... "
            "..... "
            "..... "
            "      ",
};

char* cursor =  "      "
                "      "
                "      "
                "      "
                "      "
                "......";

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 6

char* contrast[] = { "░", "▒", "▓", "█" };

char lowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

void print() {
    const char* output = xgetoutput();
    const char* input = xgetinput();
    int input_cursor = xgetinputcursor();

    printf("\033[2J\033[1;1H");
    
    for (int row = 0; row < CHAR_HEIGHT; row++) {
        for (int charIndex = 0; charIndex < X_WIDTH; charIndex++) {
            char c = lowercase(output[charIndex]);
            const char* glyph = font[(unsigned char)c];
            if (glyph == NULL) {
                glyph = font[255]; // Use the default glyph for unknown characters
            }
            if (glyph) {
                for (int i = 0; i < CHAR_WIDTH; i++) {
                    if (glyph[row * CHAR_WIDTH + i] == ' ') {
                        printf(" ");
                    } else {
                        printf("%s", contrast[getContrast()]);
                    }
                }
            }
            printf(" "); // Add space between characters
        }
        printf("\n");
    }

    for (int row = 0; row < CHAR_HEIGHT; row++) {
        for (int charIndex = 0; charIndex < X_WIDTH; charIndex++) {
            int drawCursor = (charIndex == input_cursor) && (row == CHAR_HEIGHT - 1);

            char c = lowercase(input[charIndex]);
            const char* glyph = font[(unsigned char)c];
            if (glyph == NULL) {
                glyph = font[255]; // Use the default glyph for unknown characters
            }
            if (glyph) {
                for (int i = 0; i < CHAR_WIDTH; i++) {
                    if (glyph[row * CHAR_WIDTH + i] == ' ' && !drawCursor) {
                        printf(" ");
                    } else {
                        printf("%s", contrast[getContrast()]);
                    }
                }
            }
            printf(" "); // Add space between characters
        }
        printf("\n");
    }

    fflush(stdout);
}

void exit_program(int signum) {
    resetInput();
    printf("\033[2J\033[1;1H");
    printf("\033[?25h"); // Show cursor
    exit(0);
}

int main(void) {
    makeInputRawAndHideCursor();
    printf("\033[2J\033[1;1H");

    printf("\n");

    initialize();

    signal(SIGINT, exit_program);

    while (running()) {
        print();
        char c = getchar();
        if (c == '\033') {
            getchar(); // skip the '['
            char arrow = getchar();
            if (arrow == 'A') { // Up arrow
                press_arrow(ARROW_UP);
            } else if (arrow == 'B') { // Down arrow
                press_arrow(ARROW_DOWN);
            } else if (arrow == 'C') { // Right arrow
                press_arrow(ARROW_RIGHT);
            } else if (arrow == 'D') { // Left arrow
                press_arrow(ARROW_LEFT);
            }
        } else if (c == '\n' || c == ' ') {
            press_enter();
        } else if ((c >= '0' && c <= '9') || c == '.' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '=' || c == '_' || c == ',') {
            c = lowercase(c);
            press(c);
        } else if (c == 127) { // handle backspace
            press_backspace();
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            press_enter();
            press(c);
            press_enter();
        }
    }

    exit_program(0);
}
