#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

static const int N = 8, M = 15;
char char_map[8][15];

char letter;
int x_of_letter, y_of_letter;

void initialize_map() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            char_map[i][j] = ' ';
        }
    }
    for (int i = 0; i < N; i++) {
        char_map[i][0] = '*';
        char_map[i][M - 1] = '*';
    }
    for (int i = 0; i < M; i++) {
        char_map[0][i] = '*';
        char_map[N - 1][i] = '*';
    }
    char_map[1][1] = letter;
    x_of_letter = 1, y_of_letter = 1;
}

void check_position() {
    if (y_of_letter == M - 1) {
        y_of_letter = 2;
        x_of_letter += 1;
    }
    if (x_of_letter == N - 1) {
        x_of_letter = 2;
    }
}

void move() {
    char_map[x_of_letter][y_of_letter] = ' ';

    ++y_of_letter;
    check_position();

    char_map[x_of_letter][y_of_letter] = letter;
}

void display() {
    system("cls");
    Sleep(100);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("%c", char_map[i][j]);
        }
        printf("\n");
    }
}

int speed;

void running_letter() {
    initialize_map();
    while (true) {
        move();
        display();
        Sleep(1000 / speed);
    }
}

int main() {

    printf("enter letter:");
    scanf("%c", &letter);
    printf("enter speed:");
    scanf("%d", &speed);

    system("cls");
    Sleep(100);
    running_letter(speed);

    return 0;
}
