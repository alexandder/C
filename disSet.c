#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

typedef struct element {
    struct element * parent;
    int number;
    int Rank;
} ELEMENT;

ELEMENT * makeSet(int x) {
    ELEMENT * el = (ELEMENT *)malloc(sizeof(ELEMENT));
    el->number = x;
    el->parent = el;
    el->Rank = 0;
    return el;
}

ELEMENT * findSet(ELEMENT * el) {
    if ( el != el->parent)
        el->parent = findSet(el->parent);
    return el->parent;
}

void link(ELEMENT * x, ELEMENT * y) {
    if (x->Rank > y->Rank)
        y->parent = x;
    else if (x->Rank < y->Rank)
        x->parent = y;
    else {
        y->parent = x;
        x->Rank += 1;
    }
}

void unionPc(ELEMENT * x, ELEMENT * y) {
    link(findSet(x), findSet(y));
}

void print(ELEMENT * tab[]) {
    int rep[SIZE];
    int i;
    for (i=0; i<SIZE; i++) {
        ELEMENT * temp = findSet(tab[i]);
        rep[i] = temp->number;
    }

    int j;
    for (i=0; i < SIZE; i++) {
        int check = 0;
        for (j=0; j < SIZE; j++) {
            if (rep[j] == i + 1) {
                printf("%d ", j + 1);
                check = 1;
            }
        }
        if (check == 1)
            printf("\n");
    }
}

int main()
{
    ELEMENT * tab[SIZE];
    int i;
    for (i=0; i < SIZE; i++) {
        tab[i] = makeSet(i+1);
    }
    unionPc(tab[0], tab[2]);
    unionPc(tab[3], tab[1]);
    unionPc(tab[9], tab[7]);
    unionPc(tab[7], tab[3]);
    unionPc(tab[5], tab[6]);
    unionPc(tab[7], tab[3]);
    unionPc(tab[6], tab[0]);
    print(tab);

    return 0;
}
