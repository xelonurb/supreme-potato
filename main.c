#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[10];
    float custo;
    float retorno;
} acao;

int investimento (acao *array, int n, float capital, float *melhorCusto, float *maiorRetorno){
    int totalcomb = 1 << n;;
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

void exibirResultado(acao *array, int n, float capital, int melhorcomb, float melhorCusto, float maiorRetorno ){

    printf("\n----------------------------------------\n");
    printf("Carteira de Investimentos Otimizada\n");
    printf("----------------------------------------\n");
    printf("Capital Disponível: R$ %.2f\n\n", capital);
    printf("Ações a Comprar:\n");

    for(int j = 0; j < n; j++){
        if(melhorcomb & (1<< j)){
            printf("- %s (Custo: R$ %.2f, Retorno: %.2f%%)\n", array[j].nome, array[j].custo, array[j].retorno);

        }
    }
    printf("\nResumo da Carteira:\n");
    printf("- Custo Total: R$ %.2f\n", melhorCusto);
    printf("- Retorno Máximo Esperado: %.2f%%\n", maiorRetorno);
    printf("----------------------------------------\n");


}
int main() {

    int n;
    float capital;
    printf("Quantas acoes: ");
    scanf("%d", &n);
    printf("Qual o capital inicial: ");
    scanf("%f", &capital);

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

    float melhorCusto, maiorRetorno;
    int melhorcomb = investimento(array, n, capital, &melhorCusto, &maiorRetorno);
    exibirResultado(array, n, capital, melhorcomb, melhorCusto, maiorRetorno);

    free(array);
    return 0;
}