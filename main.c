#include <stdio.h>

typedef int elem; // elemento da tabela de dados

typedef struct {
    elem key; // chave (o próprio elemento)
    long position; // inidicador da posicao deste elemento na tabela de dados
} elem_i; // tabela (mudar este nome)


double media(double A[], int n){

    double media = 0;

    for (long i = 0; i < n; i++)
        media += A[i];

    return media/n;
}

double desvioPadrao(double A[], int n, double media){

    double soma = 0;
    
    for (long i = 0; i < n; i++)
        soma += pow((media - A[i]), 2);
    
    return sqrt(soma/n);
}


int main(){



    return 0;
}