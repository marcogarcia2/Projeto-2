#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // funções strcmp e strcpy
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

#define N 50000
#define M 70000
#define B 150001

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20

unsigned colisoes_h_div = 0;
unsigned colisoes_h_mul = 0;
unsigned encontrados_h_div = 0;
unsigned encontrados_h_mul = 0;

// TAD Lista encadeada

typedef struct node{
    long key;
    struct node *next; // Não podemos colocar Node *next, pois o compilador lê de cima para baixo, e ainda não sabe o que é Node
} No;

typedef struct{
    No *begin; // Ponteiro para o primeiro nó
    No *end; // Ponteiro para o último nó, utilizado na hora de adicionar na cauda da lista
    long size;
} Lista;

Lista *cria_Lista(){
    Lista *L = (Lista *) calloc(1, sizeof(Lista)); 
    L->begin = L->end = NULL;
    L->size = 0;

    return L;
}

void destroi_Lista(Lista **L_ref){
    Lista *L = *L_ref;

    No *p = L->begin;
    No *aux = NULL; // Preciso do aux, pois ao dar free(p) eu perderia o p->next

    while(p != NULL){
        aux = p; // aux aponta para onde o p aponta (salvamos a posição)
        p = p->next; // avançamos uma posição
        free(aux);
    }
    free(L); // Desalocamos a lista em si

    *L_ref = NULL;
}

No *cria_No(long key){
    No *no = (No *) calloc(1, sizeof(No));
    no->key = key;
    no->next = NULL;

    return no;
}

void insere_No(Lista *L, long key){
    No *no = cria_No(key);

    if(L->size == 0)
        L->begin = L->end = no;
    else{
        L->end->next = no;
        L->end = no;
    }
    L->size++;
}

int busca_Lista(const Lista *L, long key){

    No *atual = L->begin;
    while (atual != NULL){

        if (atual->key == key)
            return 1;

        atual = atual->next;
    }

    return 0;
}


unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

unsigned h_div(unsigned x)
{
    return (x % B);
}

unsigned h_mul(unsigned x)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

typedef struct{

    Lista *vet[B];

}hash;

void inicializar(hash *tabela){

    for(int i = 0; i < B; i++){
        tabela->vet[i] = cria_Lista();
    }
}

void inserir_div(hash *tabela, long k){
    long pos = h_div(k);
    insere_No(tabela->vet[pos], k);
    colisoes_h_div += tabela->vet[pos]->size - 1;
}

void buscar_div(hash *tabela, long k){
    long pos = h_div(k);
    encontrados_h_div += busca_Lista(tabela->vet[pos], k);
}

void inserir_mul(hash *tabela, long k){
    long pos = h_mul(k);
    insere_No(tabela->vet[pos], k);
    colisoes_h_mul += tabela->vet[pos]->size - 1;
}

void buscar_mul(hash *tabela, long k){
    long pos = h_mul(k);
    encontrados_h_mul += busca_Lista(tabela->vet[pos], k);
}

int main(int argc, char const *argv[])
{
    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);


    // cria tabela hash com hash por divisão
    hash *tabela_div;
    inicializar(tabela_div);

    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        inserir_div(tabela_div, converter(insercoes[i]));
    }

    double tempo_insercao_h_div = finaliza_tempo();

    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        buscar_div(tabela_div, converter(consultas[i]));
    }
    double tempo_busca_h_div = finaliza_tempo();


    // cria tabela hash com hash por multiplicação
    hash *tabela_mul;
    inicializar(tabela_mul);

    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        inserir_mul(tabela_mul, converter(insercoes[i]));
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        buscar_mul(tabela_mul, converter(consultas[i]));
    }
    double tempo_busca_h_mul = finaliza_tempo();


    printf("Hash por Divisao\n");
    printf("Colisoes na insercao: %d\n", colisoes_h_div);
    printf("Tempo de insercao   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicacao\n");
    printf("Colisoes na insercao: %d\n", colisoes_h_mul);
    printf("Tempo de insercao   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    for(int i = 0; i < N; i++){
        free(insercoes[i]);
    }
    free(insercoes);

    for(int i = 0; i < M; i++){
        free(consultas[i]);
    }
    free(consultas); 

    for (int i = 0; i < B; i ++){
        destroi_Lista(&(tabela_div->vet[i]));
        destroi_Lista(&(tabela_mul->vet[i]));
    }

    return 0;
}