#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[10];
    float custo;
    float retorno; // em porcentagem
} acao;

int main() {

    int n;
    printf("Quantas acoes");
    scanf("%d", &n);

    acao array[n];
    return 0;
}