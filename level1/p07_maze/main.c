#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

char a[7][12]={
     "###########"
    ,"#@   #    #"
    ,"# ## ##   #"
    ,"#  # #  # #"
    ,"# ##### # #"
    ,"#       # x"
    ,"###########"
    };

int x = 1, y = 1;

void display() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 12; j++) {
            printf("%c", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {

    while (1) {
        display();
        int c = getch();
        if (c == 'q') {
            return 0;
        }
        int xx = x, yy = y;
        if (c == 'w') {
            xx -= 1;
        } else if (c == 's') {
            xx += 1;
        } else if (c == 'a') {
            yy -= 1;
        } else if (c == 'd') {
            yy += 1;
        }
        if (a[xx][yy] == 'x') {
            printf("You win!\n");
            return 0;
        }
        if (a[xx][yy] != '#' && (xx != x || yy != y)) {
            a[x][y] = ' ';
            a[xx][yy] = '@';
            x = xx, y = yy;
        }
        printf("%d %d \n", x, y);
    }
}