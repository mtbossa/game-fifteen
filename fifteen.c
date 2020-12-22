	/***************************************************************************
 * fifteen.c
 *
 * CC50
 * Pset 3
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
void initCorrect(void);
void init(void);
void draw(void);
bool getTile(int *ptr_tile);
bool move(int tile);
bool won(void);


int
main(int argc, char *argv[]) {
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
   
	// initialize the correct board;
	initCorrect();

    // initialize the board
    init();

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
		int *ptr_tile = &tile;
		if(!getTile(ptr_tile)) {
			printf("Digite um numero valido.");
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

void initCorrect(void) {	
	//comecar pecas do 1 até d - 1, ir aumentando as pecas. Criar um array correctBoard com as peças no lugar, comparar o array atual com o correctBoard
	int pecas = (d * d) - 1;
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

void init(void) {	
	int pecas = (d * d) - 1;
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
    		if(board[i][j] < 10 && board[i][j] != 0) {
    			printf("%2d ", board[i][j]);	
    		} else if(board[i][j] != 0) {
    			printf("%d ", board[i][j]);
    		}   
    		if(board[i][j] == 0) {
        		printf("\033[37;5m _ \033[0m");
        	}      	
         	if(j == d - 1) {
            		printf("\n");
        	}
        }
    }
}

/* 	
 *  If invalid input, return false. Else, returns true.
 */

bool getTile(int *ptr_tile) {

	char userInput[64];
	int num = 0;
	char temp;
	size_t length = 0;			
		
	printf("Tile to move: ");		
	fgets(userInput, 63, stdin);

	length = strlen(userInput);
		
	if(length < 2 || length > 3) {
		return false;			
	}
	
	length--;
	int flag = 0;

	for(int index = 0; index < length; ++index) {
		if(userInput[index] < '0' || userInput[index] > '9') {
			flag = 1;
			break;
		}
	}
		
	if(flag) {
		return false;			
	}
		
	if(sscanf(userInput, "%d", &num) != 1) {
		return false;
	}
				
	if(num < 1 || num > 99) {
		return false;
	}

	*ptr_tile = num;
	return true;
}

/* 	
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */

bool move(int tile) {
	int linha_0 = 0;
	int coluna_0 = 0;

	for(int i = 0; i < d; i++) {
		if(board[linha_0][coluna_0] == 0) {
			break;
		} else {
			linha_0 = i;
			for (int j = 0; j < d; j++) {
				if(board[linha_0][coluna_0] == 0) {
					break;
				} else {
					coluna_0 = j;
				}
			}
		}		
	}
	
	if(tile == board[linha_0 - 1][coluna_0]) { // para cima
		board[linha_0][coluna_0] = tile;
		linha_0 = linha_0 - 1;
		board[linha_0][coluna_0] = 0;
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

	for(int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {			
			if(correctBoard[i][j] != board[i][j]) {
				return false;
			}
		}	
	}
   return true;
}