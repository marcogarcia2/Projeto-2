#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

int busca_sequencial_indexada(const int tabela[], int N, int elemento, const int tabela_i[], int t){


    return 1;

}

void counting_sort(int A[], long posicao, long tam){

    // codigo escrito com base no pseudocodigo apresentado pelo professor

    long B[10] = {0};
    long key;

    for (long i = 0; i <= tam-1; i++){

        key = A[i] / posicao;
        key = key % 10;
        B[key] += 1;
    }
    
    for (long i = 1; i < 10; i++)
        B[i] += B[i-1];

    long *C = (long*) malloc(sizeof(long)*tam);

    for (long i = tam-1; i >= 0; i--){

        key = A[i]/posicao;
        key = key % 10;
        B[key] -= 1;
        C[B[key]] = A[i];
    }

    for (long i = 0; i <= tam-1; i++){

        A[i] = C[i];
    }

    free(C);
}

void radix_sort(int array[], int N){

    long maior = array[0]; //detectando o maior elemento do array
    for (long i = 1; i <= N - 1; i++)
        if(array[i] > maior) maior = array[i];

    // A funcao counting_sort eh chamada D vezes, sendo D o numero 
    //de digitos do maior elemento presente no array
    for (int posicao = 1; maior / posicao > 0; posicao *= 10)
        counting_sort(array, posicao, N);
    
}


int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    
    // ordenar entrada
    radix_sort(entradas, N);
    
    /*
    int flag = TRUE;
    for (int i = 0; i < N-1; i++){

        if (entradas[i] > entradas[i+1]){
            printf("ERRO!\n");
            flag = FALSE;
            break;
        }
    }
    if (flag) printf("Lista Ordenada\n");
    */

    // -------------

    // criar tabela de indice


    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
