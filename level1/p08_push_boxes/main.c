#include <stdio.h>
#include <stdlib.h>

char a[10][10], a_[10][10];
int x_size, y_size, x, y;

FILE *file, *file_in;

void load_map() {
    fscanf(file_in, "%d %d %d %d\n", &x_size, &y_size, &x, &y);
    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            fscanf(file_in, "%c", &a[i][j]);
            a_[i][j] = a[i][j];
        }
        fgetc(file_in);
    }
    a[x][y] = '@';
}

void display() {
    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            printf("%c", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int fix() {
    int cnt = 0;
    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            if (a_[i][j] == 'x') {
                if (a[i][j] == 'b') {
                    a[i][j] = '$';
                }
                else if(a[i][j] != '$') cnt++;

                if (a[i][j] == '^') {
                    a[i][j] = 'x';
                }
            }
            if (a[i][j] == '$' && a_[i][j] != 'x') {
                a[i][j] = 'b';
            }
        }
    }
    return cnt;
}

int move(char c, int* x, int* y) {
    int xx = *x, yy = *y;
    if (c == 'w') {
        xx -= 1;
    } else if (c == 's') {
        xx += 1;
    } else if (c == 'a') {
        yy -= 1;
    } else if (c == 'd') {
        yy += 1;
    }
    else return 0;

    if (a[xx][yy] == '#') {
        return 0;
    }
    if (a[xx][yy] == 'b' || a[xx][yy] == '$') {
        int bx = xx, by = yy;
        if(!move(c, &bx, &by)) {
            return 0;
        }
    }
    a[xx][yy] = a[*x][*y];
    a[*x][*y] = '^';
    *x = xx, *y = yy;
    return 1;
}

int step() {
    char c;
    while ((c = getchar()) == '\n') ;
    if (c == 'q')
        exit(0);
    move(c, &x, &y);
    if(!fix()) {
        return 0;
    }
    display();
    return 1;
}

int main() {

    file_in = fopen("maze_map.txt", "r");
    file = fopen("score_save.txt", "w");

    char a[100];

    int sum = 0;

    for (int i = 1; i <= 3; i++) {
        load_map();
        display();
        while (step());
        sum += 100;
        fprintf(file, "the %d level is solved. sum of points is %d\n", i, sum);
        printf("next level.\n");
    }

    return 0;
}