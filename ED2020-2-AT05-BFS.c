#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXTAM 20

typedef struct {
  int inicio;
  int fim;
  int vetor[MAXTAM];
  int contador;
} FilaEstatica;

void iniciaFila(FilaEstatica *fila) {
  fila->inicio = 0;
  fila->fim = -1;
  fila->contador = 0;
}

bool filaVazia(FilaEstatica *fila){
  return(fila->contador == 0);
}

bool filaCheia(FilaEstatica *f){
  return(f->contador == MAXTAM);
}

int incrementaIndice(int i){
  return (i+1)%MAXTAM;
}

void inserirElementoFila(FilaEstatica *f, int x){
  if(filaCheia(f) == false){
    f->fim++;
    f->vetor[f->fim] = x;
    f->contador++;
  }
}

int excluirElementoFila(FilaEstatica *f){
  int x = -999;
  if(filaVazia(f) == false){
    x = f->vetor[f->inicio];
    f->inicio = incrementaIndice(f->inicio);
    f->contador--;
    return x;
  }
  return x;
}
////////////////////////////////////////////////////////////////////////////////


typedef struct{
  int rotulo;
  int pai;
  int cor; //0 = branco, 1 = cinza, 2 = preto
  int d; // 999 = infinito
}VerticeInfo;

typedef struct {
  VerticeInfo Vinfo[MAXTAM];
  int vertices;
  int **matriz;
  bool direcionado;
} GrafoMatriz;

void iniciaGrafoMatriz(GrafoMatriz *grafo, int V, bool direcionado) {

  grafo->direcionado = direcionado;
  grafo->vertices = V;

  // matrix memmory allocation
  grafo-> matriz = malloc (grafo->vertices * sizeof ( int *));
  for (int i = 0 ; i < grafo->vertices ; i ++) {
    grafo-> matriz [i] = malloc (grafo->vertices * sizeof ( int ));
  }
  // initialize matrix with 0s
  for(int i = 0; i < grafo->vertices ; i++) {
    for(int j = 0; j < grafo->vertices ; j++) {
      grafo->matriz[i][j] = 0;
    }
  }
}

void imprimeGrafoMatriz(GrafoMatriz *grafo) {

  printf("---------------\n");
  printf(" *** grafo *** \n");
  printf("---------------\n");

  printf("     ");
  for(int k = 1; k < grafo->vertices; k++) {
    printf("%d ", k);
  }
  printf("\n");

  for(int i = 1; i < grafo->vertices; i++) {
    printf("%d: | ", i);
    for(int j = 1; j < grafo->vertices ; j++) {
      printf("%d ", grafo->matriz[i][j]);
    }
    printf("\n");
  }
  printf("---------------\n");
}

void insereArestaGrafoMatriz(GrafoMatriz *grafo, int v1, int v2) {

  grafo->matriz[v1][v2] = 1;

  if(!grafo->direcionado) {
    grafo->matriz[v2][v1] = 1;
  }
}

bool verificaAdjacencia(GrafoMatriz *grafo, int v1, int v2) {
  return(grafo->matriz[v1][v2] == 1);
}

void bfsGrafoMatriz(GrafoMatriz *grafo, int s, FILE *arqS){
  FilaEstatica f;
  iniciaFila(&f);

  for (int i = 0; i < grafo->vertices ; i++) {
    grafo->Vinfo[i].rotulo = i;
    grafo->Vinfo[i].cor = 0;
    grafo->Vinfo[i].d = 999;
    grafo->Vinfo[i].pai = -1;
  }

  grafo->Vinfo[s].cor = 1;
  grafo->Vinfo[s].d = 0;
  grafo->Vinfo[s].pai = -1;

  inserirElementoFila(&f, s);

  while(f.contador != 0){
    int u = excluirElementoFila(&f);
    for (int i = 0; i < grafo->vertices; i++) {
      if(verificaAdjacencia(grafo,u,i) && grafo->Vinfo[i].cor == 0){
          grafo->Vinfo[i].cor = 1;
          grafo->Vinfo[i].d = grafo->Vinfo[u].d + 1;
          grafo->Vinfo[i].pai = u;
          inserirElementoFila(&f, i);
      }
      grafo->Vinfo[u].cor = 2;
    }
  }

  for (int i = 1; i < grafo->vertices; i++) {
    fprintf(arqS, " %d[d=%d]\n",grafo->Vinfo[i].rotulo,grafo->Vinfo[i].d );
  }
}

void destruirGrafoMatriz(GrafoMatriz *grafo) {
  for (int i = 1; i <= grafo->vertices; i++) {
     free(grafo->matriz[i]) ;
  }
  free(grafo->matriz);
}

////////////////////////////////////////////////////////////////////////////////
struct AdjListNode{
    int dest;
    struct AdjListNode* next;
};

struct AdjList{
    struct AdjListNode *head;
};

struct Graph{
    int V;
    bool directed;
    VerticeInfo VinfoL[MAXTAM];
    struct AdjList *array;
};

struct AdjListNode* newAdjListNode(int dest){
    struct AdjListNode* newNode =
     (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int V, bool direcionado){
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->directed = direcionado;

    // Create an array of adjacency lists.  Size of
    // array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by
    // making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest){
    // Add an edge from src to dest.  A new node is
    // added to the adjacency list of src.  The node
    // is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from
    // dest to src also
    if (graph->directed != true) {
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
  }
}

void printGraph(struct Graph* graph){

    for (int v = 1; v < graph->V; ++v){
        struct AdjListNode* aux = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (aux){
            printf("-> %d", aux->dest);
            aux = aux->next;
        }
        printf("\n");
    }
}

void bfsGrafoLista(struct Graph *graph, int s, FILE *arqS){
  FilaEstatica f;
  iniciaFila(&f);

  for (int i = 0; i < graph->V ; i++) {
    graph->VinfoL[i].rotulo = i;
    graph->VinfoL[i].cor = 0;
    graph->VinfoL[i].d = 999;
    graph->VinfoL[i].pai = -1;
  }

  graph->VinfoL[s].cor = 1;
  graph->VinfoL[s].d = 0;
  graph->VinfoL[s].pai = -1;

  inserirElementoFila(&f, s);

  while(f.contador != 0){
    int u = excluirElementoFila(&f);

    for (int i = 1; i < graph->V; i++) {

      struct AdjListNode *aux = graph->array[u].head;

      while (aux){
        if( graph->VinfoL[i].cor == 0 && aux->dest == i){
            graph->VinfoL[i].cor = 1;
            graph->VinfoL[i].d = graph->VinfoL[u].d + 1;
            graph->VinfoL[i].pai = u;
            inserirElementoFila(&f, i);
          }
          aux = aux->next;
        }
        graph->VinfoL[u].cor = 2;
    }
  }
  for (int i = 1; i < graph->V; i++) {
      fprintf(arqS, " %d[d=%d]\n",graph->VinfoL[i].rotulo,graph->VinfoL[i].d );
  }
}

////////////////////////////////////////////////////////////////////////////////

int main(){

    if(argc!=3){
      printf(" Apenas 3 argumentos sao aceitos\n" );
      exit(1);
    }

  FILE *arqE, *arqS;
  int arquivo, s, n1, n2, n3, control = 0;
  char str1[1000], *str2, str3[1000], *str4, c1, c2;
  GrafoMatriz g;
  struct Graph *graph;

  arqE = fopen(argv[1],"r");// abre um arquivo para leitura
  arqS = fopen(argv[2],"w");// abre um arquivo para escrita
  //arqE = fopen("arquivoE.txt","r");// abre o arquivoEntrada.txt para leitura
  //arqS = fopen("arquivoS.txt","w");// abre o arquivoSaida.txt para escrita

  if(arqE == NULL || arqS == NULL ){ // Verifica se não ocorreu um erro ao tentar abrir os arquivos
    printf(" Erro ao executar o arquivo / ou arquivo inexistente\n");
    exit(1);
  }

  fseek(arqE, 0, SEEK_END);
    arquivo = ftell(arqE);//pega o tamanho do arquivoE
  fseek(arqE, 0, SEEK_SET);

  if (arquivo == 0) {//se o tamanho do arquivoE for igual a 0 o programa é encerrado
    printf(" O arquivo esta vazio\n");
    exit(1);
  }

// Ler linha do arquivo e pegar a representação a ser usada para codificar o grafo
  fscanf(arqE,"%c\n", &c1);
    if(c1 != 'M' && c1 != 'm' && c1 != 'L' && c1 != 'l'){
      printf(" Primeira letra invalida\n");
      exit(1);
    }

// Ler a linha do arquivo e pegar o método de percurso a ser empregado
  fscanf(arqE,"%c\n", &c2);
    if(c2 != 'B' && c2 != 'b' && c2 != 'D' && c2 != 'd'){
      printf(" Segunda letra invalida\n");
      exit(1);
    }

// Ler a linha dos pares a serem inseridos no grafo
  fscanf(arqE,"%[^\n]\n", &str1);

  strcpy(str3,str1);

// Pegar o maior vertice do grafo entre 1 e 20
  str4 = strtok (str3," (),.\n");
    while (str4 != NULL){
      n3 = atoi(str4);
        str4 = strtok (NULL, " (),.\n");
        if (n3 > control) {
          control = n3;
        }
    }

  if(c1 == 'M' || c1 == 'm'){
    iniciaGrafoMatriz(&g, control+1, true);
  }else{
    graph = createGraph(control+1, true);
  }

// Inserir no grafo os pares
  str2 = strtok (str1," (),.\n");
    while (str2 != NULL){
      n1 = atoi(str2);
        str2 = strtok (NULL, " (),.\n");
      n2 = atoi(str2);
        str2 = strtok (NULL, " (),.\n");

      if(c1 == 'M' || c1 == 'm'){
        insereArestaGrafoMatriz(&g, n1, n2);
      }else{
        addEdge(graph, n1, n2);
      }
    }

// Ler a linha do arquivo e pegar o vértice inicial da busca
  fscanf(arqE,"%d", &s);
    if (s < 1 || s > 20) {
      exit(1);
    }

  if(c1 == 'M' || c1 == 'm'){
    imprimeGrafoMatriz(&g);
    bfsGrafoMatriz(&g, s, arqS);
    destruirGrafoMatriz(&g);
  }else{
    printGraph(graph);
    bfsGrafoLista(graph, s, arqS);
  }


   fclose(arqE);
   fclose(arqS);
  return 0;
}
