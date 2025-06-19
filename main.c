#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char nome[10];
    float custo;
    float retorno;
} acao;

int investimento (acao *array, int n, float capital, float *melhorCusto, float *maiorRetorno){
    int  totalcomb = pow(2, n);
    int melhorcomb;
    *melhorCusto = 0;
    *maiorRetorno = -1;

    for(int comb = 1; comb < totalcomb; comb++){
        float custototal = 0;
        float retornototal = 0;

        for(int j = 0; j < n; j++){
            if(comb &(1 << j)){
                custototal += array[j].custo;
                retornototal += array[j].retorno;

            }
        }
        if (custototal<=capital && retornototal > *maiorRetorno)
        {
            *maiorRetorno = retornototal;
            *melhorCusto =  custototal;
            melhorcomb = comb;
        }
    }
    return melhorcomb;
}

int main() {

    int n;
    float capital;
    printf("Quantas acoes: ");
    scanf("%d", &n);
    printf("Qual o capital inicial");
    scanf("%d", &capital);

    acao *array = malloc(n*sizeof(acao));

    for(int i = 0; i < n; i++){
        printf("nome: ");
        scanf("%s", array[i].nome);
         printf("custo: ");
        scanf("%f", &array[i].custo);
         printf("retorno: ");
        scanf("%f", &array[i].retorno);
    }
    printf("========================== Acoes ==========================\n");
    printf("-----------------------------------------------------------\n");
    printf("%-20s | %-10s | %-10s\n", "Nome", "Custo", "Retorno");
    for(int i = 0; i< n; i++){
        printf("%-20s | %10.2f | Retorno: %9.2f%%\n", array[i].nome, array[i].custo, array[i].retorno);
    }
    free(array);
    return 0;
}