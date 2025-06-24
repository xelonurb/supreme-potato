#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[30];
    int custo;
    float retorno;
} acao;

//funcao metodo mascara de bit, muito lenta
int investimento (acao *array, int n, int capital, int *melhorCusto, float *maiorRetorno){
    int totalcomb = 1 << n;
    int melhorcomb;
    *melhorCusto = 0;
    *maiorRetorno = -1;

    for(int comb = 1; comb < totalcomb; comb++){
        int custototal = 0;
        float retornototal = 0;

        for(int j = 0; j < n; j++){
            if(comb & (1 << j)){
                custototal += array[j].custo;
                retornototal += array[j].retorno;
            }
        }
        if (custototal <= capital && retornototal > *maiorRetorno){
            *maiorRetorno = retornototal;
            *melhorCusto = custototal;
            melhorcomb = comb;
        }
    }
    return melhorcomb;
}

float mochilaknapsack(acao *array, int n, int capital, int *selecionadas){
    float **dp = malloc((n + 1) * sizeof(float *));
    for(int i = 0; i <= n; i++){
        dp[i] = calloc((capital + 1), sizeof(float));
    }
    for(int i = 1; i <= n; i++){
        for(int j = 0; j <= capital; j++){
            if(array[i-1].custo > j){
                dp[i][j] = dp[i - 1][j];
            }
            else {
                float naoPega = dp[i - 1][j];
                float pega = dp[i - 1][j - array[i - 1].custo] + array[i-1].retorno;
                dp[i][j] = (pega > naoPega) ? pega : naoPega;
            }
        }
    }
    int restante = capital;
    for(int i = n; i > 0; i--){
        if(dp[i][restante] != dp[i - 1][restante]){
            selecionadas[i - 1] = 1;
            restante -= array[i - 1].custo;
        }else{
            selecionadas[i - 1] = 0;
        }
    }
    float resultado = dp[n][capital];
    for(int i = 0; i <= n; i++){
        free(dp[i]);
    }
    free(dp);
    return resultado;
}

acao *lerArquivo(const char *nomeArquivo, int *n, int *capital){
    FILE *file = fopen(nomeArquivo, "r");
    if(file == NULL){
        printf("erro ao abrir arquivo");
        exit(1);
    }
    char linha[150];
    *n = 0;
    int capacidade = 50;
    acao *array = malloc(capacidade * sizeof(acao));

    while (fgets(linha, sizeof(linha), file)){
        if(strncmp(linha, "CAPITAL_DISPONIVEL_R$", 22) == 0){
            float temp;
            sscanf(linha, "CAPITAL_DISPONIVEL_R$: %f", &temp);
            *capital = (int)(temp * 100 + 0.5);
            break;
        }
    }
    while (fgets(linha, sizeof(linha), file)){
        if (strncmp(linha, "ACOES:", 6) == 0){
            break;
        }
    }
    char id[5], nomeTemp[50];
    float retorno, custo;

    while (fgets(linha, sizeof(linha), file)){
        if(sscanf(linha,"%s %f %f %s", id, &custo, &retorno, nomeTemp) == 4){
            if(*n >= capacidade){
                capacidade *= 2;
                array = realloc(array, capacidade * sizeof(acao));
            }
            snprintf(array[*n].nome, sizeof(array[*n].nome), "%s", nomeTemp);
            array[*n].custo = (int)(custo * 100 + 0.5);
            array[*n].retorno = retorno;
            (*n)++;
        }
    }   
    fclose(file);
    return array;
}

void exibirResultado(acao *array, int n, int capital, int melhorcomb, int melhorCusto, float maiorRetorno ){
    printf("\n----------------------------------------\n");
    printf("Carteira de Investimentos Otimizada\n");
    printf("----------------------------------------\n");
    printf("Capital Disponível: R$ %.2f\n\n", capital / 100.0);
    printf("Ações a Comprar:\n");

    for(int j = 0; j < n; j++){
        if(melhorcomb & (1 << j)){
            printf("- %s (Custo: R$ %.2f, Retorno: %.2f%%)\n", array[j].nome, array[j].custo /100.0, array[j].retorno);
        }
    }
    printf("\nResumo da Carteira:\n");
    printf("- Custo Total: R$ %.2f\n", melhorCusto /100.0);
    printf("- Retorno Máximo Esperado: %.2f%%\n", maiorRetorno);
    printf("----------------------------------------\n");
}

void exibirResultadoPD(acao *array, int n, int capital, int *selecionadas, float retornoMaximo){
    printf("\n----------------------------------------\n");
    printf("Carteira de Investimentos Otimizada\n");
    printf("----------------------------------------\n");
    printf("Capital Disponível: R$ %.2f\n\n", capital / 100.0);
    printf("Ações a Comprar:\n");

    int custoTotal = 0;
    for(int i = 0; i < n; i++){
        if(selecionadas[i]){
            printf("- %s (Custo: R$ %.2f, Retorno: %.2f%%)\n", array[i].nome, array[i].custo / 100.0, array[i].retorno);
            custoTotal += array[i].custo;
        }
    }

    printf("\nResumo da Carteira:\n");
    printf("- Custo Total: R$ %.2f\n", custoTotal /100.0);
    printf("- Retorno Máximo Esperado: %.2f%%\n", retornoMaximo);
    printf("----------------------------------------\n");
}

int main(){
    int n, capital;
    char nomeArquivo[100];

    //acao *array = malloc(n*sizeof(acao));

    printf("Digite o nome do arquivo: ");
    scanf("%s", nomeArquivo);

    acao *array = lerArquivo(nomeArquivo, &n, &capital);

    printf("Foram lidas %d ações. Capital disponível: R$ %.2f\n", n, capital /100.0);
    //por ler no arquivo comentei a parte de digitar manualmente as acoes
/*
    for(int i = 0; i < n; i++){
        printf("nome: ");
        scanf("%s", array[i].nome);
        printf("custo: ");
        scanf("%d", &array[i].custo);
        printf("retorno: ");
        scanf("%f", &array[i].retorno);
    }
*/
    printf("========================== Acoes ==========================\n");
    printf("-----------------------------------------------------------\n");
    printf("%-20s | %-10s | %-10s\n", "Nome", "Custo", "Retorno");
    for(int i = 0; i < n; i++){
        printf("%-20s | %10.2f | Retorno: %9.2f%%\n", array[i].nome, array[i].custo / 100.0, array[i].retorno);
    }

    /*
    int melhorCusto;
    float maiorRetorno;
    int melhorcomb = investimento(array, n, capital, &melhorCusto, &maiorRetorno);
    exibirResultado(array, n, capital, melhorcomb, melhorCusto, maiorRetorno);
    */

    //com a mochila de knapsack
    int *selecionadas = calloc(n, sizeof(int));
    float retornoMaximo = mochilaknapsack(array, n, capital, selecionadas);
    exibirResultadoPD(array, n, capital, selecionadas, retornoMaximo);

    free(array);
    free(selecionadas);
    return 0;
}
