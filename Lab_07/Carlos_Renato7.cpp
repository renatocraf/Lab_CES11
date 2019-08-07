/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 7: Fibra Ótica                              */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Dezembro de 2018                 */
/* Programa Compilado com Code::Blocks 17.12             */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char vetor[11];
typedef char lixo[73];

struct noh
{// esse eh o no que mostra o indice e o custo dos adjacentes
    int indice;
    int custo;
    noh *prox;
};

struct grafo
{//estrutura do grafo
    vetor nome;
    bool visitado;
    noh *adj;

};

struct algoritmo
{
    int indice1,
        indice2,
        custo;
};

//variaveis globais
grafo predio[151];
algoritmo prim [150];
int quant=0;

int busca(vetor nome)
{
    for(int i = 1;i<=quant;i++)
        if(strcmp(predio[i].nome,nome) == 0)
            return i;
    return -1;
}

void inserir(vetor nome1,vetor nome2,int custo)
{
    int indice1,indice2;
    noh *aux;
    //verificar se o predio ja esta no grafo
    indice1 = busca(nome1);
    indice2 = busca(nome2);
    //se o predio 1 nao estiver no grafo, adiciona ele
    if(indice1 == -1)
    {
        //soma mais um no quant
        quant++;
        //inserer valores na posicao quant
        strcpy(predio[quant].nome,nome1);
        predio[quant].visitado = false;
        predio[quant].adj = NULL;
        //noh para adjacente sera criado no final
        indice1 = quant;
    }
    //se o predio 2 nao estiver no grafo, adiciona ele
    if(indice2 == -1)
    {
        //soma mais um no quant
        quant++;
        //inserir valores na posicao quant
        strcpy(predio[quant].nome,nome2);
        predio[quant].visitado = false;
        predio[quant].adj = NULL;
        //noh para adjacente sera criado no final
        indice2 = quant;
    }
    //agora q sabemos os indices dos predio, temos que adicionar o nohs(adjacentes)
    //alocando memoria para o nome1
    aux = (noh*)malloc(sizeof(noh));
    //copiando valores para aux
    aux->indice = indice1;
    aux->custo = custo;
    //inserindo nos adj
    aux->prox = predio[indice2].adj;
    predio[indice2].adj = aux;
    //mesma coisa para o predio 2
    //alocando memoria para o nome2
    aux = NULL;
    aux = (noh*)malloc(sizeof(noh));
    //copiando valores para aux
    aux->indice = indice2;
    aux->custo = custo;
    //inserindo nos adj
    aux->prox = predio[indice1].adj;
    predio[indice1].adj = aux;

}

void lerentrada(FILE *entrada)
{
    vetor nome1,nome2;
    int custo;
    while(fscanf(entrada,"%s %s %d ",nome1,nome2,&custo)!=EOF)
    {
        inserir(nome1,nome2,custo);
    }
}

void calcularprim()
{
    //criar contador com o numero de arestas necessarias para ligar todos os membros do grafo
    int cont = 0;
    //inserir o primeiro vertice na nuvem(no meu caso, escolhi o primeiro do grafo)
    predio[1].visitado = true;
    for(cont = 1;cont<quant;cont++)
    {//vai fazer a contagem de 1 ate quant-1
        int menor=1001;//variavel auxiliar e tem q ter custo maior do que 999
        int i=0,j=1;//i server como contador de vertice que estao na "nuvem", quando "i" se iguala a "cont" quer dizer que eu ja rodei por todos os vertices da nuvem
        while(i<cont && j<=quant )//j eh vertice que esta na "nuvem"
        {//a cada passagem, tem q verificar todas arestas q saem da nuvem
            if(predio[j].visitado)//verifica se j ta na nuvem
            {
                i++;//se sim, adiciona 1 no contadar "i" para otimizar a busca
                noh *p; //ponteiro auxiliar para correr os adjacentes
                p = predio[j].adj;//p recebe o primeiro adjacente
                while(p!= NULL)
                {//percorrendo os adjacentes de j
                    //se o adjacente nao esta na nuvem e o caminho entre j e o adj eh menor, faz a troca
                    if(!predio[p->indice].visitado && menor> p->custo)
                    {
                        menor = p->custo;
                        prim[cont].custo = menor;
                        prim[cont].indice1 = j;
                        prim[cont].indice2 = p->indice;
                    }
                    p = p->prox;
                }
            }
            j++;
        }
        //depois de definir o menor custo da vez, temos que marcar os predios contemplados como visitados
        //como o indice 1 do prim eh sempre o predio visitado, falta marcar o indice2 como visitado
        predio[prim[cont].indice2].visitado=true;
    }
}

void printarsaida(FILE *saida)
{
    fprintf(saida,"Arquivo saida\n");
    fprintf(saida,"Resposta do Algoritmo de Prim\n");
    fprintf(saida,":-)\n");
    fprintf(saida,"\n");
    fprintf(saida,"---------------------------------------\n");
    fprintf(saida,"Ordem de selecao pelo algoritmo\n");
    fprintf(saida,"---------------------------------------\n");

    /* escrevendo o prim[]*/
    int custo =0;
    for(int i = 1;i<quant;i++)
    {
        custo+=prim[i].custo;
        fprintf(saida,"%-10s %-10s %3d\n",predio[prim[i].indice1].nome,predio[prim[i].indice2].nome,prim[i].custo);
    }
    /* */
    fprintf(saida,"\nCusto total: %d\n\n",custo);
    fprintf(saida,"---------------------------------------\n");
    fprintf(saida,"Ordem crescente de custos\n");
    fprintf(saida,"---------------------------------------\n");
    /* escrevendo o prim[] em ordem crescente*/
    //fazer um bubblesort do prim
    bool trocou= true;
    for(int i = 1;i<=quant-2 && trocou;i++)
    {
        //trocou = false;
        for(int j = i+1;j<=quant-1;j++)
        {
            int a=prim[j].custo;
            int b=prim[i].custo;
            if(prim[j].custo<prim[i].custo)
            {
                trocou = true;
                algoritmo aux;
                aux = prim[i];
                prim[i]= prim[j];
                prim[j]= aux;
            }
        }
    }
    for(int i = 1;i<quant;i++)
    {
        custo+=prim[i].custo;
        fprintf(saida,"%-10s %-10s %3d\n",predio[prim[i].indice1].nome,predio[prim[i].indice2].nome,prim[i].custo);
    }
    /* */
}

int main()
{
    //variaveis
    FILE *entrada,*saida;
    lixo linhasiniciais[6];
    //abrindo arquivos de texto
    entrada = fopen("C:\\Lab7\\entrada7.txt","r");
    saida = fopen("C:\\Lab7\\Carlos_Renato7.txt","w");
    //pulando as 6 primeiras linhas
    for(int i = 0;i<6;i++)
    {
        fgets(linhasiniciais[i],72,entrada);
    }
    //inserindo nomes nos grafos
    lerentrada(entrada);
    calcularprim();
    printarsaida(saida);
    fclose(entrada);
    fclose(saida);
    //system("C:\\Lab7\\Carlos_Renato7.txt"); //debug
    return 0;
}
