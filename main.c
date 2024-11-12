#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <errno.h>    

void hideCursor(){
    printf("\e[?25l");
}

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
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
#define INITIALSNAKELENGTH 2

typedef struct {
    int X;
    int Y;
} Position;

typedef struct {
    Position segments[ROWS * COLS];
    int length;
} Snake;

#define clrscr() printf("\e[1;1H\e[2J")
void gotoxy(int x,int y){
    printf("%c[%d;%df",0x1B,y,x);
}

void drawBoundaries(){
    for(int row = 0; row < ROWS+2; row++){
        for(int col = 0; col < COLS+2; col++){
            if(row == 0 || row == ROWS+1 || col == 0 || col == COLS+1){
                printf("%c",WALL);         
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}

void drawSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        if (i == 0) {
            // Huvudet
            gotoxy(snake->segments[i].X, snake->segments[i].Y);
            printf("O");
        } else {
            // Svansen
            gotoxy(snake->segments[i].X, snake->segments[i].Y);
            printf("o");
        }
    }
}

void clearSnake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        gotoxy(snake->segments[i].X, snake->segments[i].Y);
        printf(" ");
    }
}

typedef enum {
    KeyboardDir_Left,    
    KeyboardDir_Right,
    KeyboardDir_Up,
    KeyboardDir_Down,
}KeyboardDir;

int kbhit2()
{
    return _kbhit();
}

int getNextKeyboardAction(){
    if(kbhit2()){
        char ch = getch();
        return ch;
    }
    return 0;
}

int directionCounter = 0;  
void moveSnake(Snake *snake, KeyboardDir direction) {
    
    for (int i = snake->length - 1; i > 0; i--) {
        snake->segments[i] = snake->segments[i - 1];
    }

    if (direction == KeyboardDir_Up) {
        snake->segments[0].Y = (snake->segments[0].Y == 1) ? ROWS : snake->segments[0].Y - 1;
    }
    if (direction == KeyboardDir_Down) {
        snake->segments[0].Y = (snake->segments[0].Y == ROWS) ? 1 : snake->segments[0].Y + 1;
    }
    if (direction == KeyboardDir_Left) {
        snake->segments[0].X = (snake->segments[0].X == 1) ? COLS : snake->segments[0].X - 1;
    }
    if (direction == KeyboardDir_Right) {
        snake->segments[0].X = (snake->segments[0].X == COLS) ? 1 : snake->segments[0].X + 1;
    }

    
    directionCounter++;

    
    if (directionCounter >= 3) {
        snake->length++;  
        directionCounter = 0;
    }
}

int main(){
    Snake snake;
    snake.length = INITIALSNAKELENGTH;
    for (int i = 0; i < snake.length; i++) {
        snake.segments[i].X = 5 - i;
        snake.segments[i].Y = 5;
    }
    
    clrscr();
    hideCursor();
    drawBoundaries();

    while(1){
        clearSnake(&snake);
        gotoxy(0,0); 
        char ch = getNextKeyboardAction();
        
       
        if(ch == 'w') moveSnake(&snake, KeyboardDir_Up);
        else if(ch == 's') moveSnake(&snake, KeyboardDir_Down);
        else if(ch == 'a') moveSnake(&snake, KeyboardDir_Left);
        else if(ch == 'd') moveSnake(&snake, KeyboardDir_Right);

        
        drawSnake(&snake);
        
        msleep(250);
    }
    return 0;
}