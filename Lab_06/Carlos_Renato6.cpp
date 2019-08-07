/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 6: Agenda Eletronica                        */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Novembro de 2018                 */
/* Programa Compilado com Code::Blocks 17.12             */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef char vetor[73];
/* ESTRUTURAS */
struct tarefa{
    //cria tarefa com prioridade(import) e descricao (desc)
    int import;
    char desc[72];
};

struct arvbin{
    //estrututa da arvore
    tarefa *agenda;//cria ponteiro pra depois criar o vetor
    int quant,maxi;
};

//FUNÇOES: Inserir, Maximum, RemoveMax, Inicializar, Finalizar, HeapVazio, HeapCheio.
void inicializar(int maxi,arvbin *heap)
{//atribui um valor maximo do vetor que sera criado, depois cria um vetor com essa quantidade+1( ja que a posicao 0 nao sera utilizada)
    //atribui valor maximo, zera a quantidade e cria o vetor de tarefas
    heap->maxi = maxi;
    heap->quant = 0;
    heap->agenda = (tarefa*)malloc((heap->maxi+1)*sizeof(tarefa));
    return;
}

bool heapvazio(const arvbin *heap)
{//retorna true se o heap estiver vazio
    if(heap->quant == 0)
        return true;
    else
        return false;
}

bool heapcheio(const arvbin *heap)
{//retorna verdadeiro se o heap estiver cheio
    if(heap->quant == heap->maxi)
        return true;
    else
        return false;
}

void inserir(int import,vetor desc, arvbin *heap)
{//insere uma tarefa na agenda
    int filho,pai;
    tarefa trfaux;
    //adiciona mais um na quantidade
    heap->quant++;
    //melhorando a clareza
    filho = heap->quant;
    pai = heap->quant/2;
    //insere as informaçoes na ultima posicao(filho)
    heap->agenda[filho].import = import;
    strcpy(heap->agenda[filho].desc,desc);
    //compara a prioridade com o pai e troca caso seja maior
    //enqnt o pai for diferente de zero e a prioridade do pai for menor do que q prioridade de filho,
     //ele troca o filho com o pai
    while(pai != 0 && heap->agenda[pai].import < heap->agenda[filho].import)
    {
        trfaux = heap->agenda[pai];
        heap->agenda[pai]= heap->agenda[filho];
        heap->agenda[filho]= trfaux;
        //atribui o valor do pai para o filho
        filho = pai;
        pai = filho/2;
    }

}

tarefa maximum(const arvbin *heap)
{//retorna a tarefa de maior prioridade da agenda
    if (heapvazio(heap))
    {//se nao tiver tarefas, cria uma tarefa vazio, atribui valor -1 e retorna
        tarefa vazio;
        vazio.import = -1;
        return vazio;
    }
    else
    //senao, retorna a primeira tarefa da agenda
        return heap->agenda[1];
}

void removeMax(arvbin *heap)
{//pega o ultimo da lista e joga ele na raiz
 //depois vai testando de cima pra baixo para ver quem eh o maior
     int pai = 1, fesq = 2*pai, fdir = 2*pai +1,maior;
     tarefa trfmaior;
     bool trocou = true;
     //ultima tarefa vira a primeira
     heap->agenda[pai]= heap->agenda[heap->quant];
     //reduz o contador
     heap->quant--;
     do
     {
         //verificar se existe fesq e fdir
         if(fesq > heap->quant)
            maior = pai;
         else if(fdir > heap->quant)
            maior = fesq;
         //comparando os filhos e pegando o maior(caso igual, pega o filho da direita)
         else if(heap->agenda[fesq].import > heap->agenda[fdir].import)
            maior = fesq;
         else
            maior = fdir;
         if (pai!= maior && heap->agenda[pai].import<heap->agenda[maior].import)
         {
             trfmaior = heap->agenda[pai];
             heap->agenda[pai] = heap->agenda[maior];
             heap->agenda[maior]= trfmaior;
             pai = maior;
             fesq = pai*2;
             fdir = pai*2+1;
             trocou = true;
         }
         else
            trocou = false;
     }while(trocou);
}

void finalizar(arvbin *heap)
{//da o free no vetor alocado e zera maxi e quant
    free(heap->agenda);
    heap->maxi = 0;
    heap->quant = 0;
}

/*  */

int main()
{
    //variaveis
    FILE *entrada,*saida;
    arvbin heap;
    vetor linhainicial[5];
    vetor lixo,descricao;
    char acao[10];
    int maxi,importancia;
    tarefa proxtarefa;
    //abrindo arquivos de entrada e saida
    entrada = fopen("C:\\Lab6\\entrada6.txt","r");
    saida = fopen("C:\\Lab6\\Carlos_Renato6.txt","w");
    //ler as linhas iniciais
    for(int i=0;i<5;i++)
        fgets(linhainicial[i],72,entrada);
    //pegar numero max da heap
    fscanf(entrada,"%d ",&maxi);
    //inicializar a agenda
    inicializar(maxi,&heap);
    //pula tres linhas do arquivo de entrada
    for (int i = 0;i <3;i++)
        fgets(lixo,72,entrada);
    //escrever introducao no arquivo de saida
    fprintf(saida,"Agenda Eletronica - arquivo de saida.\n");
    fprintf(saida,":-)\n");
    fprintf(saida,":-)\n");
    fprintf(saida,":-)\n");
    fprintf(saida,"\n");
    fprintf(saida,"--------------------------------------------------\n");
    fprintf(saida,"RESPOSTAS DAS CONSULTAS\n");
    fprintf(saida,"--------------------------------------------------\n");
    //iniciando leitura das acoes
    do
    {
        fscanf(entrada,"%s ",acao);
        if(strcmp(acao,"NOVA")== 0)
        {
            //duas leituras: importancia e descricao
            fscanf(entrada,"%d ",&importancia);
            fgets(descricao,72,entrada);
             //verifica se heap esta cheio e retorna erro em caso positivo
            if(heapcheio(&heap))
                fprintf(saida,"ERRO     Agenda cheia. Impossivel inserir.\n");
            else
                inserir(importancia,descricao,&heap);
        }
        else if (strcmp(acao,"PROXIMA") == 0)
        {
            proxtarefa = maximum(&heap);
            //se a prioridade da tarefa retornada for -1 quer dizer que nao ha tarefas na agenda
            if (proxtarefa.import == -1)
                fprintf(saida,"AVISO    Nao ha tarefas na agenda  :-)\n");
            //senao, vai printar a prioridade e a tarefa e depois remove ela
            else
            {
                fprintf(saida,"%2d       %s",proxtarefa.import,proxtarefa.desc);
                removeMax(&heap);
            }
        }
    }while(strcmp(acao,"FIM")!= 0);
    //escrever o que fica para o dia seguinte
    fprintf(saida,"\n");
    fprintf(saida,"--------------------------------------------------\n");
    fprintf(saida,"FICA PARA O DIA SEGUINTE\n");
    fprintf(saida,"--------------------------------------------------\n");
    //enqnt o heap nao estiver vazio, pega a tarefa de maior prioridade, escreve e depois remove ela
    if(heapvazio(&heap))
    {
        fprintf(saida,"Agenda vazia! Nao restam tarefas para o dia seguinte.\n",proxtarefa.import,proxtarefa.desc);
    }
    while(!heapvazio(&heap))
    {
        proxtarefa = maximum(&heap);
        fprintf(saida,"%2d       %s",proxtarefa.import,proxtarefa.desc);
        removeMax(&heap);
    }
    //dar o free no heap
    finalizar(&heap);
}
