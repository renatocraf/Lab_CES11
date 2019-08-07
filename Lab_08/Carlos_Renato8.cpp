/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 8: PERT-CPM                                 */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Dezembro de 2018                 */
/* Programa Compilado com Code::Blocks 17.12             */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char vetor[72];
typedef char lixo[73];

struct tarefas
{//struct q vai guardar as informacoes de cada tarefa
    char letra;
    char descricao[31];
    int duracao;
};

struct vetores
{//struct para guardar varios vetores utilizados ao longo do codigo.
    tarefas tarefa[51];
    int visitados[51];//esse vetor vai ter os valores 0,1 ou 2
    int matriz[51][51];//matriz de adjacencias
    int quant;//guarda a quantidade de tarefas
    int TMT[51];//guarda o TMT para cada tarefa
    int antTMT[51];//antecessores do TMT
};

/* ESTRUTURA E FUNCOES PARA PILHA - PADRAO  */

int MAX;

//pilha com vetor

/*funcoes basicas da pilha
    - mostrar elemento
    - pilha vazia
    - inserir
    - remover
  Com TAD:
    - inicializar
    - esvaziar
*/
typedef int tipelemento;
struct pilha
{//informacoes padroes da pilha
    tipelemento *info; //lembrar de fazer o malloc
    int top;
};

void inicializar(pilha *t)
{//cria o vetor com malloc e deixa o top no 0
    t->info = (tipelemento*)malloc((MAX+1)*sizeof(tipelemento));//precisa de +1 porque nao utilizaremos a posicao 0
    t->top = 0;
}

bool pilhavazia(const pilha *t)
{//retorna true se a pilha estiver vazia
    if (t->top == 0)
        return true;
    else
        return false;
}

bool pilhacheia(const pilha *t)
{//retorna true se a pilha estiver cheia
    if (t->top == MAX)
        return true;
    else
        return false;
}

tipelemento mostrar(const pilha *t)
{//retorna o elemento(nesse caso int) que esta no topo da pilha
    if(pilhavazia(t))
        return -1;//quer dizer q ta vazio
    else
        return t->info[t->top];
}

void inserir(tipelemento x, pilha *t)
{//insere elemento no topo da pilha
    if(pilhacheia(t))
    {
        printf("Pilha cheia.\n");
        return;
    }
    else
    {
        t->top++;
        t->info[t->top]= x;
    }
    return;
}

void remover(pilha *t)
{//remove elemento do topo da pilha(faz apenas um top--)
   if (pilhavazia(t))
   {
       printf("pilha vazia.\n");
       return;
   }
   else
   {
        t->top--;
   }
   return;
}

void esvaziar(pilha *t)
{//esvazia a pilha(poe o topo em 0)
    t->top = 0;
    return;
}

/* FIM PILHA */

/* FUNCOES PARA LEITURA DO ARQUIVO DE ENTRADA E ORGANIZACAO DO GRAFO */
void lerinicio(FILE *entrada, vetor linhainicio[])
{//ler as 7 linhas iniciais
    for(int i= 0; i<7;i++)
        fgets(linhainicio[i],72,entrada);
}

void criartarefa(vetores *grafo, char letra, char descricao[31],int duracao)
{//funcao que cria uma tarefa quando ela aparecer nas tarefas precendentes e ainda nao estiver no vetor
    grafo->quant++;
    int quant = grafo->quant;
    grafo->tarefa[quant].letra = letra;
    grafo->tarefa[quant].duracao = duracao;
    strcpy(grafo->tarefa[quant].descricao,descricao);
    return;

}

int buscar(char c, vetores *grafo)
{//busca o caractere c dentro do vetor tarefa e retorna seu indice
    int quant = grafo->quant;
    int i = 1;
    while(i<=quant)
    {
        if(grafo->tarefa[i].letra == c)
            return i;
        i++;
    }
    return -1;
}

void lertarefa(FILE *entrada, vetores *grafo,int i)//,int matriz[][51])
{//funcao que le as informacoes da tarefa do arquivo de entrada e cria a tarefa caso ela ainda nao exista
    //lendo descricao e duracao
    fgets(grafo->tarefa[i].descricao,30,entrada);
    fscanf(entrada," %d ",&grafo->tarefa[i].duracao);
    //ler precedentes e verificar se ja estao no vetor de struct
    char c;
    fscanf(entrada,"%c",&c);
    while(c!='\n')
    {
        //se c for '.' quer dizer que nao há precedentes, entao a coluna da tarefa fica com 0 em tudo
        //se c for ',' deve ser ignorado e passado para o proximo
        //senao, deve buscar c no vetor de struct
        if(c !='.' && c != ',')
        {
            int busca = buscar(c,grafo);
            //se a busca retornar -1, cria a tarefa sem descricao e com duracao 0 (serao inseridos no futuro)
            if(busca == -1)
            {
                criartarefa(grafo,c,"",0);
                //atribuir ao valor de busca o novo indice da tarefa criada
                busca =  grafo->quant;
            }
            //adicionar o valor 1 na coluna da matriz de adjacencias
            grafo->matriz[i][busca]=1;
        }
        fscanf(entrada,"%c",&c);
    }
    return;
}

/* FIM ORGANIZACAO */

void inicializargrafo(vetores *grafo)
{//funcao que "zera" todas as informacoes do grafo
    for(int i= 0; i<=50;i++)
    {
        for(int j= 0; j<=50;j++)
        {
            grafo->matriz[i][j]=0;
        }
        grafo->visitados[i]=0;
        grafo->TMT[i]=0;
        grafo->antTMT[i]=0;
    }
    grafo->quant = 0;
}

void zerarvisitados(vetores *grafo)
{//funcao que "zera" o vetor visitados para poder ser utilizada em outra fase do exercicio
    for(int i= 0; i<=50;i++)
        grafo->visitados[i]=0;
}

void printarsaida(FILE *saida)
{//printa as informacoes iniciais no arquivo de saida
    fprintf(saida,"Arquivo de saida\n");
    fprintf(saida,"PERT/CPM\n");
    fprintf(saida,":-)\n");
    fprintf(saida,":-)\n");
    fprintf(saida,"\n");
    fprintf(saida,"------------------------------------------------------------\n");
    fprintf(saida,"\n");
    return;
}

/* FUNCOES PARA VERIFICAR SE EH CICLICO */

bool DFS(vetores *grafo, pilha *pilha1, int i,FILE *saida)
{//busca em profundidade que retorna true se houver ciclo e printa no arquivo de saida qual eh o ciclo.
    //caso nao haja ciclo, visita todas as tarefas conexas marcando-as como descobertas ao final
    bool aux = false;
    //marcar como descoberto
    grafo->visitados[i]= 1;
    //inserir na pilha
    inserir(i,pilha1);
    //fazendo dfs dos sucessores
    int j = 1;
    while (j <= grafo->quant && aux == false)
    {   //verifica se j eh sucessor de i
        if(grafo->matriz[j][i]==1)//matriz[sucessor]´[antecessor]
        {   //se o indice J no vetor visitados for 0, faz um DFS nele
            if (grafo->visitados[j]==0)
                {
                aux = DFS(grafo, pilha1,j,saida);
                //se o DFS de cima tiver encontrado o ciclo, ja podemos retornar true
                if (aux == true)
                    return true;
                }
            //se o indice J no vetor visitados for 1, achamos um ciclo! retorna e mostra o ciclo
            else if(grafo->visitados[j]==1)
            //escrever no arquivo de saida e retornar true
            {
                /* TEXTO BONITO DIZENDO QUE HÁ CICLO */
                //inserir(j,pilha1);
                fprintf(saida,"IMPOSSIVEL REALIZAR ORDENACAO TOPOLOGICA E CAMINHO CRITICO: GRAFO CICLICO\n");
                fprintf(saida,"\nCICLO ENCONTRADO EM:\n\t");
                int k = 1;
                while(pilha1->info[k]!= j)
                    k++;
                while(k <= pilha1->top)
                {
                    if(k < pilha1->top)
                        fprintf(saida,"%c -> ",grafo->tarefa[pilha1->info[k]].letra);
                    else
                        fprintf(saida,"%c",grafo->tarefa[pilha1->info[k]].letra);
                    k++;
                }
                return true;
            }
        }
        j++;
    }
    grafo->visitados[i]= 2;//marcar como visitado
    remover(pilha1);//remove da pilha

    //se sair do while, quer dizer que nao ha ciclos.. retorna false
    return false;


}

bool percorrer(vetores *grafo, pilha *pilha1,FILE *saida)
{//funcao para percorrer as tarefas que nao foram descobertas ainda
    int quant = grafo->quant;
    int i=1;
    bool aux=false;
    while(i<=quant && aux == false)
    {
        if(grafo->visitados[i]== 0)
        {
            aux = DFS(grafo,pilha1,i,saida);
            if (aux == true)
                return true;
        }
        i++;
    }
    return aux;
}
/* FIM CICLO */

/* FUNCOES PARA FAZER ORDENACAO TOPOLOGICA*/

void DFSOT(vetores *grafo, pilha *pilha1, int i)
{//busca em profundidade(pos-ordem) que, alem de marcar como visitado, insere numa pilha
    //marcar como descoberto
    grafo->visitados[i]= 1;
    //fazendo dfs dos sucessores
    int j = 1;
    while (j <= grafo->quant)
    {   //verifica se j eh sucessor de i
        if(grafo->matriz[j][i]==1)//matriz[sucessor]´[antecessor]
           //se o indice J no vetor visitados for 0, faz um DFS nele
            if (grafo->visitados[j]==0)
                DFSOT(grafo, pilha1,j);
        j++;
    }
    grafo->visitados[i]= 2;//marcar como visitado
    inserir(i,pilha1);
}

void percorrerOT(vetores *grafo,pilha *pilha1)
{//funcao que serve para percorrer as tarefas que ainda nao foram descobertas
    int quant = grafo->quant;
    int i=1;
    while(i<=quant)
    {
        if(grafo->visitados[i]== 0)
            DFSOT(grafo,pilha1,i);
        i++;
    }
}
/* FIM ORDENACAO TOPOLOGICA */
/* FUNCAO PARA TMT */

void TMT(vetores *grafo, int ind)
{//funcao que calcula o TMT de um indice e armazena no vetor grafo->tmt e armazena tbm seu antecessor no vetor grafo->antTMT
    int maior = 0;
    //int indantecessor;
    int ant=1;
    while(ant <= grafo->quant)
    {
        if(grafo->matriz[ind][ant]== 1 && maior< grafo->TMT[ant])
        {
            maior = grafo->TMT[ant];
            grafo->antTMT[ind]= ant;
        }
        ant++;
    }
    grafo->TMT[ind]= grafo->tarefa[ind].duracao+maior;
}
/* */

int main()
{
    //variaveis
    FILE *entrada, *saida;
    vetor linhainicio[7];
    vetores grafo;
    bool ciclo;
    //abrindo os arquivos de entrada e saida
    entrada = fopen("C:\\Lab8\\entrada8.txt","r");
    saida = fopen("C:\\Lab8\\Carlos_Renato8.txt","w");
    //lendo as primeiras 7 linhas
    lerinicio(entrada,linhainicio);
    //inicializando o grafo
    inicializargrafo(&grafo);
    //leitura do arquivo de entrada e organizando no grafo
    //capturando o primeiro caractere
    int busca=-1;
    grafo.quant=1;
    fscanf(entrada," %c " , &grafo.tarefa[grafo.quant].letra);
    //lendo outras informações
    char c = grafo.tarefa[grafo.quant].letra;
    while(c!='-')
    {
        if(busca == -1)
            lertarefa(entrada,&grafo, grafo.quant);//,matriz);
        else
            lertarefa(entrada,&grafo, busca);//,matriz);
        fscanf(entrada," %c " ,&c);
        busca = buscar(c,&grafo);
        if(c != '-' && busca == -1)
        {
            grafo.quant++;
            grafo.tarefa[grafo.quant].letra = c;
        }
    }

    //printar inicio do arquivo de saida
    printarsaida(saida);

    /* Verificando se tem ciclo */
    //criando pilha
    pilha pilha1;
    MAX = grafo.quant;
    inicializar(&pilha1);
    ciclo = percorrer(&grafo,&pilha1, saida);
    //esvaziar pilha pra ser usada no proximo passo
    esvaziar(&pilha1);
    zerarvisitados(&grafo);
    //fim verificacao

    /* Ordenacao topologica */
    //so vai haver ordenacao topologica se nao houver ciclo
    if (!ciclo)
    {
        //fazer uma pos ordem , com o visitado sendo o empilhar
        fprintf(saida,"UMA ORDENACAO TOPOLOGICA:\n");
        fprintf(saida,"\n");
        percorrerOT(&grafo,&pilha1);
        int i = pilha1.top;
        while(i >0)
        {
            int ind = pilha1.info[i];
            fprintf(saida,"%c ",grafo.tarefa[ind].letra);
            i--;
        }
    }
    fprintf(saida,"\n\n------------------------------------------------------------\n");
    /* Fim ordenacao topologica */

    /* Caminho critico */
    if (!ciclo)
    {
        //cria vetor
        fprintf(saida,"\n");
        fprintf(saida,"CAMINHO CRITICO:\n");
        fprintf(saida,"\n");
        fprintf(saida,"TAREFA        DESCRICAO           DURACAO\n");
        fprintf(saida,"\n");
        /*CODIGO AKI!*/
        while(pilha1.top>0)
        {
            int ind = mostrar(&pilha1);
            remover(&pilha1);
            TMT(&grafo,ind);
        }
        int maior=1,cont=2;
        while(cont<=grafo.quant)
        {
            if(grafo.TMT[maior]<grafo.TMT[cont])
                maior = cont;
            cont++;
        }
        esvaziar(&pilha1);
        int ant = maior;
        while(ant >0)
        {
            inserir(ant,&pilha1);
            ant = grafo.antTMT[ant];
        }
        while(pilha1.top>0)
        {
            int ind = mostrar(&pilha1);
            remover(&pilha1);
            fprintf(saida,"  %c   %s  %2d\n",grafo.tarefa[ind].letra,grafo.tarefa[ind].descricao,grafo.tarefa[ind].duracao);
        }
        /* FIM CODIGO*/
        fprintf(saida,"                                   ----\n");
        fprintf(saida,"TEMPO MINIMO PARA O PROJETO:         %2d semanas\n",grafo.TMT[maior]);
    }

    /* Fim caminho critico */

    //fechando os arquivos abertos
    fclose(entrada);
    fclose(saida);
    //debug
    //system("C:\\Lab8\\Carlos_Renato8.txt");
    return 0;
}

