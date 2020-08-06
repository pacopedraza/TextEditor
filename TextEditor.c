#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disablingRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enablingRawMode() {
    // Read terminal attributes
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disablingRawMode);
    struct termios raw = orig_termios;
    // Turn off the raw (not seing on terminal what is typed)
    raw.c_lflag &= ~(ECHO | ICANON);
    // Apply the attributes to the terminal
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enablingRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}