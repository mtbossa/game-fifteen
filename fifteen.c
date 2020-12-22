	/***************************************************************************
 * fifteen.c
 *
 * CC50
 * Pset 3 - Mateus Ribeiro Bossa
 *
 * Implements The Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 ***************************************************************************/
 
#define _XOPEN_SOURCE 500

#include <string.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// constants
#define DIM_MIN 3
#define DIM_MAX 9

// correct board
int correctBoard[DIM_MAX][DIM_MAX];

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void initCorrect(int pecas);
void init(int pecas);
void draw(void);
bool getTile(int *ptr_tile, int pecas);
bool move(int tile);
bool won(void);


int main(int argc, char *argv[]) {
    // greet user with instructions
    greet();

    // ensure proper usage
    if (argc != 2) {
        printf("Usage: %s d\n", argv[0]);
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX) { // se d < que 3 OU d > 9
        printf("Board must be between %d x %d and %d x %d, inclusive.\n",
         DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

	/* 
     * Declaração e cáculdo da quantidade de peças. Não utilizei variável global, pois em uma função existe pecas--, que vai diminuir a quantidade na variável em si,
     * então tive que declarar aqui e passar esse valor para as funções que a utiliza, pois assim será utilizado somente o valor de pecas, e o seue valor será utilizado
     * e modificado somente dentro das funções.
     */
	int pecas = (d * d) - 1; 
   
	// initialize the correct board
	initCorrect(pecas);

    // initialize the board
    init(pecas);

    // accept moves until game is won
    while (true) {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // check for win
        if (won()) {
            printf("ftw!\n");
            break;
        }

        // prompt for move
		int tile;
		int *ptr_tile = &tile; // ponteiro com endereço da int tile, utilizado na funcao getTile(int *ptr_tile)
		
		if(!getTile(ptr_tile, pecas)) { // se input do usuario nao for um inteiro valido volta para o loop
			printf("Digite um numero valido.\n");			
			tile = 0;
			usleep(500000);
		}		

        // move if possible, else report illegality
        if (!move(tile)) {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}


/*
 * Clears screen using ANSI escape sequences.
 */

void clear(void) {
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}


/*
 * Greets player.
 */

void greet(void) {
    clear();
    printf("WELCOME TO THE GAME OF FIFTEEN\n");
    usleep(5000000);
}


/*
 * Initializes the game's board with the correct array  
 */

void initCorrect(int pecas) {	
	//comecar pecas do 1 até d - 1, ir aumentando as pecas. Criar um array correctBoard com as peças no lugar, comparar o array atual com o correctBoard
    int pecas_certas = 1;
    for(int i = 0; i < d; i++) {
    	for(int j = 0; j < d; j++) {
            if(pecas_certas <= pecas) {
    		    correctBoard[i][j] = pecas_certas;
    		    pecas_certas++;
            }	 
    	}
    }
}

/*
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */

void init(int pecas) {		
    // se o d for par, pecas sera impar, entao o 1 e 2 devem ser trocados
    if(d % 2 == 0) {
    	for(int i = 0; i < d; i++) {
    		for(int j = 0; j < d; j++) { 
    			if(pecas > 2) {
    				board[i][j] = pecas;
    				pecas--;
    			} else if(pecas == 2) {
    				board[i][j] = pecas - 1;
    				pecas--;
                } else if(pecas == 1) {
    				board[i][j] = pecas + 1;
                    pecas--;
                } else if(pecas == 0) {
                    board[i][j] = pecas;
                }
    		}
    	}
    }	

    //se o d for impar, tera numero par de pecas, entao o 1 e 2 ficam certos
    if(d % 2 != 0) {
    	for(int i = 0; i < d; i++) {
    		for(int j = 0; j < d; j++) { 
    			board[i][j] = pecas;
    			pecas--;	 
    		}
    	}
    }
}


/* 
 * Prints the board in its current state.
 */

void draw(void) {
	for(int i = 0; i < d; i++) {
    	for(int j = 0; j < d; j++) {
			// se o valor atual do array for menor que 10 e diferente de 0, utiliza 2 casas decimais e um espaço (para deixar alinhado com os numeros de 2 digitos)
    		if(board[i][j] < 10 && board[i][j] != 0) {
    			printf("%2d ", board[i][j]);	
    		} else if(board[i][j] != 0) { // se for maior ou igual a 10 e diferente de 0, printa o numero com um espaço
    			printf("%d ", board[i][j]);
    		}   
    		// printa o valor 0 como um '_' piscado
			if(board[i][j] == 0) {
        		printf("\033[37;5m _ \033[0m");
        	}      	
			/* 
			 * quando j chegar no valor de d - 1, print uma nova linha, ou seja, o j só vai chegar no d - 1 uma vez por i, então, será printada uma nova linha por i, que
			 * são as linhas, e o j = d - 1 é a última posição da coluna.
			 */
         	if(j == d - 1) { 
            		printf("\n");
        	}
        }
    }
}

/* 	
 *  If invalid input, return false. Else, return true. Utiliza fgets, ou seja, recebe uma string mas faz as verificações, depois transforma para int e coloca no ptr_tile
 */

bool getTile(int *ptr_tile, int pecas) {
	char userInput[64];	// array para guardar o input
	int num = 0;
	size_t length = 0; // pode ter qualquer valor, por isso size_t	
	
	printf("Tile to move: ");		
	fgets(userInput, 63, stdin);
	length = strlen(userInput);			
		
	if(length < 2 || length > 3) { // 2 pois é o valor + \0. Ex1.: '1''\0' = 2 char; Ex2.: '1''2''\0' = 3 char;	Ex3.: '1''2''3''\0' = 4 char;
		return false;	
	}
	
	length--; // diminuir o length para retirar o local '\0', para assim ser utilizado no loop for. Senão ele iria verificar o '\0'	

	for(int index = 0; index < length; ++index) {
		if(userInput[index] < '0' || userInput[index] > '9') { // se o valor na posicao atual do input for menor do que o caractere '0' ou maior que o caractere '9'
			return false;			
		}
	}	

	if(sscanf(userInput, "%d", &num) != 1) { // como para chegar aqui ja passou por todas etapas, so tem numeros no array. sscanf atribui o valor na variavel num.
		return false; // retorna falso caso tenha algum erro no input
	}	
				
	if(num < 1 || num > pecas) { // agora num recebeu o valor passado por sscanf
		return false; // retorna falso caso o input tenha sido menor do que 1 ou maior do que a quantidade de peças disponíveis
	}

	*ptr_tile = num; // coloca num, ou seja, o input no valor da variável para qual o ponteiro *ptr_tile aponta, ou seja, para int tile
	return true; // retorna true;
	
}

/* 	
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */

bool move(int tile) {

	// definicao de variaveis para controlar o local atual do valor 0
	int linha_0 = 0;
	int coluna_0 = 0;
	//loop para descobrir em que posicao está o 0
	for(int i = 0; i < d; i++) {
		if(board[linha_0][coluna_0] == 0) { // se a posição do board nas variaveis for 0, sai do loop
			break;
		} else {
			linha_0 = i; // senão, a linha_0 se tornar o valor de i
			for (int j = 0; j < d; j++) {
				if(board[linha_0][coluna_0] == 0) { // se a posição do board nas variaveis for 0, sai do loop
					break;
				} else {
					coluna_0 = j; // senão, a coluna_0 se torna o valor de j
				}
			}
		}		
	}
	
	if(tile == board[linha_0 - 1][coluna_0]) { // se o valor de tile for o valor de cima do 0, ou seja, linha_0 (posicao do 0 na linha) - 1, na linha superior
		board[linha_0][coluna_0] = tile; // a posicao board[linha_0][coluna_0] se torna o valor de tile
		linha_0 = linha_0 - 1; // a linha_0 agora vai seu valor reduzido em 1
		board[linha_0][coluna_0] = 0; // board[linha_0 (atualizado com o - 1)][coluna_0] vai ter seu valor como 0
		return true;
	}
	if(tile == board[linha_0 + 1][coluna_0]) { // para baixo
		board[linha_0][coluna_0] = tile;
		linha_0 = linha_0 + 1;
		board[linha_0][coluna_0] = 0;
		return true;
	}
	if(tile == board[linha_0][coluna_0 - 1]) { // para esquerda
		board[linha_0][coluna_0] = tile;
		coluna_0 = coluna_0 - 1;
		board[linha_0][coluna_0] = 0;
		return true;
	}
	if(tile == board[linha_0][coluna_0 + 1]) { // para direta		
		board[linha_0][coluna_0] = tile;
		coluna_0 = coluna_0 + 1;
		board[linha_0][coluna_0] = 0;
		return true;
	}
	return false;
}

/*
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */

bool won(void) {
	/* loop para verificar todas as posições de ambos os arrays. Caso qualquer valor na mesma posição esteja diferente, retorna false. Só irá retornar true se sair do loop,
	e só irá sair de loop quando passar por todos os elementos do array e não retornar false */
	for(int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {			
			if(correctBoard[i][j] != board[i][j]) {
				return false;
			}
		}	
	}
   return true;
}