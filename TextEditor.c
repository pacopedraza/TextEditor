/*** includes ***/

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void kill(const char *s) {
    perror(s);
    exit(1);
}

void disablingRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        kill("tcsetattr");
}

void enablingRawMode() {
    // Read terminal attributes
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) kill("tcgetattr");
    atexit(disablingRawMode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    // Apply the attributes to the terminal
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) kill("tcsetattr");
}

/*** init ***/
int main() {
    enablingRawMode();

    while (1) {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) kill("read");
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q')  break;
    }

    return 0;
}