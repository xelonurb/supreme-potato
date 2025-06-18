#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[10];
    float custo;
    float retorno;
} acao;

int main() {

    int n;
    printf("Quantas acoes");
    scanf("%d", &n);

    acao array[n];

    for(int i = 0; i < n; i++){
        printf("nome:");
        scanf("%s", array[i].nome);
         printf("custo:");
        scanf("%s", array[i].custo);
         printf("retorno:");
        scanf("%s", array[i].retorno);
    

    }
    return 0;
}