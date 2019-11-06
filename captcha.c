//Daniel De Marco Fucci BSI 2019
//N USP: 11218639

#include <stdlib.h>
#include <string.h>
#include <captcha.h>
#include <stdio.h>
#include <stdbool.h>

MATRIX_PGM* matrixReader(char* fileName, int* error) {
    MATRIX_PGM* matrixFromFile = (MATRIX_PGM*) malloc(sizeof(MATRIX_PGM));
    char aux;
    int lineIter = 0;
    int columnIter = 0;
    FILE *arq =fopen (fileName, "r");
    if (arq == NULL) {
        *error=FILE_ERROR;
    }
    else {
        matrixFromFile->img_type =(char*) calloc(3, sizeof(char));
        fgets(matrixFromFile->img_type, 3, arq);
        aux = fgetc(arq);
        fscanf(arq, "%d %d", &matrixFromFile->column_number, &matrixFromFile->line_number);
        aux = fgetc(arq);
        fscanf(arq, " %d", &matrixFromFile->max_gray);
        aux = fgetc(arq);
        matrixFromFile->matrix = (char**) malloc(sizeof(char*) * matrixFromFile->line_number);
        for (int i=0; i<matrixFromFile->line_number; i++) {
            matrixFromFile->matrix[i] = (char*) malloc(sizeof(char) * matrixFromFile->column_number);
        }
        while(!feof(arq)) {
            fscanf (arq, "%c", &aux);
            if(aux == '\n') {
                lineIter++;
                columnIter=0;
                continue;
            }
            else if (aux != ' ') {
                matrixFromFile->matrix[lineIter][columnIter] = aux;
                columnIter++;
            }
        }
        fclose(arq);
        *error = SUCESS;
        return matrixFromFile;
    }
}

void findNumbers(MATRIX_PGM* matrixCaptcha, MATRIX_PGM** masks, VETOR_RESP* vetorResposta) {
    for (int j=(matrixCaptcha->column_number)-1; j>=0; j--) {
        for (int i=(matrixCaptcha->line_number)-1; i>=0; i--) {
            if (matrixCaptcha->matrix[i][j] == '1') {
                if (quadrantValidator(matrixCaptcha, i, j)) {
                    vetorResposta->counter++; 
                    whichNumber(matrixCaptcha, masks, i, j, vetorResposta);
                    //Marcador de colunas recebe - 30
                    j -= 30;
                    i=matrixCaptcha->line_number - 1 ;
                }
            }
        }
    }
}
void whichNumber(MATRIX_PGM* matrixCaptcha, MATRIX_PGM** masks, int iFound, int jFound, VETOR_RESP* vetorResposta) {
    vetorResposta->vetor = (int*) realloc(vetorResposta->vetor, vetorResposta->counter);
    int maxSum=0;
    int number=-1;
    int sum = 0;
    int j = 0;
    int i = iFound - (masks[0]->line_number-1);
    for (int n=0; n<MASKS_NUMBER; n++) {
        sum = 0;
        if (n == 1) {
            j = (jFound - (masks[n] ->column_number - 11)) ;
        }
        else {
            j = jFound - (masks[n] ->column_number-1);
        }
        if (i>=0 && j>=0) {
            for (int k=0; k<masks[n]->line_number; k++) {
                for (int l=0; l<masks[n]->column_number; l++) {
                    if ((matrixCaptcha->matrix[i+k][j+l] == '1' && masks[n]->matrix[k][l] == '1') || (matrixCaptcha->matrix[i+k][j+l] == '0' && masks[n]->matrix[k][l] == '0')) {
                        sum++;
                    }
                }
            }
            if (sum > maxSum) {
                number = n;
                maxSum=sum;
            }
        }
    }
    vetorResposta->vetor[vetorResposta->counter - 1] = number;
}

bool quadrantValidator(MATRIX_PGM* matrix, int lineStarter, int columnStarter) {
    int aux = 0;
    int zerosAbove=0;
    for (int i=lineStarter; i>lineStarter-10 && i>=0; i--) {
        for (int j=columnStarter; j>columnStarter-10 && j>=0; j--) {
            if (matrix->matrix[i][j] == '1') {
                aux++;
            }
        }
    }
    if (aux >= MIN_PRECISION) {
        for (int i = lineStarter - 10; i<lineStarter; i++) {
            if (matrix->matrix[i][columnStarter] == '0') zerosAbove++;
        }
        if (zerosAbove <= MAX_ZEROS_ABOVE) return true;
    }
    return false;
}

void freeMatrix(MATRIX_PGM* matrix) {
    for (int i=0; i<matrix->line_number; i++) {
        free(matrix->matrix[i]);
    } 
    free(matrix->matrix);
    free(matrix->img_type);
    free(matrix);
}

MATRIX_PGM** loadMasks(int numberOfMasks) {
    int error=0;
    //Criando vetor de matrizes, uma matriz para cada mascara diferente.
    MATRIX_PGM** matrixMask = (MATRIX_PGM**) malloc(sizeof(MATRIX_PGM*) * MASKS_NUMBER);
    char* fileName = (char*) malloc(sizeof(char) * 6);
    char* iString= (char*) malloc(sizeof(char) * 2);
    for (int i=0; i<numberOfMasks; i++) {
        sprintf(iString, "%d", i);
        strcpy(fileName, iString);
        strcat(fileName, ".pgm");
        matrixMask[i] = matrixReader(fileName, &error);
    }
    return matrixMask;

}

void printMatrix(MATRIX_PGM* matrix) {
    printf ("%s\n", matrix->img_type);
    printf ("%d %d\n", matrix->column_number, matrix->line_number);
    printf ("%d\n", matrix->max_gray);
    for (int i=0; i<matrix->line_number; i++) {
        for (int j=0; j<matrix->column_number; j++) {
            printf("%c ", matrix->matrix[i][j]);
        }
        printf ("\n");
    }
}