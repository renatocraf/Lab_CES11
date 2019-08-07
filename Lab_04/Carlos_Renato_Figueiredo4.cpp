/* Desenvolvido por: Carlos Renato de Andrade Figueiredo */
/* Turma 22.3                                            */
/*                                                       */
/* Exercicio 3: Banco                                    */
/*                                                       */
/* CES-11: Algoritmos e Estruturas de Dados              */
/* Prof. Armando    -   Outubro de 2018                  */
/* Programa Compilado com Code::Blocks 17.12             */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* ESTRUTURA DE DADOS */
struct pessoa{
    char nome[22];
    float saldo;
};

struct fila{
    pessoa *cliente;
    int ini,fim,quant,maxi;
    char titulo[7][100];
};

/* FUNCAO GERAL DE ERRO */
void erro(int n, FILE *saida,char nome[22]){
    switch (n){
        case 1://lista lotada
            fprintf(saida,"ERRO: Lista lotada. Impossível abrir conta\n");
            break;
        case 2://pessoas que nao sao clientes
            fprintf(saida,"ERRO: Nao encontrado cliente com nome %s",nome);
            break;
        case 3://conta com mesmo nome
            fprintf(saida,"ERRO: Nome ja cadastrado. Impossível abrir conta\n");
            break;
    }
}

/* FUNCOES GERAIS */
void inicializarFila(FILE *entrada, fila *contas){
    //leitura das linhas iniciais
    for(int i = 0; i <5;i++){
        fgets(contas->titulo[i],100,entrada);
    }
    //leitura da quantidade maxima
    fscanf(entrada,"%d ",&contas->maxi);
    //leitura das duas proximas linhas que nao serão utilizadas
    fgets(contas->titulo[5],100,entrada);
    //inicializacao propriamente dita
    contas->cliente = (pessoa*)malloc (contas->maxi*sizeof (pessoa));
    contas->quant = 0;
    contas->ini = 0;
    contas->fim = contas->maxi-1;
    return;
}

bool filaVazia(fila *contas){
    if (contas->quant == 0)
        return true;
    else
        return false;
}

int buscarNome(char nome[22],fila *contas){
    //definindo o meio da fila
    int aux = (contas->ini+(contas->quant-1)/2)%contas->maxi;
    //verificar se o nome esta na posicao aux
    int n = strcmp(nome,contas->cliente[aux].nome);
    //se a lista for vazia, ele retorna -1
    if (filaVazia(contas))
        return -1;
    else if (n== 0)//se for igual a 0, a posicao ja foi encontrada, retorna aux
        return aux;
    else if(n< 0)//se for menor, a busca eh para a esquerda
        {
            //enqnt o aux nao for o ini-1 e a o nome da posicao aux nao for igual ao procurado, ele vai descrescer 1 do aux
            while(aux!=contas->ini && strcmp(nome, contas->cliente[aux].nome)<0 )
                aux = (aux +contas->maxi-1)%contas->maxi;
            //se o aux estiver abaixo do ini, retorna posicao nao encontrada
            if(strcmp(nome, contas->cliente[aux].nome)!=0)
                return -1;
            else
                return aux;
        }
    else //senao , a busca eh para a direita
        {
            while(aux!=contas->fim && strcmp(nome, contas->cliente[aux].nome)>0 )
                aux = (aux +1)%contas->maxi;
            if(strcmp(nome, contas->cliente[aux].nome)!=0)
                return -1;
            else
                return aux;
        }
    return -1;
}

/* FUNCOES DE SAIDA DE DADOS */
void printarSaidaInicial(FILE *saida, fila *contas)
{
    fputs(contas->titulo[0],saida);
    fputs(contas->titulo[2],saida);
    fprintf(saida,"Arquivo de saida\n");
    fprintf(saida,"\n");
    fprintf(saida,"RESULTADOS DAS CONSULTAS\n");
    fprintf(saida,"\n");
}

void printarSaidaFinal(FILE *saida, fila *contas)
{
    fprintf(saida,"\n");
    fprintf(saida,"RELATORIO FINAL\n");
    fprintf(saida,"\n");
    fprintf(saida,"Tamanho maximo da lista:  %4d\n",contas->maxi);
    fprintf(saida,"Total de correntistas:    %4d\n",contas->quant);
    fprintf(saida,"Posicoes ocupadas: inicio:%4d\n",contas->ini);
    fprintf(saida,"                      fim:%4d\n",contas->fim);
    fprintf(saida,"\n");
    fprintf(saida,"CORRENTISTAS ATUAIS\n");
    fprintf(saida,"\n");
    for(int i = 0;i<contas->quant;i++)
    {//print dos correntistas atuais
        fprintf(saida, "%8.2f  %s",contas->cliente[(contas->ini+i)%contas->maxi].saldo,contas->cliente[(contas->ini+i)%contas->maxi].nome);
    }

}

/* FUNCOES DE ABERTURA DE CONTA*/
//tanto o shiftDireita quanto o shiftEsquerda ja adicionam 1 no quant e mudam o valor do ini ou fim
void shiftDireita(int aux, fila *contas)
{//da o shift para a direita desde o fim ate a posicao indicada por aux
    int i = contas->fim;
    while(i != (aux+contas->maxi-1)%contas->maxi){//o i vai parar no aux -1
       contas->cliente[(i+1)%contas->maxi]=contas->cliente[i];
       i = (i+contas->maxi-1)%contas->maxi;// seria a mesma coisa que i--, so que para fila circular
    }
    //mudar posicao do fim e do quant
    contas->fim = (contas->fim+1)%contas->maxi;//mesma coisa que fim++
    contas->quant++;

}

void shiftEsquerda(int aux, fila *contas)
{//da o shift para a esquerda desde o ini ate a posicao indicada por aux
    int i = contas->ini;
    while(i!= (aux+1)%contas->maxi){//o i vai parar no aux +1
       contas->cliente[(i+contas->maxi-1)%contas->maxi]=contas->cliente[i];//inserir valor que ta no ini no ini--
       i = (i+1)%contas->maxi;//i++
    }
    //mudar posicao do ini e do quant
    contas->ini= (contas->ini+contas->maxi-1)%contas->maxi;
    contas->quant++;
}

void abrirConta(FILE *entrada,FILE *saida, fila *contas)
{
    char nomeCliente[22];
    fgets(nomeCliente,22,entrada);
    //se a lista estiver cheia, da erro
    if(contas->quant == contas->maxi)
        erro(1,saida,"");
    else{
        //se a fila estiver vazia, a conta sera aberta na posicao 0
        bool errado = false;
        if(filaVazia(contas))
        {
            strcpy(contas->cliente[0].nome,nomeCliente);
            contas->cliente[0].saldo=0;
            contas->ini = 0;
            contas->fim = 0;
            contas->quant++;
        }
        //senao, comecar a busca pelo cliente a partir da metade da lista
        //metade da lista seria: (inicio + (quant-1)/2)% maxi
        else
        {
            int aux = (contas->ini+ (contas->quant-1)/2)%contas->maxi;
            //fazer uma comparacao entre o nomea a adicionar e nome na posicao aux.
            //se o retorno for maior que 0, o shift sera para direita e a busca sera para o resto da direita
            //se o retorno for igual a 0, da erro
            if( strcmp(nomeCliente, contas->cliente[aux].nome)>0)
            {//se entrar aqui, sabemos que o shift sera pra direita
                //esse while vai rodar o aux ate a primeira posicao que vai shiftar para direita
                while(aux != contas->fim && strcmp(nomeCliente, contas->cliente[aux].nome)>0)
                //dessa maneira, o aux vai parar no primeiro cliente a ser shiftado ou no no fim
                    aux=(aux+1)%contas->maxi;
                //se o nome do cliente for igual a onde o aux parou, da erro
                if(strcmp(nomeCliente, contas->cliente[aux].nome)==0)
                    {
                    erro(3,saida,"");
                    errado = true;
                    }
                //temos que verificar se o elemento vai ser inserido no fim da fila, se sim, temos q somar 1 no aux
                else if(aux == contas->fim &&strcmp(nomeCliente, contas->cliente[aux].nome)>0)
                    aux=(aux+1)%contas->maxi;
                if(!errado)
                {
                    shiftDireita(aux,contas);
                    strcpy(contas->cliente[aux].nome,nomeCliente);
                    contas->cliente[aux].saldo = 0;
                }
            }
            else if( strcmp(nomeCliente, contas->cliente[aux].nome)<0)
            {//se entrar aqui, sabemos que o shift sera para esquerda
                while(aux != contas->ini && strcmp(nomeCliente, contas->cliente[aux].nome)<0)
                    aux=(aux+contas->maxi-1)%contas->maxi;
                if(strcmp(nomeCliente, contas->cliente[aux].nome)==0)
                {
                    erro(3,saida,"");
                    errado = true;
                }
                else if(aux==contas->ini && strcmp(nomeCliente, contas->cliente[aux].nome)<0)
                    aux=(aux+contas->maxi-1)%contas->maxi;
                if(!errado)
                {
                    shiftEsquerda(aux,contas);
                    strcpy(contas->cliente[aux].nome,nomeCliente);
                    contas->cliente[aux].saldo = 0;
                }
            }
            //tratamento do erro
            else if( strcmp(nomeCliente, contas->cliente[aux].nome)==0)
                erro(3,saida,"");
        }
    }
}

/* FUNCOES DE DEPOSITO */
void depositar(FILE *entrada, FILE *saida, fila *contas)
{
    float valor;
    char nome[22];
    fscanf(entrada, "%f ",&valor);
    fgets(nome,22,entrada);
    // antes de depositar, tem que verificar se o nome esta na lista e sua posicao
    int posic = buscarNome(nome,contas);
    //se posic = -1, da erro
    if(posic == -1)
        erro(2,saida,nome);
    else
        contas->cliente[posic].saldo += valor;
    return;
}

/* FUNCOES DE SAQUE */
void sacar(FILE *entrada, FILE *saida, fila *contas)
{
    float valor;
    char nome[22];
    fscanf(entrada, "%f ",&valor);
    fgets(nome,22,entrada);
    // antes de depositar, tem que verificar se o nome esta na lista e sua posicao
    int posic = buscarNome(nome,contas);
    //se posic = -1, da erro
    if(posic == -1)
        erro(2,saida,nome);
    else
        contas->cliente[posic].saldo -= valor;
    return;
}

/* FUNCOES DE FECHAMENTO DE CONTA */
void shiftDireitaFechar(int posic, fila *contas)
{
    int i = posic;
    //correndo do posic ate o ini
    while(i!=contas->ini)
    {
        contas->cliente[i]=contas->cliente[(i+contas->maxi-1)%contas->maxi];
        i = (i+contas->maxi-1)%contas->maxi;
    }
    //mudar valores de ini e quant
    contas->ini=(contas->ini+1)%contas->maxi;
    contas->quant--;
}

void shiftEsquerdaFechar(int posic, fila *contas)
{
    int i = posic;
    while(i!=contas->fim)
    {
        contas->cliente[i]=contas->cliente[(i+1)%contas->maxi];
        i = (i+1)%contas->maxi;
    }
    //mudar valores de fim e quant
    contas->fim=(contas->fim+contas->maxi-1)%contas->maxi;
    contas->quant--;
}

void fecharConta(FILE *entrada, FILE *saida, fila *contas)
{
    char nome[22];
    fgets(nome,22,entrada);
    // antes de depositar, tem que verificar se o nome esta na lista e sua posicao
    int posic = buscarNome(nome,contas);
    //se posic = -1, da erro
    if(posic == -1)
        erro(2,saida,nome);
    else
    {
        int n = (contas->ini+(contas->quant-1)/2)%contas->maxi;
        //se a comparacao for menor do que 0 ou a comparacao for igual a 0 e a quantidade de elementos for PAR, o shift a partir do posic ate o ini
        if(strcmp(nome,contas->cliente[n].nome)<0 || (strcmp(nome,contas->cliente[n].nome)== 0 && (contas->quant)%2==0))
            shiftDireitaFechar(posic,contas);
        //se a comparacao for maior do que 0 ou igual a 0 e a quantidade de elemento for IMPAR, o shift sera do posic para o fim
        else
            shiftEsquerdaFechar(posic,contas);
    }
    return;
}

/* FUNCOES DE EXTRATO */
void extrato(FILE *entrada, FILE *saida, fila *contas)
{
    char nome[22];
    fgets(nome,22,entrada);
    // antes de depositar, tem que verificar se o nome esta na lista e sua posicao
    int posic = buscarNome(nome,contas);
    //se posic = -1, da erro
    if(posic == -1)
        erro(2,saida,nome);
    else
        fprintf(saida, "Saldo %8.2f  %s",contas->cliente[posic].saldo,contas->cliente[posic].nome);
    return;
}

/* LEITURA DOS DADOS */
void iniciarLeituraDados(FILE *entrada, FILE *saida, fila *contas)
{
    char operacao[15];
    do
    {
        //faz a leitura da operação
        fscanf(entrada,"%s ",operacao);
        if(strcmp(operacao,"ABRE_CONTA") == 0)
            abrirConta(entrada,saida,contas);
        else if(strcmp(operacao,"DEPOSITO") == 0)
            depositar(entrada, saida, contas);
        else if(strcmp(operacao,"SAQUE") == 0)
            sacar(entrada,saida,contas);
        else if(strcmp(operacao,"FECHA_CONTA") == 0)
            fecharConta(entrada,saida,contas);
        else if(strcmp(operacao,"EXTRATO") == 0)
            extrato(entrada,saida,contas);
    }while(strcmp(operacao,"FIM") != 0);
}

/* FIM FUNCOES */

int main()
{
    FILE *entrada, *saida;
    entrada = fopen("C:\\Lab4\\TesteAleatorio.txt","r");
    saida = fopen("C:\\Lab4\\TesteAleatorio4.txt","w");
    fila contas;
    //ler entrada inicial
    inicializarFila(entrada,&contas);
    printarSaidaInicial(saida,&contas);
    iniciarLeituraDados(entrada,saida, &contas);
    printarSaidaFinal(saida,&contas);
    fclose(entrada);
    fclose(saida);
    system("C:\\Lab4\\TesteAleatorio4.txt"); //debug
    return 0;
}

