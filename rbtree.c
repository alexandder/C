
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#define SIZE 30



struct node {
    enum color {BLACK, RED} color;
    unsigned int value;
    struct node* left;
    struct node* right;
    struct node* parent;
};

struct node* root = NULL;


struct node* treeSearch(int k) {
    struct node* x = root;
    while(x != NULL && x->value != k) {
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
    if (y->left != NULL) {
        y->left->parent = x;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void RightRotate(int k) {
    struct node* y = treeSearch(k);
    struct node* x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

void RBInsertFixup(int k) {
    struct node* z = treeSearch(k);
    while ( z->parent->color == RED) {
      if (z->parent == z->parent->parent->left) {
        struct node* y = z->parent->parent->right;
        if (y->color == RED) {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
        }
        else if (z == z->parent->right) {
            z = z->parent;
            LeftRotate(z->value);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        RightRotate(z->parent->parent->value);
      }
      else {
        struct node* y = z->parent->parent->left;
        if (y->color == RED) {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
        }
        else if (z == z->parent->left) {
            z = z->parent;
            RightRotate(z->value);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        LeftRotate(z->parent->parent->value);
      }
    }
    root->color = BLACK;
}


void RBInsert(int k){

    struct node* y = NULL;
    struct node* x = root;
    while (x != NULL){
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
    printf("test\n");
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    if (newNode == NULL) {
      printf("za malo pamieci\n");
      return;
    }
    else {
    newNode->parent = y;
    newNode->value = k;
    newNode->left = NULL;
    newNode->right = NULL;
    if ( y == NULL){
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
    newNode->color=RED;
    RBInsertFixup(k);
    printf("Element dodany\n");
    }
}


void display(struct node* x){
    if (x != NULL)
    {
        display(x->left);
        printf("%d\n", x->value);
        display(x->right);
    }
}



int my_itoa(int val, char* buf)
{
    const unsigned int radix = 10;

    char* p;
    unsigned int a;        //every digit
    int len;
    char* b;            //start of the digit char
    char temp;
    unsigned int u;

    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    u = (unsigned int)val;

    b = p;

    do
    {
        a = u % radix;
        u /= radix;

        *p++ = a + '0';

    } while (u > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return len;
}



void drawTree()
{
    initscr();

    if ( root != NULL)
    {
        int i=0;
        int col;
        int row;
        getmaxyx(stdscr, row, col);
    char* valueWord = (char *)malloc(5);
	int lenWord = my_itoa(root->value, valueWord);
        mvprintw(0, col/2 - lenWord, valueWord);

        if ( root->left != NULL)
        {
            row = 2; // pierwszy poziom
            struct node* l = root->left;
            int loc = col/2 - 6;
            drawLeftEdge(row, loc);
            row += 3;
            loc = loc - 5;
            drawLeftWord(row, loc, l);

            if (l->left != NULL)
            {
                row = 6; // drugi poziom
                struct node* ll = l->left;
                int loc2 = loc - 1;
                drawLeftEdge(row, loc2);
                loc2 = loc2 - 4;
                row += 3;
                drawLeftWord(row, loc2, ll);

                if (ll->left != NULL)
                {
                        row = 10; // poziom trzeci
                        struct node* lll = ll->left;
                        int loc3 = loc2 - 1;
                        drawLeftEdge(row, loc3);
                        loc3 = loc3 - 3;
                        row += 3;
                        drawLeftWord(row, loc3, lll);

                        if (lll->left != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 - 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 3;
                            row += 2;
                            struct node* llll = lll->left;
                            drawLeftWord(row, loc4, llll);

                        }

                        if (lll->right != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3;
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

                if (ll->right != NULL)
                {
                        row = 10; // poziom trzeci
                        struct node* llr = ll->right;
                        int loc3 = loc2 + 1;
                        drawRightEdge(row, loc3);
                        loc3 += 1;
                        row += 3;
                        drawRightWord(row, loc3, llr);

                        if ( llr->left != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* llrl = llr->left;
                            drawLeftWord(row, loc4, llrl);
                        }

                        if ( llr->right != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 + 1;
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

            if (l->right != NULL)
            {
                row = 6; // drugi poziom
                struct node* lr = l->right;
                int loc2 = loc + 2;
                drawRightEdge(row, loc2);
                row += 3;
                loc2 += 2;
                drawRightWord(row, loc2, lr);

                if (lr->left != NULL)
                {
                        struct node* lrl = lr->left; // poziom trzeci
                        int loc3 = loc2;
                        row = 11;
                        mvprintw(row, loc3, "/");
                        mvprintw(row + 1, loc3 - 1, "/");
                        row += 2;
                        loc3--;
                        drawLeftWord(row, loc3, lrl);

                        if (lrl->left != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 - 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* lrll = lrl->left;
                            drawLeftWord(row, loc4, lrll);
                        }

                        if (lrl->right != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* lrlr = lrl->right;
                            drawRightWord(row, loc4, lrlr);
                        }
                }

                if (lr->right != NULL)
                {
                        row = 11;
                        int loc3 = loc2 + 4; // trzeci poziom
                        struct node* lrr = lr->right;
                        mvprintw(row, loc3, "\\");
                        mvprintw(row + 1, loc3 + 1, "\\");
                        row += 2;
                        loc3++;
                        drawLeftWord(row, loc3 + 2, lrr);

                        if (lrr->left != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 + 2;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* lrrl = lrr->left;
                            drawLeftWord(row, loc4, lrrl);
                        }

                        if (lrr->right != NULL)
                        {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 + 3;
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

        if (root->right != NULL)
        {
            struct node* r = root->right; // pierwszy poziom
            int loc = col/2 + 6;
            row = 2;
            drawRightEdge(row, loc);
            loc += 3;
            row += 3;
            drawRightWord(row, loc, r);

            if (r->left != NULL)
            {
                row = 6; // drugi poziom
                struct node* rl = r->left;
                int loc2 = loc;
                drawLeftEdge(row, loc2);
                loc2 -= 3;
                row += 3;
                drawLeftWord(row, loc2, rl);

                if (rl->left != NULL)
                {
                    struct node* rll = rl->left; // poziom trzeci
                    int loc3 = loc2 - 1;
                    row = 11;
                    mvprintw(row, loc3, "/");
                    mvprintw(row + 1, loc3 - 1, "/");
                    row += 2;
                    loc3 -= 2;
                    drawLeftWord(row, loc3, rll);

                    if (rll->left != NULL)
                    {
                        row = 15; // poziom czwarty
                        int loc4 = loc3 - 1;
                        mvprintw(row, loc4, "/");
                        mvprintw(row + 1, loc4 - 1, "/");
                        loc4 -= 1;
                        row += 2;
                        struct node* rlll = rll->left;
                        drawLeftWord(row, loc4, rlll);
                    }

                    if (rll->right != NULL)
                    {
                        row = 15; // poziom czwarty
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

                if (rl->right != NULL)
                {
                    row = 11;
                    int loc3 = loc2 + 1; // trzeci poziom
                    struct node* rlr = rl->right;
                    mvprintw(row, loc3, "\\");
                    mvprintw(row + 1, loc3 + 1, "\\");
                    row += 2;
                    loc3++;
                    drawRightWord(row, loc3, rlr);

                    if (rlr->left != NULL)
                    {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 + 2;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* rlrl = rlr->left;
                            drawLeftWord(row, loc4, rlrl);

                    }

                    if (rlr->right != NULL)
                    {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 + 3;
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

            if (r->right != NULL)
            {
                row = 6; // drugi poziom
                struct node* rr = r->right;
                int loc2 = loc + 3;
                drawRightEdge(row, loc2);
                row += 3;
                loc2 += 3;
                drawRightWord(row, loc2, rr);

                if (rr->left != NULL)
                {
                    struct node* rrl = rr->left; // poziom trzeci
                    int loc3 = loc2 + 3;
                    row = 11;
                    mvprintw(row, loc3, "/");
                    mvprintw(row + 1, loc3 - 1, "/");
                    row += 2;
                    loc3 -= 2;
                    drawLeftWord(row, loc3, rrl);

                    if (rrl->left != NULL)
                    {
                        row = 15; // poziom czwarty
                        int loc4 = loc3 - 1;
                        mvprintw(row, loc4, "/");
                        mvprintw(row + 1, loc4 - 1, "/");
                        loc4 -= 1;
                        row += 2;
                        struct node* rrll = rrl->left;
                        drawLeftWord(row, loc4, rrll);
                    }

                    if (rrl->right != NULL)
                    {
                        row = 15; // poziom czwarty
                        int loc4 = loc3;
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

                if (rr->right != NULL)
                {
                    row = 11;
                    int loc3 = loc2 + 5; // trzeci poziom
                    struct node* rrr = rr->right;
                    mvprintw(row, loc3, "\\");
                    mvprintw(row + 1, loc3 + 1, "\\");
                    row += 2;
                    loc3++;
                    drawRightWord(row, loc3, rrr);

                    if (rrr->left != NULL)
                    {
                            row = 15; // poziom czwarty
                            int loc4 = loc3 + 2;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* rrrl = rrr->left;
                            drawLeftWord(row, loc4, rrrl);

                    }

                    if (rrr->right != NULL)
                    {
                            row = 15; // poziom czwarty
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

void drawLeftWord(int locr, int locc, struct node* left)
{
    char* valueWord = (char *)malloc(5);
    int lenWord = my_itoa(left->value, valueWord);
    mvprintw(locr, locc - lenWord, valueWord);
}

void drawRightWord(int locr, int locc, struct node* right)
{
    char* valueWord = (char *)malloc(5);
    int lenWord = my_itoa(right->value, valueWord);
    mvprintw(locr, locc, valueWord);
}

void drawLeftEdge(int x, int y)
{
    mvprintw(x, y, "/");
    mvprintw(x+1, y-1, "/");
    mvprintw(x+2, y-2, "/");
}

void drawRightEdge(int x, int y)
{
    mvprintw(x, y, "\\");
    mvprintw(x+1, y+1, "\\");
    mvprintw(x+2, y+2, "\\");
}

int main() {
  
  RBInsert(5);
  RBInsert(3);
  RBInsert(10);
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
        }
	else {
	  printf("Nie ma takiego wyboru\n");
	}
     } while (1);

    return 0;
}
