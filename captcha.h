//Daniel De Marco Fucci

#ifndef _CAPTCHA_
#define _CAPTCHA_


#include <stdlib.h>
#include <stdbool.h>
//Definindo tipo de retornos
#define SUCESS 0
#define FILE_ERROR -1
#define LOAD_ERROR -2
//MASKS_NUMBER define numero de mascaras
#define MASKS_NUMBER 10
//MIN_PRECISION define qual o numero minimo de elementos '1' no quadrante (10x10) para que ele seja considerado de fato um quadrante valido
#define MIN_PRECISION 92
//Define qual o numero maximo de zeros permitidos acima do elemento '1' para que ele seja considerado quadrante
#define MAX_ZEROS_ABOVE 5
//Declaracao do modelo em que as matrizes sao armazenadas na memoria
struct matrix_pgm {
    int line_number;
    int column_number;
    int max_gray;
    char* img_type;
    char** matrix;
};
typedef struct matrix_pgm MATRIX_PGM;
//Declaracao do modelo de vetor resposta
struct vetor_resp {
    int* vetor;
    int counter;
};
typedef struct vetor_resp VETOR_RESP;
//Carrega matriz do arquivo na memoria. Usada para carregar o captcha a ser quebrado e as mascaras.
MATRIX_PGM* matrixReader(char*, int*);
//freeMatrix libera as matrizes alocadas na memoria
void freeMatrix(MATRIX_PGM*);
//loadMasks carrega todas as mascaras na memoria.
MATRIX_PGM** loadMasks(int);
//Imprime a matriz
void printMatrix(MATRIX_PGM*);
//Corre a matriz e procura por '1', se encontrar, chama quadrantValidator() e whichNumber()
void findNumbers(MATRIX_PGM*, MATRIX_PGM**, VETOR_RESP*);
//Ao encontrar um elemento '1' no arquivo, o quadrantValidator confere se de fato se trata de um quadrante de um numero, ou apenas um elemento '1' chuviscado pelo modelo sal e pimenta
bool quadrantValidator(MATRIX_PGM*, int, int);
//Compara o quadrante encontrado com todas as mascaras a partir do método de convolucao e devolve um vetor com os numeros existentes no CAPTCHA
void whichNumber(MATRIX_PGM*, MATRIX_PGM**, int , int, VETOR_RESP*);
#endif