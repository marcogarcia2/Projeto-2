#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
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

unsigned colisoes = 0;
unsigned encontrados = 0;

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


unsigned h_div(unsigned x, unsigned i)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i)
{
    const double A = 0.6180;
    return  ((int) ((fmod(x * A, 1) * B) + i)) % B;
}

unsigned h(unsigned x, unsigned i){

    return (h_mul(x, i) + i * h_div(x, i)) % B;
}

// marcacao: -1 indica posicao vazia, nunca utilizada
void inicializar(long *vet){
    for(int i = 0; i < B; i++)
        vet[i] = -1;
}

// Funcoes utilizando o tratamento h_div
long inserir(long *vet, long k){
    long pos;
    for(long i = 0; i < B; i++){
        pos = h(k, i); // calcula o endereco onde eu devo inserir
        if(vet[pos] == -1 ||vet[pos] == -2){ // se esta vazia eu insiro
            vet[pos] = k; // copia registro
            return pos;
        }
        if(vet[pos] == k){
            return -1; // erro, elemento repetido
        }
        colisoes++;
    }

    return -1; // tabela cheia, pois eu tentei de i do 0 a B-1 e nao consegui encaixar, quer dizer que a tabela esta cheia
}

long buscar(long *vet, long k){
    long pos;
    for(long i = 0; i < B; i++){
        pos = h(k,i);
        if(k == vet[pos]){
            encontrados++;
            return pos;
        }
        if(vet[pos] == -1)
            return -1; // tabela com espaco
    }
    return -1; // erro, tabela cheia
}

int main(int argc, char const *argv[])
{

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);

    // cria tabela hash com hash por hash duplo
    long tabela[B];
    inicializar(tabela);


    // inserção dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        inserir(tabela, converter(insercoes[i]));
    }
    double tempo_insercao = finaliza_tempo();

    // busca dos dados na tabela hash
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        buscar(tabela, converter(consultas[i]));
    }
    double tempo_busca = finaliza_tempo();


    printf("Colisões na inserção: %d\n", colisoes);
    printf("Tempo de inserção   : %fs\n", tempo_insercao);
    printf("Tempo de busca      : %fs\n", tempo_busca);
    printf("Itens encontrados   : %d\n", encontrados);

    for(int i = 0; i < N; i++){
        free(insercoes[i]);
    }
    free(insercoes);

    for(int i = 0; i < M; i++){
        free(consultas[i]);
    }
    free(consultas); 

    return 0;
}
