/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 3: Indice de Livro                          */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Outubro de 2018                  */
/* Programa Compilado com Code::Blocks 17.12             */


#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef char vetor[72];
typedef char tipelemento[20];

struct arvore{
    tipelemento info;
    arvore *pai,
           *fesq,
           *idir;
};

/* Variaveis Globais*/
arvore *raiz;
int nivel = -1,posic[8]={0},lista=0;// variaveis para utilizar na itemizacao

/* FUNCOES GERAIS */
void lerinicio(FILE *entrada, vetor linhainicio[4])
{//ler as linhas iniciais
    for(int i= 0; i<4;i++)
        fgets(linhainicio[i],72,entrada);
}

void printhifen(FILE *saida)
{//funcao que printa os hifens no arquivo de saida
    fprintf(saida,"--------------------------------------------------\n");
    return;
}

void printinicio(FILE *saida, vetor linhainicio[4])
{//imprime as linhas iniciais no arquivo de saida
    for(int i = 0;i<3;i++)
        fputs(linhainicio[i],saida);
    printhifen(saida);
}

void erro(int n, FILE *saida, tipelemento nome )
{//funcao geral com os erros
    switch (n)
    {
    case 1:
        fprintf(saida,"ERRO: o item %s ja existe na lista\n", nome);
        printhifen(saida);
        break;
    case 2:
        fprintf(saida,"ERRO: nao encontrado item %s\n", nome);
        printhifen(saida);
        break;
    }
}

/* FUNCOES COMPLEMENTARES AS ACOES */
void inicializar(arvore *p)
{//atribui NULL aos ponteiros de um noh
    p->fesq = NULL;
    p->idir = NULL;
    p->pai = NULL;
}

arvore* buscar(tipelemento nome, arvore *x)
{//retorna o ponteiro q esta apontando o lugar na arvore q possui o nome buscado
    arvore *aux;
    arvore *resposta;
    if(x== NULL || strcmp(nome,x->info)==0)
        return x;
    else
    {
        resposta = NULL;
        aux = x->fesq;
        while(aux != NULL && resposta == NULL)
        {
            resposta = buscar(nome,aux);
            aux = aux->idir;
        }
        return resposta;
    }
}

void liberar(arvore *x)
{//funcao que da free no noh e em seus filhos
    if(x != NULL)
    {
        arvore *p;//defino um ponteiro auxiliar p
        p = x->fesq;//aponto para o filho de x
        while(p!= NULL)//enquanto p for diferente de NULL, o filhoesquerdo de x vira o irmao direito de p e libero o P
        {
            x->fesq = p->idir;
            liberar(p);
            p = x->fesq;
        }
        free(x);
    }
}

void visitar(FILE *saida,arvore *x)
{//funcao que printa um item no arquivo de saida
    int i=1;
    while(posic[i]!=0)
    {
        fprintf(saida,"%d.",posic[i]);
        i++;
    }
    fprintf(saida,"      %s\n",x->info);
}

/* FUNÇOES DE ACAO */
void inserir(FILE *entrada, FILE *saida)
{//inserir um elemento na arvore
    arvore *parente, *filho, *aux;
    char referencia[15];
    tipelemento item, paiouirmao;
    //ler o nome do item q sera inserido, a referencia(SUB ou SEGUINTE) e o nome da referencia
    fscanf(entrada,"%s %s %s ", item, referencia,paiouirmao);
    //buscar os nomes na arvore
    filho = (arvore*)malloc(sizeof(arvore));//precisa de previsao de ERRO para memoria cheia???
    inicializar(filho);
    aux = buscar(item, raiz);
    parente = buscar(paiouirmao, raiz);
    //se encontrar o mesmo nome no indice, da erro
    if(aux != NULL)
    {
        erro(1,saida, item);
        return;
    }
    //se nao encontrar o parente na lista, da erro
    else if(parente == NULL)
    {
        erro(2,saida, paiouirmao);
        return;
    }
    else if(strcmp(referencia,"SUB")==0)
    {//se SUB, o parente eh pai
       strcpy(filho->info,item);
       //se o pai nao tem filho, adiciona o item como fesq
       filho->idir = parente->fesq;
       parente->fesq = filho;
       filho->pai= parente;
    }
    else if(strcmp(referencia,"SEGUINTE_A")==0)
    {//se SEGUINTE_A, o parente eh irmao
        strcpy(filho->info,item);
        //mesmo pai do parente
        filho->pai = parente->pai;
        arvore *auxiliar;
        auxiliar = parente->idir;
        parente->idir = filho;
        filho->idir = auxiliar;
    }
}

void listar(FILE *saida, arvore *x)
{//imprimir os elementos da arvores no arquivo de saida
    if(x!= NULL)
    {
        //sempre q chamar recursivamente a funcao, ele desce um nivel
        nivel++;
        arvore *p;
        visitar(saida,x);
        p = x->fesq;
        while(p!= NULL)
        {
            posic[nivel+1]++;
            listar(saida,p);
            p= p->idir;
        }
        //zera a quantidade de itens no nivel inferior
        posic[nivel+1]=0;
        //ao encerrar a funcao, sobe um nivel
        nivel --;
    }
}

void titular(FILE *entrada)
{//essa funcao cria uma arvore e ja substitui o titulo atual(mesmo sendo NULL) por um titulo lido no arquivo
    //cria um ponteiro auxiliar
    arvore *aux;
    aux = (arvore*)malloc(sizeof(arvore));
    //aloca memoria para o ponteiro
    inicializar(aux);
    //le a string e coloca no info
    fscanf(entrada, "%s ", aux->info);
    //atribui filho esquerdo à raiz atual
    aux->fesq = raiz;
    if(raiz!= NULL)
        raiz->pai = aux;
    //transformar o aux na raiz;
    raiz = aux;
}

void remover(FILE *entrada,FILE *saida)
{//remove um noh e seus filhos da arvore
    arvore *aux,*ant;
    tipelemento item;
    fscanf(entrada,"%s ",item);
    aux = buscar(item,raiz);
    if (aux == NULL)
        erro(2,saida,item);
    else
    {
        ant = aux->pai;
        if(ant == NULL)//se o ant for NULL, o aux eh o titulo
        {
            liberar(aux);
            raiz=NULL;
        }
        else if(ant->fesq == aux)//aux eh o filho esquerdo
        {
            ant->fesq = ant->fesq->idir;
            liberar(aux);
        }
        else//aux eh irmao direito de alguem
        {
            ant= ant->fesq;//desce o ant para o fesq e depois roda ate achar o aux
            while(ant->idir != aux)
                ant = ant->idir;
            ant->idir = aux->idir;
            liberar (aux);
        }
    }
}

void transferir(FILE *entrada,FILE *saida)
{//transfere um noh para outro lugar da arvore
    char referencia[15];
    tipelemento item, paiouirmao;
    fscanf(entrada,"%s %s %s ", item, referencia,paiouirmao);
    arvore *ant, *aux,*dest;
    aux = buscar(item,raiz);
    dest = buscar(paiouirmao,raiz);
    if (aux == NULL)
        erro(2,saida,item);
    else if (dest == NULL)
        erro(2,saida,paiouirmao);
    else
    {
        //primeira coisa: tirar o item da lista
        ant = aux->pai;
        //aux nunca poderar ser o titulo, segundo o professor
        if(ant->fesq == aux)//aux eh o filho esquerdo
            ant->fesq = ant->fesq->idir;
        else//aux eh irmao direito de alguem
        {
            ant= ant->fesq;//desce o ant para o fesq e depois roda ate achar o aux
            while(ant->idir != aux)
                ant = ant->idir;
            ant->idir = aux->idir;
        }
        //segunda parte: inserir no lugar falado
        if(strcmp(referencia,"SUB")==0)
        {//se SUB, o dest eh pai
           //se o pai nao tem filho, adiciona o item como fesq
           aux->idir = dest->fesq;
           dest->fesq = aux;
           aux->pai= dest;
        }
        else if(strcmp(referencia,"SEGUINTE_A")==0)
        {//se SEGUINTE_A, o dest eh irmao
            //mesmo pai do dest
            aux->pai = dest->pai;
            //idir do auxiliar sera o atual idir do dest
            aux->idir = dest->idir;
            //idir do dest passa a ser o aux
            dest->idir = aux;
        }
    }
}

/* INICIALIZAÇAO DO PROGRAMA */
void iniciarprograma(FILE *entrada, FILE *saida)
{
    char acao[11];
    do
    {
        fscanf(entrada,"%s ", acao);
        if (strcmp(acao,"INSERIR")==0)
            inserir(entrada,saida);
        else if(strcmp(acao,"LISTA")==0)
        {
            lista++;
            fprintf(saida,"LISTA %d\n\n",lista);
            listar(saida,raiz);
            printhifen(saida);
        }
        else if(strcmp(acao,"TITULO")==0)
            titular(entrada);
        else if(strcmp(acao,"REMOVER")==0)
            remover(entrada,saida);
        else if(strcmp(acao,"TRANSFERIR")==0)
            transferir(entrada,saida);
    }while(strcmp(acao, "FIM")!=0);
}

/* MAIN */
int main()
{
    FILE *entrada,*saida;
    entrada = fopen("C:\\Lab5\\entrada5.txt","r");
    saida = fopen("C:\\Lab5\\Carlos_Renato_Figueiredo5.txt","w");
    vetor linhainicio[4];
    raiz = NULL;
    lerinicio(entrada, linhainicio);
    printinicio(saida,linhainicio);
    iniciarprograma(entrada,saida);
    fclose(saida);
    //system("C:\\Lab5\\saida5.txt");//debug
    return 0;
}
