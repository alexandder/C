#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

void rmv(char givenWord[]){
	
}

void display(){
	
}


int main() {
	
	do {
	printf("\n");
	printf("MENU\n");
	printf("0 - zakonczenie\n");
	printf("1 - dopisanie\n");
	printf("2 - usuniecie\n");
	printf("3 - wyswietlenie\n");
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
			add(root, newWord);							
		}
	}
	else if (strcmp(choice, "2") == 0){
		if (start == NULL){
		 	printf("Lista jest pusta\n");
		}
		else {
			char newWord[SIZE];
			printf("Podaj slowo, ktore chcesz usunac: ");
			scanf("%s", newWord);
			rmv(newWord);
		}
	}
	else if (strcmp(choice, "3") == 0){
		display();
	}
	else {
		printf("Nie ma takiego wyboru\n");
	}
} while (1);

getchar();
return 0;
}
