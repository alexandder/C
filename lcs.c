#include <stdio.h>
#include <string.h>

void LCS(char x[], char y[], char b[][strlen(y)]) {
    int m = strlen(x);
    int n = strlen(y);
    int c[m][n];
    int i, j;
    for(i = 0; i < m; i++) {
        c[i][0] = 0;
        b[i][0] = '0';
    }
    for(j = 0; j < n; j++) {
        c[0][j] = 0;
        b[0][j] = '0';
    }
    for(i = 1; i < m; i++) {
        for(j = 1; j < n; j++) {
            if (x[i] == y[j]) {
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = '\\';
            }
            else if (c[i-1][j] >= c[i][j-1]) {
                c[i][j] = c[i-1][j];
                b[i][j] = '|';
            }
            else {
                c[i][j] = c[i][j-1];
                b[i][j] = '-';
            }
        }
    }
}

void PrintLCS(char x[], char y[], char b[][strlen(y)], int i, int j) {
    if (i==0 || j == 0) {
        return;
    }
    if (b[i][j] == '\\') {
        PrintLCS(x, y, b, i-1, j-1);
        printf("%c", x[i]);
    }
    else if (b[i][j] == '|') {
        PrintLCS(x, y, b, i-1, j);
    }
    else {
        PrintLCS(x, y, b, i, j-1);
    }
}

int main()
{
    char x[] = {'0', 'a', 'b', 'd','b','\0'};
    char y[] = {'0', 'b', 'd', 'c', 'b','\0'};
    //x[strlen(x)] = '\0';
    //y[strlen(y)] = '\0';
    char b[strlen(x)][strlen(y)];
    LCS(x, y, b);
    int i, j;
    int m = strlen(x);
    int n = strlen(y);
    for(i = 0; i < m; i++) {
        for(j = 0; j < n; j++) {
                printf("%c", b[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("Najdluzszy wspolny podciag dla %s i %s to: \n", x, y);
    PrintLCS(x, y, b, strlen(x)-1, strlen(y)-1);
    getchar();
    return 0;
}
