/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 2: Ordenação                                */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Setembro de 2018                 */
/* Programa Compilado com Code::Blocks 17.12             */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

//Variaveis globais
typedef char vetor[52];
int cont=0;
//declarando vetor global para o MergeSort
vetor *T;

void printfnome(int n,vetor *nomes)
{//funcao para debug
    for(int i=0;i<n;i++)
        printf("%s",nomes[i]);
}

int compara (const char * a, const char * b)
{//funcao que sera usada para contar a quantidade de comparacoes de strings
cont ++;
return strcmp (a, b);
}

//--------------------------------------------------
//    BUBBLE SORT
//--------------------------------------------------

void bubblesort(int n,vetor *nomes)
{
    vetor aux;
    for(int i=0; i<=n-2;i++)
    {
        for(int j=n-i-2;j>=0;j--)
        {
            //int comp = compara(nomes[j],nomes[j+1]);
            if(compara(nomes[j],nomes[j+1]) == 1)
            {
                strcpy(aux,nomes[j]);
                strcpy(nomes[j],nomes[j+1]);
                strcpy(nomes[j+1],aux);
            }
        }
    }
}

//--------------------------------------------------
//    MERGE SORT
//--------------------------------------------------

void Merge(vetor V[],int ini, int fim)
{//funcao que mescla os vetores
   //vetor *T;                                  //Para merge com vetor local
   //T = (vetor*)malloc((fim+1)*sizeof(vetor)); // temporario
   int med,
       i,          //percorre T
       j,          //percorre lado esquerdo de V
       k;          //percorre lado direito  de V

   med = (ini+fim)/2; //mesmo arredondamento feito ao chamar MergeSort
   j = ini;
   k = med+1;
   i = ini;
   //percorrer selecionando os menores
   while (j <= med && k <= fim)
   {
      if (compara(V[j],V[k]) <1)
      {
         strcpy(T[i],V[j]);
         j++;
      }
      else
      {
         strcpy(T[i],V[k]);
         k++;
      }
      i++;
   }
   //se sobrou algo `a esquerda, copiar para T
   while(j <= med)
   {
      strcpy(T[i],V[j]);
      i++;
      j++;
   }
   //se sobrou algo `a direita, copiar para T
   while(k<=fim)
   {
      strcpy(T[i],V[k]);
      i++;
      k++;
   }
   // V recebe T
   for(i=ini; i<=fim; i++)
        strcpy(V[i],T[i]);
    //free(T);          //para T local
}

void MergeSort(vetor V[], int ini, int fim)
{
   int med;
   if (ini < fim)
   {
      med = (ini+fim)/2;          // dividir ao meio
      MergeSort (V, ini,   med);  // ordenar lado esquerdo
      MergeSort (V, med+1, fim);  // ordenar lado direito
      Merge     (V, ini,   fim);  // mesclar as duas metades
   }
}

//--------------------------------------------------
//    QUICK SORT
//--------------------------------------------------

int Partition (vetor V[], int ini, int fim)
{
   vetor pivo, aux;
   int esq, dir;

   strcpy(pivo,V[ini]);    // neste exemplo, o pivo é o primeiro da esquerda
   esq  = ini + 1;
   dir  = fim;
   //int comp;
   do
   {
      // percorrer da esquerda para a direita. encontrar alguem >= pivo
      //comp = ;
      while (esq < fim && compara(V[esq],pivo)<0){
         esq++;
         //comp = compara(V[esq],pivo);
         }
      // percorrer da direita para a esquerda. encontrar alguem < pivo
      //comp = compara(pivo,V[dir]);
      while (ini < dir && compara(pivo,V[dir]) <=0){
         dir--;
         //comp = compara(pivo,V[dir]);
         }
      // se achou um `a esquerda e outro `a direita
      if(esq < dir)
      {
         // permutar
         strcpy(aux,V[esq]);
         strcpy(V[esq],V[dir]);
         strcpy(V[dir],aux);
      }
      // caso contrario, terminara´ a repeticao
   }while (esq < dir);

   // trocar o pivo com o elemento que "divide" os subvetores
   strcpy(V[ini],V[dir]);
   strcpy(V[dir],pivo);
   // retornar a posição da "divisa"
   return dir;
}

void QuickSort (vetor V[], int ini, int fim)
{
   int p; //posição do pivo
   if (ini < fim)
   {
      p = Partition (V, ini, fim);
      QuickSort     (V, ini, p-1);
      QuickSort     (V, p+1, fim);
   }
}

//--------------------------------------------------
//    FUNCOES DE LEITURA E ESCRITA NO DOCUMENTO
//--------------------------------------------------

void LerTxt(FILE *entrada,int *n,vetor *nomes){
    entrada = fopen("c:\\Lab2\\entrada2.txt","r");
    //pegando o valor de N na primeira linha
    fscanf(entrada,"%d ",n);
    //pegando os nomes
    for(int i=0;i<*n;i++)
        fgets(nomes[i],52,entrada);
    fclose(entrada);
}

void ImprimirTxt(FILE *saida,int n, double delta_t,vetor *nomes,vetor algoritmo){
    fprintf(saida,"Algoritmo: %s\n",algoritmo);
    fprintf(saida,"\nTamanho da Entrada: %d\n",n);
    fprintf(saida,"Comparacoes feitas: %d\n",cont);
    fprintf(saida,"Tempo de execucao : %.3lf segundos\n",delta_t);
    fprintf(saida,"\n--------------------------------------------------\n");
    for(int i=0;i<n;i++)
        fputs(nomes[i],saida);
    fclose(saida);
}

//--------------------------------------------------
//    MAIN
//--------------------------------------------------

int main(){
    //Declaracao de variaveis
    FILE *entrada, *saidabubble,*saidamerge,*saidaquick;
    entrada = fopen("c:\\Lab2\\entrada2.txt","r");
    int n;
    clock_t inicio,fim;
    double delta_t;
    vetor *nomes;
    fscanf(entrada,"%d ",&n);
    //criando o vetor com o tamanho certo
    nomes = (vetor*)malloc(n*sizeof(vetor));
    T =  (vetor*)malloc(n*sizeof(vetor));       //vetor auxiliar do Merge-Sort
    fclose(entrada);
    LerTxt(entrada,&n,nomes);

    /* BUBBLE-SORT*/
    inicio = clock();
    bubblesort(n,nomes);
    fim = clock();
    delta_t = (fim - inicio) /(double) CLOCKS_PER_SEC;
    //Printando no arquivo no bubble
    saidabubble = fopen("c:\\Lab2\\Carlos_Renato_Figueiredo2_bubble.txt","w");
    ImprimirTxt(saidabubble,n,delta_t,nomes,"Bubble-Sort");
    /* FIM BUBBLE-SORT*/

    /* MERGE-SORT*/
    cont =0;
    LerTxt(entrada,&n,nomes);
    inicio = clock();
    MergeSort(nomes,0,n-1);
    fim = clock();
    delta_t = (fim - inicio) /(double) CLOCKS_PER_SEC;
    //Printando no arquivo no merge
    saidamerge = fopen("c:\\Lab2\\Carlos_Renato_Figueiredo2_merge.txt","w");
    ImprimirTxt(saidamerge,n,delta_t,nomes,"Merge-Sort");
    /* FIM MERGE-SORT*/

    /* QUICK-SORT*/
    cont = 0;
    LerTxt(entrada,&n,nomes);
    inicio = clock();
    QuickSort(nomes,0,n-1);
    fim = clock();
    delta_t = (fim - inicio) /(double) CLOCKS_PER_SEC;
    //Printando no arquivo no quick
    saidaquick = fopen("c:\\Lab2\\Carlos_Renato_Figueiredo2_quick.txt","w");
    ImprimirTxt(saidaquick,n,delta_t,nomes,"Quick-Sort");
    /* FIM QUICK-SORT*/

    return 0;
}
