#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <curses.h>
#define SIZE 30



struct node {
    char* word;
    unsigned int arity;
    struct node* left;
    struct node* right;
    struct node* parent;
};

struct node* root = NULL;


void add(char* givenWord){

    struct node* y = NULL;
    struct node* x = root;
    while (x != NULL){
        y = x;
        if (strcmp(givenWord, x->word) < 0){
            x = x->left;
        }
        else if(strcmp(givenWord, x->word) == 0){
            x->arity++;
            printf("Krotnosc zwiekszona\n");
            return;
        }
        else{
            x = x->right;
        }
    }
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->parent = y;
    newNode->word = givenWord;
    newNode->arity = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    if ( y == NULL){
        root = newNode;
    }
    else {
        if (strcmp(newNode->word, y->word ) < 0){
            y->left = newNode;
        }
        else {
            y->right = newNode;
        }
    }
    printf("Element dodany\n");
}

struct node* minimum(struct node* x)
{
    while (x->left != NULL)
        x = x->left;
    return x;
}

struct node* succesor(struct node* x)
{
    if (x->right != NULL)
        return minimum(x->right);
    struct node* y = x->parent;
    while (y != NULL && x == y->right)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

struct node* searchTree(char givenWord[])
{
        struct node* x = root;
        while (x != NULL && strcmp(givenWord, x->word) != 0)
        {
            if (strcmp(givenWord, x->word) < 0)
                x = x->left;
            else
                x = x->right;
        }
        return x;
}

void rmv(char givenWord[])
{

    struct node* z = searchTree(givenWord);
    if (z == NULL)
    {
        printf("Nie ma takiego slowa\n");
    }
    else
    {
        if (z->arity > 1)
            z->arity = z->arity - 1;
        else
		{
			struct node* y;
			struct node* x;
			if (z->left == NULL || z->right == NULL)
				y = z;
			else
				y = succesor(z);
			if (y->left != NULL)
				x = y->left;
			else
				x = y->right;
			if (x != NULL)
				x->parent = y->parent;
			if (y->parent == NULL)
				root = x;
			else if (y == y->parent->left)
				y->parent->left = x;
			else
				y->parent->right = x;
			if (y != z)
			{
				z->word = y->word;
				z->arity = y->arity;
			}
		}
	}
}

void display(struct node* x){
    if (x != NULL)
    {
        display(x->left);
        printf("%s \t %d\n", x->word, x->arity);
        display(x->right);
    }
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
        char* arity=(char*)malloc(sizeof(char));
        mvprintw(0, col/2 - strlen(root->word), root->word);
        itoa(root->arity, arity, 10);
        mvprintw(1, col/2 - strlen(root->word)/2 , arity);

        if ( root->left != NULL)
        {
            row = 2;                                 // pierwszy poziom
            struct node* l = root->left;
            int loc = col/2 - strlen(root->word) - 5;
            drawLeftEdge(row, loc);
            row += 3;
            loc = loc - 5;
            drawLeftWord(row, loc, l);

            if (l->left != NULL)
            {
                row = 6;                               // drugi poziom
                struct node* ll = l->left;
                int loc2 = loc - strlen(ll->word) - 1;
                drawLeftEdge(row, loc2);
                loc2 = loc2 - 4;
                row += 3;
                drawLeftWord(row, loc2, ll);

                if (ll->left != NULL)
                {
                        row = 10;                                // poziom trzeci
                        struct node* lll  = ll->left;
                        int loc3 = loc2 - strlen(ll->word) - 1;
                        drawLeftEdge(row, loc3);
                        loc3 = loc3 - 3;
                        row += 3;
                        drawLeftWord(row, loc3, lll);

                        if (lll->left != NULL)
                        {
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3 - 1;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 3;
                            row += 2;
                            struct node* llll  = lll->left;
                            drawLeftWord(row, loc4, llll);

                        }

                        if (lll->right != NULL)
                        {
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* lllr  = lll->right;
                            drawRightWord(row, loc4, lllr);
                        }
                }

                if (ll->right != NULL)
                {
                        row = 10;                                 // poziom trzeci
                        struct node* llr  = ll->right;
                        int loc3 = loc2 + strlen(ll->word)/2;
                        drawRightEdge(row, loc3);
                        loc3 += 1;
                        row += 3;
                        drawRightWord(row, loc3, llr);

                        if ( llr->left != NULL)
                        {
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "/");
                            mvprintw(row + 1, loc4 - 1, "/");
                            loc4 -= 1;
                            row += 2;
                            struct node* llrl  = llr->left;
                            drawLeftWord(row, loc4, llrl);
                        }

                        if ( llr->right != NULL)
                        {
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3 + 1;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* llrr  = llr->right;
                            drawRightWord(row, loc4, llrr);
                        }
                }
            }

            if (l->right != NULL)
            {
                row = 6;                                // drugi poziom
                struct node* lr  = l->right;
                int loc2 = loc + 2;
                drawRightEdge(row, loc2);
                row += 3;
                loc2 += strlen(lr->word);
                drawRightWord(row, loc2, lr);

                if (lr->left != NULL)
                {
                        struct node* lrl  = lr->left;         // poziom trzeci
                        int loc3 = loc2;
                        row = 11;
                        mvprintw(row, loc3, "/");
                        mvprintw(row + 1, loc3 - 1, "/");
                        row += 2;
                        loc3--;
                        drawLeftWord(row, loc3, lrl);

                        if (lrl->left != NULL)
                        {
                            row = 15;                              // poziom czwarty
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
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* lrlr  = lrl->right;
                            drawRightWord(row, loc4, lrlr);
                        }
                }

                if (lr->right != NULL)
                {
                        row = 11;
                        int loc3 = loc2 + 4;                               // trzeci poziom
                        struct node* lrr  = lr->right;
                        mvprintw(row, loc3, "\\");
                        mvprintw(row + 1, loc3 + 1, "\\");
                        row += 2;
                        loc3++;
                        drawLeftWord(row, loc3 + 2, lrr);

                        if (lrr->left != NULL)
                        {
                            row = 15;                              // poziom czwarty
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
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3 + 3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* lrrr  = lrr->right;
                            drawRightWord(row, loc4, lrrr);
                        }
                }
            }
        }

        if (root->right != NULL)
        {
            struct node* r = root->right;              // pierwszy poziom
            int loc = col/2 + 6;
            row = 2;
            drawRightEdge(row, loc);
            loc += 3;
            row += 3;
            drawRightWord(row, loc, r);

            if (r->left != NULL)
            {
                row = 6;                             // drugi poziom
                struct node* rl = r->left;
                int loc2 = loc;
                drawLeftEdge(row, loc2);
                loc2 -= 3;
                row += 3;
                drawLeftWord(row, loc2, rl);

                if (rl->left != NULL)
                {
                    struct node* rll  = rl->left;         // poziom trzeci
                    int loc3 = loc2 - 1;
                    row = 11;
                    mvprintw(row, loc3, "/");
                    mvprintw(row + 1, loc3 - 1, "/");
                    row += 2;
                    loc3 -= 2;
                    drawLeftWord(row, loc3, rll);

                    if (rll->left != NULL)
                    {
                        row = 15;                              // poziom czwarty
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
                        row = 15;                              // poziom czwarty
                        int loc4 = loc3;
                        mvprintw(row, loc4, "\\");
                        mvprintw(row + 1, loc4 + 1, "\\");
                        mvprintw(row + 2, loc4 + 2, "\\");
                        mvprintw(row + 3, loc4 + 3, "\\");
                        loc4 += 3;
                        row += 4;
                        struct node* rllr  = rll->right;
                        drawRightWord(row, loc4, rllr);
                    }
                }

                if (rl->right != NULL)
                {
                    row = 11;
                    int loc3 = loc2 + 1;                               // trzeci poziom
                    struct node* rlr  = rl->right;
                    mvprintw(row, loc3, "\\");
                    mvprintw(row + 1, loc3 + 1, "\\");
                    row += 2;
                    loc3++;
                    drawRightWord(row, loc3, rlr);

                    if (rlr->left != NULL)
                    {
                            row = 15;                              // poziom czwarty
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
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3 + 3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* rlrr  = rlr->right;
                            drawRightWord(row, loc4, rlrr);

                    }
                }
            }

            if (r->right != NULL)
            {
                row = 6;                             // drugi poziom
                struct node* rr  = r->right;
                int loc2 = loc + strlen(r->word) + 3;
                drawRightEdge(row, loc2);
                row += 3;
                loc2 += 3;
                drawRightWord(row, loc2, rr);

                if (rr->left != NULL)
                {
                    struct node* rrl  = rr->left;         // poziom trzeci
                    int loc3 = loc2 + 3;
                    row = 11;
                    mvprintw(row, loc3, "/");
                    mvprintw(row + 1, loc3 - 1, "/");
                    row += 2;
                    loc3 -= 2;
                    drawLeftWord(row, loc3, rrl);

                    if (rrl->left != NULL)
                    {
                        row = 15;                              // poziom czwarty
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
                        row = 15;                              // poziom czwarty
                        int loc4 = loc3;
                        mvprintw(row, loc4, "\\");
                        mvprintw(row + 1, loc4 + 1, "\\");
                        mvprintw(row + 2, loc4 + 2, "\\");
                        mvprintw(row + 3, loc4 + 3, "\\");
                        loc4 += 3;
                        row += 4;
                        struct node* rrlr  = rrl->right;
                        drawRightWord(row, loc4, rrlr);
                    }
                }

                if (rr->right != NULL)
                {
                    row = 11;
                    int loc3 = loc2 + 5;                               // trzeci poziom
                    struct node* rrr  = rr->right;
                    mvprintw(row, loc3, "\\");
                    mvprintw(row + 1, loc3 + 1, "\\");
                    row += 2;
                    loc3++;
                    drawRightWord(row, loc3, rrr);

                    if (rrr->left != NULL)
                    {
                            row = 15;                              // poziom czwarty
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
                            row = 15;                              // poziom czwarty
                            int loc4 = loc3 + 3;
                            mvprintw(row, loc4, "\\");
                            mvprintw(row + 1, loc4 + 1, "\\");
                            mvprintw(row + 2, loc4 + 2, "\\");
                            mvprintw(row + 3, loc4 + 3, "\\");
                            loc4 += 3;
                            row += 4;
                            struct node* rrrr  = rrr->right;
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
    char* arity=(char*)malloc(sizeof(char));
    mvprintw(locr, locc - strlen(left->word)/2, left->word);
    itoa(left->arity, arity, 10);
    mvprintw(locr + 1, locc, arity);
}

void drawRightWord(int locr, int locc, struct node* right)
{
    char* arity=(char*)malloc(sizeof(char));
    mvprintw(locr, locc, right->word);
    itoa(right->arity, arity, 10);
    mvprintw(locr + 1, locc + strlen(right->word) - 2 , arity);
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
    do {
        printf("\n");
        printf("MENU\n");
    printf("0 - zakonczenie\n");
    printf("1 - dopisanie\n");
    printf("2 - usuniecie\n");
    printf("3 - wyswietlenie\n");
    printf("4 - rysowanie\n");
    printf("Wybierz opcje: ");
    char choice[1];
    scanf("%s", choice);
    printf("\n");
    if (strcmp(choice, "0") == 0){
        printf("Do widzenia!\n");
        break;
    }
    else if (strcmp(choice, "1") == 0){
        char *newWord;
        newWord = (char *)calloc(SIZE+1, sizeof(char));
    if (newWord==NULL){
        printf("Nie ma miejsca na nowe slowo\n");
    }
    else {
        printf("Podaj slowo: ");
        scanf("%s", newWord);
        newWord[SIZE]='\0';
        add(newWord);
        }
    }
    else if (strcmp(choice, "2") == 0){
        if (root == NULL){
            printf("Lista jest pusta\n");
        }
        else {
            char newWord[SIZE];
            printf("Podaj slowo, ktore chcesz usunac: ");
            scanf("%s", newWord);
            rmv(newWord);
        }
    }
    else if (strcmp(choice, "3") == 0)
    {
        display(root);
    }
    else if (strcmp(choice, "4") == 0)
    {
        drawTree();
    }
    else {
        printf("Nie ma takiego wyboru\n");
    }
    } while (1);

    getchar();
    return 0;
}
