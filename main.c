//Daniel De Marco Fucci BSI 2019
//N USP: 11218639

#include <stdlib.h>
#include <captcha.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char** argv) {
    //Lendo entrada do usuario
    char* fileName = (char*) malloc (sizeof(char) * 15);
    scanf("%s", fileName);


    //Alocando memoria
    int error = 0;
    MATRIX_PGM* captcha = matrixReader(fileName, &error);
    if (captcha == NULL) return FILE_ERROR;
    MATRIX_PGM** masksMatrix = loadMasks(MASKS_NUMBER);
    VETOR_RESP* vetorResp = (VETOR_RESP*) calloc(1, sizeof(VETOR_RESP));

    
    //Executando
    findNumbers(captcha, masksMatrix, vetorResp);
    for (int i=vetorResp->counter - 1; i>=0; i--) {
        printf("%d", vetorResp->vetor[i]);
    }
    printf("\n");

    
    //Liberando memoria alocada
    freeMatrix(captcha);
    for (int i=0; i<MASKS_NUMBER; i++) {
        freeMatrix(masksMatrix[i]);
    }
    free(masksMatrix);
    free(vetorResp->vetor);
    free(vetorResp);    
    free(fileName);
    
    
    return 0;
}