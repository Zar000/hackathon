

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <errno.h>    

void hideCursor() {
    printf("\e[?25l");
}

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0) {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

#define ROWS 10 // 2-9
#define COLS 30 // 2-29
#define WALL '#'
#define MAX_SNAKE_LENGTH 100

typedef enum {
    KeyboardDir_Left,    
    KeyboardDir_Right,
    KeyboardDir_Up,
    KeyboardDir_Down,
} KeyboardDir;

typedef struct {
    int X;
    int Y;
} Position;

typedef struct {
    Position body[MAX_SNAKE_LENGTH];
    int length;
} Snake;

typedef struct {
    int X;
    int Y;
} Apple;

Position getNextPosition(Snake snake, KeyboardDir direction) {
    Position nextPosition;
    nextPosition.X = snake.body[0].X;
    nextPosition.Y = snake.body[0].Y;

    if (direction == KeyboardDir_Up) {
        nextPosition.Y = (nextPosition.Y == 2) ? ROWS + 1 : nextPosition.Y - 1;
    } else if (direction == KeyboardDir_Down) {
        nextPosition.Y = (nextPosition.Y == ROWS + 1) ? 2 : nextPosition.Y + 1;
    } else if (direction == KeyboardDir_Left) {
        nextPosition.X = (nextPosition.X == 2) ? COLS + 1 : nextPosition.X - 1;
    } else if (direction == KeyboardDir_Right) {
        nextPosition.X = (nextPosition.X == COLS + 1) ? 2 : nextPosition.X + 1;
    }

    return nextPosition;
}


#define clrscr() printf("\e[1;1H\e[2J")
void gotoxy(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

int score = 0;
char *scoreText = "Score : ";

void updateScore() {
    gotoxy(0, 30);
    printf("\r");
    printf("%s", scoreText);
    printf("%d", score);
}

void increaseScore() {
    score += 100;
    updateScore();
}

void drawBoundaries() {
    for (int row = 0; row < ROWS + 2; row++) {
        for (int col = 0; col < COLS + 2; col++) {
            if (row == 0 || row == ROWS + 1 || col == 0 || col == COLS + 1) {
                printf("%c", WALL);         
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void drawSnake(Snake snake) {
    for (int i = 0; i < snake.length; i++) {
        gotoxy(snake.body[i].X, snake.body[i].Y);
        printf("O");
    }
}

void clearSnake(Snake snake) {
    for (int i = 0; i < snake.length; i++) {
        gotoxy(snake.body[i].X, snake.body[i].Y);
        printf(" ");
    }
}

void drawApple(Apple apple) {
    gotoxy(apple.X, apple.Y);
    printf("A");
}

void placeApple(Apple *apple) {
    apple->X = (rand() % COLS) + 2;
    apple->Y = (rand() % ROWS) + 2;
}

int kbhit2() {
    return _kbhit();
}

int getNextKeyboardAction() {
    if (kbhit2()) {
        char ch = getch();
        return ch;
    }
    return 0;
}

void moveSnake(Snake *snake, KeyboardDir direction) {
    Position newHead = getNextPosition(*snake, direction);

    // Move the body
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    // Place the new head at the front
    snake->body[0] = newHead;
}

int main() {
    Snake snake;
    snake.length = 1;  // Initial length of the snake
    snake.body[0].X = 5; 
    snake.body[0].Y = 5;
    

    Apple apple;
    srand(time(NULL));  
    placeApple(&apple);

    clrscr();
    hideCursor();
    drawBoundaries();
    
    while (1) {
        clearSnake(snake);

        if (snake.body[0].X == apple.X && snake.body[0].Y == apple.Y) {
            placeApple(&apple);
            increaseScore();
            snake.length++;  // Grow the snake by adding a new segment
        }

        gotoxy(0, 0);
        char ch = getNextKeyboardAction();
        if (ch == 'w') moveSnake(&snake, KeyboardDir_Up);
        else if (ch == 's') moveSnake(&snake, KeyboardDir_Down);
        else if (ch == 'a') moveSnake(&snake, KeyboardDir_Left);
        else if (ch == 'd') moveSnake(&snake, KeyboardDir_Right);
        
        drawSnake(snake);
        drawApple(apple);
        msleep(250);
    }
    
    return 1;
}