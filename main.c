#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[10];
    float custo;
    float retorno;
} acao;

int main() {

    int n;
    printf("Quantas acoes: ");
    scanf("%d", &n);

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