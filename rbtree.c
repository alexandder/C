#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#define SIZE 30



struct node {
    enum color {BLACK, RED} color;
    unsigned int value;
    struct node* left;
    struct node* right;
    struct node* parent;
};


struct node* NIL;
struct node* root;

void makeNIL() {
  NIL = (struct node*)malloc(sizeof(struct node));
  NIL->parent = root;
  NIL->left = NIL;
  NIL->right = NIL;
  NIL->color = BLACK;
  root = NIL;
}


struct node* treeSearch(int k) {
    struct node* x = root;
    while(x != NIL && x->value != k) {
        if ( k < x->value)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

void LeftRotate(int k) {
    struct node* x = treeSearch(k);
    struct node* y = x->right;
    x->right = y->left;
    if (y->left != NIL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;
    }
    else {
        if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
}

void RightRotate(int k) {
    struct node* x = treeSearch(k);
    struct node* y = x->left;
    x->left = y->right;
    if (y->right != NIL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NIL) {
        root = y;
    }
    else {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void RBInsertFixup(int k) {
    struct node* z = treeSearch(k);
    while (z != root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            struct node* y = z->parent->parent->right;
            if (y != NIL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    LeftRotate(z->value);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(z->parent->parent->value);
            }
        }
        else {
            struct node* y = z->parent->parent->left;
            if (y != NIL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    RightRotate(z->value);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(z->parent->parent->value);
            }
        }
    }
    root->color = BLACK;
}


void RBInsert(int k){

    struct node* y = NIL;
    struct node* x = root;
    while (x != NIL){
        y = x;
        if (k < x->value){
            x = x->left;
        }
        else if(k == x->value){
            printf("Wartosc juz wystepuje w drzewie\n");
            return;
        }
        else{
            x = x->right;
        }
    }
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if (newNode == NULL) {
        printf("za malo pamieci\n");
        exit(1);
    }
    else {
        newNode->parent = y;
        newNode->value = k;
        newNode->left = NIL;
        newNode->right = NIL;
        newNode->color=RED;
        if ( y == NIL){
            root = newNode;
        }
        else {
            if (newNode->value< y->value){
                y->left = newNode;
            }
            else {
                y->right = newNode;
            }
        }
        RBInsertFixup(k);
        printf("Element dodany\n");
    }
}


void display(struct node* x){
    if (x != NIL) {
        display(x->left);
        printf("%d\n", x->value);
        display(x->right);
    }
}


int my_itoa(int val, char* buf)
{
    const unsigned int radix = 10;
    char* p;
    unsigned int a;
    int len;
    char* b;
    char temp;
    unsigned int u;
    p = buf;
    if (val < 0) {
        *p++ = '-';
        val = 0 - val;
    }
    u = (unsigned int)val;
    b = p;
    do {
        a = u % radix;
        u /= radix;
        *p++ = a + '0';
    } while (u > 0);

    len = (int)(p - buf);
    *p-- = 0;
    do {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;
    } while (b < p);
    return len;
}

void drawLeftWord(int locr, int locc, struct node* left) {
    char* valueWord = (char *)malloc(5);
    int lenWord = my_itoa(left->value, valueWord);
    init_pair( 1, COLOR_RED, COLOR_BLACK );
    if (left->color == RED) {
        attron( COLOR_PAIR( 1 ) );
        mvprintw(locr, locc - lenWord, valueWord);
        attroff( COLOR_PAIR( 1 ) );
    }
    if (left->color == BLACK) {
        attron( COLOR_PAIR( 2 ) );
        mvprintw(locr, locc - lenWord, valueWord);
        attroff( COLOR_PAIR( 2 ) );
    }
}

void drawRightWord(int locr, int locc, struct node* right) {
    char* valueWord = (char *)malloc(5);
    int lenWord = my_itoa(right->value, valueWord);
    init_pair( 1, COLOR_RED, COLOR_BLACK );
    if (right->color == RED) {
        attron( COLOR_PAIR( 1 ) );
        mvprintw(locr, locc, valueWord);
        attroff( COLOR_PAIR( 1 ) );
    }
    if (right->color == BLACK) {
        attron( COLOR_PAIR( 2 ) );
        mvprintw(locr, locc, valueWord);
        attroff( COLOR_PAIR( 2 ) );
    }
}

void drawLeftEdge(int x, int y) {
    mvprintw(x, y, "/");
    mvprintw(x+1, y-1, "/");
    mvprintw(x+2, y-2, "/");
}

void drawRightEdge(int x, int y) {
    mvprintw(x, y, "\\");
    mvprintw(x+1, y+1, "\\");
    mvprintw(x+2, y+2, "\\");
}

void drawTree() {
    initscr();
    start_color();
    if ( root != NIL) {
        int col;
        int row;
        getmaxyx(stdscr, row, col);
        char* valueWord = (char *)malloc(5);
        int lenWord = my_itoa(root->value, valueWord);
        mvprintw(0, col/2 - lenWord, valueWord);
        if ( root->left != NIL)
        {
            row = 2; // pierwszy poziom
            struct node* l = root->left;
            int loc = col/2 - 9;
            drawLeftEdge(row, loc);
            row += 3;
            loc = loc - 2;
            drawLeftWord(row, loc, l);

            if (l->left != NIL)
            {
                row = 6; // drugi poziom
                struct node* ll = l->left;
                int loc2 = loc - 3;
                drawLeftEdge(row, loc2);
                loc2 = loc2 - 2;
                row += 3;
                drawLeftWord(row, loc2, ll);

                if (ll->left != NIL)
                {
                        row = 10; // poziom trzeci
                        struct node* lll = ll->left;
                        int loc3 = loc2 - 3;
                        drawLeftEdge(row, loc3);
                        loc3 = loc3 - 2;
                        row += 3;
                        drawLeftWord(row, loc3, lll);

                        if (lll->left != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 - 3;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* llll = lll->left;
                            drawLeftWord(row, loc4, llll);

                        }

                        if (lll->right != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 - 2;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* lllr = lll->right;
                            drawRightWord(row, loc4, lllr);
                        }
                }

                if (ll->right != NIL)
                {
                        row = 10; // poziom trzeci
                        struct node* llr = ll->right;
                        int loc3 = loc2;
                        drawRightEdge(row, loc3);
                        loc3 += 1;
                        row += 3;
                        drawRightWord(row, loc3, llr);

                        if ( llr->left != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 - 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            row += 2;
                            struct node* llrl = llr->left;
                            drawLeftWord(row, loc4, llrl);
                        }

                        if ( llr->right != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* llrr = llr->right;
                            drawRightWord(row, loc4, llrr);
                        }
                }
            }

            if (l->right != NIL)
            {
                row = 6; // drugi poziom
                struct node* lr = l->right;
                int loc2 = loc + 1;
                drawRightEdge(row, loc2);
                row += 3;
                loc2 += 3;
                drawRightWord(row, loc2, lr);

                if (lr->left != NIL)
                {
                        struct node* lrl = lr->left; // poziom trzeci
                        int loc3 = loc2 + 1;
                        row = 10;
                        drawLeftEdge(row, loc3);
                        row += 3;
                        loc3--;
                        drawLeftWord(row, loc3, lrl);

                        if (lrl->left != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 - 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            row += 2;
                            struct node* lrll = lrl->left;
                            drawLeftWord(row, loc4, lrll);
                        }

                        if (lrl->right != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 2;
                            row += 4;
                            struct node* lrlr = lrl->right;
                            drawRightWord(row, loc4, lrlr);
                        }
                }

                if (lr->right != NIL)
                {
                        row = 10;
                        int loc3 = loc2 + 2; // trzeci poziom
                        struct node* lrr = lr->right;
                        drawRightEdge(row, loc3);
                        row += 3;
                        loc3 += 2;
                        drawLeftWord(row, loc3 + 2, lrr);

                        if (lrr->left != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 + 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 += 2;
                            row += 2;
                            struct node* lrrl = lrr->left;
                            drawLeftWord(row, loc4, lrrl);
                        }

                        if (lrr->right != NIL)
                        {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 + 2;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* lrrr = lrr->right;
                            drawRightWord(row, loc4, lrrr);
                        }
                }
            }
        }

        if (root->right != NIL) {
            struct node* r = root->right; // pierwszy poziom
            int loc = col/2 + 7;
            row = 2;
            drawRightEdge(row, loc);
            loc += 3;
            row += 3;
            drawRightWord(row, loc, r);

            if (r->left != NIL)
            {
                row = 6; // drugi poziom
                struct node* rl = r->left;
                int loc2 = loc + 1;
                drawLeftEdge(row, loc2);
                loc2 -= 1;
                row += 3;
                drawLeftWord(row, loc2, rl);

                if (rl->left != NIL)
                {
                    struct node* rll = rl->left; // poziom trzeci
                    int loc3 = loc2 - 2;
                    row = 10;
                    drawLeftEdge(row, loc3);
                    row += 3;
                    loc3 -= 1;
                    drawLeftWord(row, loc3, rll);

                    if (rll->left != NIL)
                    {
                        row = 14; // poziom czwarty
                        int loc4 = loc3 - 1;
                        mvprintw(row, loc4, "/");
                        mvprintw(row + 1, loc4 - 1, "/");
                        row += 2;
                        struct node* rlll = rll->left;
                        drawLeftWord(row, loc4, rlll);
                    }

                    if (rll->right != NIL)
                    {
                        row = 14; // poziom czwarty
                        int loc4 = loc3;
                        mvprintw(row, loc4, "\\");
                        mvprintw(row + 1, loc4 + 1, "\\");
                        mvprintw(row + 2, loc4 + 2, "\\");
                        mvprintw(row + 3, loc4 + 3, "\\");
                        loc4 += 3;
                        row += 4;
                        struct node* rllr = rll->right;
                        drawRightWord(row, loc4, rllr);
                    }
                }

                if (rl->right != NIL)
                {
                    row = 10;
                    int loc3 = loc2; // trzeci poziom
                    struct node* rlr = rl->right;
                    drawRightEdge(row, loc3);
                    row += 3;
                    loc3 = loc3 + 2;
                    drawRightWord(row, loc3, rlr);

                    if (rlr->left != NIL)
                    {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 + 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 += 1;
                            row += 2;
                            struct node* rlrl = rlr->left;
                            drawLeftWord(row, loc4, rlrl);

                    }

                    if (rlr->right != NIL)
                    {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 + 2;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* rlrr = rlr->right;
                            drawRightWord(row, loc4, rlrr);
                    }
                }
            }

            if (r->right != NIL)
            {
                row = 6; // drugi poziom
                struct node* rr = r->right;
                int loc2 = loc + 5;
                drawRightEdge(row, loc2);
                row += 3;
                loc2 += 3;
                drawRightWord(row, loc2, rr);

                if (rr->left != NIL)
                {
                    struct node* rrl = rr->left; // poziom trzeci
                    int loc3 = loc2 + 3;
                    row = 10;
                    drawLeftEdge(row, loc3);
                    row += 3;
                    drawLeftWord(row, loc3, rrl);

                    if (rrl->left != NIL)
                    {
                        row = 14; // poziom czwarty
                        int loc4 = loc3 - 2;
                        mvprintw(row, loc4, "/");
                        mvprintw(row + 1, loc4 - 1, "/");
                        loc4 += 1;
                        row += 2;
                        struct node* rrll = rrl->left;
                        drawLeftWord(row, loc4, rrll);
                    }

                    if (rrl->right != NIL)
                    {
                        row = 14; // poziom czwarty
                        int loc4 = loc3 - 1;
                        mvprintw(row, loc4, "\\");
                        mvprintw(row + 1, loc4 + 1, "\\");
                        mvprintw(row + 2, loc4 + 2, "\\");
                        mvprintw(row + 3, loc4 + 3, "\\");
                        loc4 += 3;
                        row += 4;
                        struct node* rrlr = rrl->right;
                        drawRightWord(row, loc4, rrlr);
                    }
                }

                if (rr->right != NIL)
                {
                    row = 10;
                    int loc3 = loc2 + 5; // trzeci poziom
                    struct node* rrr = rr->right;
                    drawRightEdge(row, loc3);
                    row += 3;
                    loc3 += 2;
                    drawRightWord(row, loc3, rrr);

                    if (rrr->left != NIL) {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 + 2;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 += 1;
                            row += 2;
                            struct node* rrrl = rrr->left;
                            drawLeftWord(row, loc4, rrrl);
                    }

                    if (rrr->right != NIL) {
                            row = 14; // poziom czwarty
                            int loc4 = loc3 + 3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* rrrr = rrr->right;
                            drawRightWord(row, loc4, rrrr);

                    }
                }
            }
        }
    }

    getch();
    endwin();
}

int main() {

    makeNIL();
    do {
		printf("\n");
		printf("MENU\n");
		printf("0 - zakonczenie\n");
		printf("1 - dopisanie\n");
		printf("2 - wyswietlenie\n");
		printf("3 - rysowanie\n");
		printf("Wybierz opcje: ");
		char choice[1];
		scanf("%s", choice);
		printf("\n");

		if (strcmp(choice, "0") == 0) {
			printf("Do widzenia!\n");
			break;
		}

		else if (strcmp(choice, "1") == 0) {
			int newElement;
			printf("Podaj wartosc: ");
			scanf("%d", &newElement);
			RBInsert(newElement);
		}

		else if (strcmp(choice, "2") == 0) {
			display(root);
		}

		else if (strcmp(choice, "3") == 0) {
			drawTree();
			char c;
			while ((c=getchar()) != "\n");
		}

		else {
			printf("Nie ma takiego wyboru\n");
		}

	} while (1);

    return 0;
}
