#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 25

void chooseGameMode(int *mode);
int countNeighbours(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH], int y, int x);
void updateState(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH]);
void drawState(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH], WINDOW *win);
void setState(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH], int mode);
int finishGame(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH]);
void delay_changing(char input, int *delay);

int main() {
    char state[WINDOW_HEIGHT][WINDOW_WIDTH];
    int mode = 0, delay = 1000;
    char input;
    initscr();
    chooseGameMode(&mode);
    noecho();
    nodelay(stdscr, TRUE);
    WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    setState(state, mode);
    drawState(state, win);

    while (1) {
        input = getch();
        delay_changing(input, &delay);
        wrefresh(win);
        updateState(state);
        drawState(state, win);
        if (finishGame(state) == 1 || input == 'q' || input == 'Q') {
            printw("\nGAME OVER!");
            break;
        }
        napms(delay);
    }
    return 0;
}

void delay_changing(char input, int *delay) {
    if ((input == 'w' || input == 'W') && *delay > 200) {
        *delay -= 200;
        if (*delay < 200) {
            *delay = 200;
        }
    }
    if ((input == 's' || input == 'S') && *delay < 1800) {
        *delay += 200;
        if (*delay > 1800) {
            *delay = 1800;
        }
    }
}

void chooseGameMode(int *mode) {
    printw("Input pattern from 1 to 5: ");
    if (scanw("%d", mode) != 1 || *mode < 1 || *mode > 5) {
        printw("Incorrec input!\n");
        chooseGameMode(mode);
    }
}

int countNeighbours(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH], int y, int x) {
    int neighbours[8][2] = {{1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}};
    int count = 0;
    int col = 0;
    int row = 0;
    for (int i = 0; i < 8; i++) {
        row = y + neighbours[i][0];
        col = x + neighbours[i][1];

        if (col < 0) {
            col = WINDOW_WIDTH - 1;
        }
        if (col > WINDOW_WIDTH - 1) {
            col = 0;
        }
        if (row > WINDOW_HEIGHT - 1) {
            row = 0;
        }
        if (row < 0) {
            row = WINDOW_HEIGHT - 1;
        }

        if (matrix[row][col] == '1') {
            count++;
        }
    }
    return count;
}

void setState(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH], int mode) {
    char *filename = NULL;
    switch (mode) {
        case 1:
            filename = "./patterns/cow.txt";
            break;
        case 2:
            filename = "./patterns/glider_gun.txt";
            break;
        case 3:
            filename = "./patterns/glider.txt";
            break;
        case 4:
            filename = "./patterns/live-forever.txt";
            break;
        case 5:
            filename = "./patterns/pulsar.txt";
            break;
        default:
            filename = "./patterns/cow.txt";
            break;
    }
    FILE *fp = fopen(filename, "r");
    int row = 0;
    while (!feof(fp)) {
        for (int x = 0; x < WINDOW_WIDTH - 1; x++) {
            if (fscanf(fp, "%c", &matrix[row][x]) == EOF) {
                break;
            };
            if (matrix[row][x] == '\n') {
                x--;
                continue;
            }
        }
        row++;
        if (row == WINDOW_HEIGHT) {
            break;
        }
    }
}

void updateState(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH]) {
    int neighbours = 0;
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH - 1; x++) {
            neighbours = countNeighbours(matrix, y, x);

            if (neighbours < 2) {
                matrix[y][x] = '0';
            }

            if (neighbours > 3) {
                matrix[y][x] = '0';
            }

            if (neighbours == 3) {
                matrix[y][x] = '1';
            }
        }
    }
}

void drawState(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH], WINDOW *win) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            if (matrix[y][x] == '0') {
                mvwprintw(win, y, x, " ");
            } else if (matrix[y][x] == '1') {
                mvwprintw(win, y, x, "X");
            }
        }
        if (y != WINDOW_HEIGHT - 1) {
            printw("\n");
        }
    }
}

int finishGame(char matrix[WINDOW_HEIGHT][WINDOW_WIDTH]) {
    int isFinished = 1;
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            if (matrix[y][x] == '1') {
                isFinished = 0;
            }
        }
    }
    return isFinished;
}
