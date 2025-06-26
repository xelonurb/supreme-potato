#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[30];
    int custo;      // custo em reais inteiros
    float retorno;
} acao;

// função método máscara de bit (muito lenta para muitos itens)
int investimento (acao *array, int n, int capital, int *melhorCusto, float *maiorRetorno){
    int totalcomb = 1 << n;
    int melhorcomb = 0;
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

// mochila dinâmica (programação dinâmica)
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
        } else {
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
        perror("erro ao abrir arquivo");
        exit(1);
    }
    char linha[150];
    *n = 0;
    int capacidade = 50;
    acao *array = malloc(capacidade * sizeof(acao));

    // Ler capital
    while (fgets(linha, sizeof(linha), file)){
        if(strncmp(linha, "CAPITAL_DISPONIVEL_R$:", strlen("CAPITAL_DISPONIVEL_R$:")) == 0){
            float temp;
            sscanf(linha, "CAPITAL_DISPONIVEL_R$: %f", &temp);
            *capital = (int)(temp + 0.5);  // capital em reais inteiros
            break;
        }
    }
    // Ler até encontrar "ACOES:"
    while (fgets(linha, sizeof(linha), file)){
        if (strncmp(linha, "ACOES:", 6) == 0){
            break;
        }
    }
    char id[10], nomeTemp[50];
    int custoint;
    float retornofloat;

    // Ler ações
    while (fgets(linha, sizeof(linha), file)){
        if(sscanf(linha, "%s %d %f %[^\n]", id, &custoint, &retornofloat, nomeTemp) == 4){
            if(*n >= capacidade){
                capacidade *= 2;
                array = realloc(array, capacidade * sizeof(acao));
            }
            // Copia segura do nome da ação
            strncpy(array[*n].nome, nomeTemp, sizeof(array[*n].nome) - 1);
            array[*n].nome[sizeof(array[*n].nome) - 1] = '\0';  // garante terminação nula
            array[*n].custo = custoint;
            array[*n].retorno = retornofloat;
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
    printf("Capital Disponível: R$ %d\n\n", capital);
    printf("Ações a Comprar:\n");

    for(int j = 0; j < n; j++){
        if(melhorcomb & (1 << j)){
            printf("- %s (Custo: R$ %d, Retorno: %.2f%%)\n", array[j].nome, array[j].custo, array[j].retorno);
        }
    }
    printf("\nResumo da Carteira:\n");
    printf("- Custo Total: R$ %d\n", melhorCusto);
    printf("- Retorno Máximo Esperado: %.2f%%\n", maiorRetorno);
    printf("----------------------------------------\n");
}

void exibirResultadoPD(acao *array, int n, int capital, int *selecionadas, float retornoMaximo){
    printf("\n----------------------------------------\n");
    printf("Carteira de Investimentos Otimizada\n");
    printf("----------------------------------------\n");
    printf("Capital Disponível: R$ %d\n\n", capital);
    printf("Ações a Comprar:\n");

    int custoTotal = 0;
    for(int i = 0; i < n; i++){
        if(selecionadas[i]){
            printf("- %s (Custo: R$ %d, Retorno: %.2f%%)\n", array[i].nome, array[i].custo, array[i].retorno);
            custoTotal += array[i].custo;
        }
    }

    printf("\nResumo da Carteira:\n");
    printf("- Custo Total: R$ %d\n", custoTotal);
    printf("- Retorno Máximo Esperado: %.2f%%\n", retornoMaximo);
    printf("----------------------------------------\n");
}

int main(){
    int n, capital;
    char nomeArquivo[100];

    printf("Digite o nome do arquivo: ");
    scanf("%s", nomeArquivo);

    acao *array = lerArquivo(nomeArquivo, &n, &capital);

    if(n == 0){
        printf("Nenhuma ação foi lida. Verifique o arquivo.\n");
        free(array);
        return 1;
    }

    printf("Foram lidas %d ações. Capital disponível: R$ %d\n", n, capital);

    printf("========================== Acoes ==========================\n");
    printf("-----------------------------------------------------------\n");
    printf("%-20s | %-10s | %-10s\n", "Nome", "Custo", "Retorno");
    for(int i = 0; i < n; i++){
        printf("%-20s | %10d | Retorno: %9.2f%%\n", array[i].nome, array[i].custo, array[i].retorno);
    }

    // Método máscara de bit (pode ser muito lento para muitas ações)
    /*
    int melhorCusto;
    float maiorRetorno;
    int melhorcomb = investimento(array, n, capital, &melhorCusto, &maiorRetorno);
    exibirResultado(array, n, capital, melhorcomb, melhorCusto, maiorRetorno);
    */

    // Mochila dinâmica
    int *selecionadas = calloc(n, sizeof(int));
    float retornoMaximo = mochilaknapsack(array, n, capital, selecionadas);
    exibirResultadoPD(array, n, capital, selecionadas, retornoMaximo);

    free(array);
    free(selecionadas);
    return 0;
}

