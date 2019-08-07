/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 1: Produto de Matrizes                      */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Agosto de 2018                   */
/* Programa Compilado com Code::Blocks 17.12             */

#include<stdlib.h>
#include<stdio.h>
#define MAX_INT 2147483647 //maior inteiro(4 bytes) no Code::Blocks

struct matriz
{
    int linha;
    int coluna;
};

//declaracao de variaveis
FILE *saida = fopen("C:\\Lab01\\carlos_renato_figueiredo1.txt","w");
matriz vetor[21];
int melhorcorte[20][20]={0};    //matriz para utilizacao da segunda parte do programa
int cont=0;                     //contador para a quantidade de chamadas da funcao

int melhormultiplicacao(int inicio, int fim)
{//funcao recursiva que verifica na forca bruta a melhor multiplicao escalar das matrizes
    cont++;
    if (inicio == fim)          //caso base
        return 0;
    else
    {
        int mult=MAX_INT;
        for(int k=inicio; k<fim;k++ )
        {
            int aux = melhormultiplicacao(inicio,k)+melhormultiplicacao(k+1,fim)+vetor[inicio].linha*vetor[k].coluna*vetor[fim].coluna;
            if(mult>=aux)
            {
                mult=aux;
                melhorcorte[inicio][fim] = k;
            }
        }
        return mult;
    }
}
/* ---------------------------------------------------------------------*/

void mostrarcorte(int inicio, int fim)
{//funcao recursiva que printa no arquivo de saida a ordem de realizacao do produto
    if (inicio == fim)
        return;
    else if(inicio == fim-1)
        fprintf(saida,"   %2d x %2d\n",inicio,fim);
    else
    {
        int melhor = melhorcorte[inicio][fim];
        mostrarcorte(inicio,melhor);//mostra da esquerda para a direita
        mostrarcorte(melhor+1,fim);
        if(inicio == melhor)
            fprintf(saida,"   %2d x %2d-%2d\n",inicio, melhor+1,fim);
        else if(fim == melhor+1)
            fprintf(saida,"%2d-%2d x %2d\n",inicio,melhor,fim);
        else
            fprintf(saida,"%2d-%2d x %2d-%2d\n",inicio,melhor, melhor+1,fim);
    }
    return;
}
/* ---------------------------------------------------------------------*/

int main(){
    //Declaracao de variaveis
    FILE *entrada;
    entrada = fopen("C:\\Lab01\\entrada1.txt","r");
    char lixo[72];
    int n;                  //numero de matrizes
    int multiplicacao;

    //Pular as linhas iniciais do arquivo de entrada
    for(int i=0;i<5;i++)
    {
        fgets(lixo,72,entrada);
    }
     fscanf(entrada,"%d\n",&n);
     fgets(lixo,72,entrada);

     //Capturando dados do arquivo de entrada
     for(int i = 1; i<=n;i++)
     {
        fscanf(entrada,"%d ",&vetor[i].linha);
        fscanf(entrada,"%d ",&vetor[i].coluna);
     }
     fclose(entrada);

    multiplicacao = melhormultiplicacao(1,n);

    // Escrita no arquivo de saida
    fprintf(saida,"Arquivo de saida\n");
    fprintf(saida,"Exercicio 1\n");
    fprintf(saida,"Produto de Matrizes\n\n");
    fprintf(saida,"Total de Multiplicacoes escalares = %d\n\n",multiplicacao);
    fprintf(saida,"Chamadas aa Funcao: %d\n\n",cont);
    fprintf(saida,"Melhor Ordem para realizar o produto:\n");
    mostrarcorte(1,n);

    fclose(saida);

    //para debug
    //system("C:\\Lab01\\carlos_renato_figueiredo1.txt");
    return 0;
}
