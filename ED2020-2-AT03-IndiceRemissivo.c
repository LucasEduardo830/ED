#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct NoLista *PtrNoLista;

typedef struct NoLista{

  int chave;
  PtrNoLista proximo;
} NoLista;

typedef struct{
  PtrNoLista inicio;
  int tamanho;
}Lista;

/////////////////////////////////////////////////
typedef struct{
    Lista pagina;
    char palavra[1000];
} Objeto;

typedef struct NoArvore *PtrArvore;

typedef struct NoArvore {

    Objeto elemento;
    PtrArvore left;
    PtrArvore right;
} NoArvore;
////////////////////////////////////////////////////////////////////////////////

void iniciaLista(Lista *lista){
  lista->inicio = NULL;
  lista->tamanho = 0;
}

bool estaVaziaLista(Lista *lista){
  return(lista->inicio == 0);
}

int tamanhoLista(Lista *lista){
  return(lista->tamanho);
}

bool pesquisar(Lista *lista, int key){

    if(estaVaziaLista(lista)){
      return false;
    }else{
      PtrNoLista percorre = lista->inicio;
      while(percorre != NULL && key >= percorre->chave) {
        if(percorre->chave == key) {
          return (true);
        }
        percorre = percorre->proximo;
      }
      return (false);
    }
}

void inserirLista(Lista *lista, int elemento){

  if (!pesquisar(lista, elemento)) {
    PtrNoLista novo;
      novo = (PtrNoLista)malloc(sizeof(NoLista));
      novo->chave = elemento;
    if(estaVaziaLista(lista) || elemento < lista->inicio->chave) {
      novo->proximo = lista->inicio;
      lista->inicio = novo;
    } else {
      PtrNoLista aux;
        aux = lista->inicio;
      while((aux->proximo!= NULL) && (elemento > aux->proximo->chave)) {
        aux = aux->proximo;
      }
      novo->proximo = aux->proximo;
      aux->proximo = novo;
    }
    lista->tamanho++;
  }
}

void imprimirLista(Lista *lista, FILE *arqS) {

  PtrNoLista percorre;
  for(percorre = lista->inicio; percorre != NULL; percorre = percorre->proximo) {
    fprintf(arqS, ", %d", percorre->chave);
  }
}

////////////////////////////////////////////////////////////////////////////////

void iniciaArvore(PtrArvore *arvore){
  *arvore = NULL;
}

bool estaVazia(PtrArvore *arvore){
  return(*arvore == NULL);
}

bool insereItem(PtrArvore *arvore, char *x){

    if (*arvore == NULL) {
      (*arvore) = malloc(sizeof(NoArvore));
      (*arvore)->right = NULL;
      (*arvore)->left = NULL;
      strcpy ((*arvore)->elemento.palavra, x);
      iniciaLista(&(*arvore)->elemento.pagina);
      return(true);
    }

    if(strcasecmp ((*arvore)->elemento.palavra, x) == 0) {
      return(false);
    }

    if(strcasecmp ((*arvore)->elemento.palavra, x) > 0) {
      return(insereItem(&(*arvore)->left, x));

    }else{
      return(insereItem(&(*arvore)->right, x));
    }

}

void ImprimeEmOrdem(PtrArvore *node, FILE *arqS){
  if ((*node) != NULL) {
    ImprimeEmOrdem(&(*node)->left, arqS);
    fprintf(arqS, "%s", (*node)->elemento.palavra);
    imprimirLista(&(*node)->elemento.pagina, arqS);
    fprintf(arqS, "\n");
    ImprimeEmOrdem(&(*node)->right, arqS);
  }
}

bool procuraItem(PtrArvore *node, int pag, char *string){
  if (*node == NULL) {
    return (false);
  }
  if (strcasecmp((*node)->elemento.palavra, string) == 0) {
    inserirLista(&(*node)->elemento.pagina, pag);
    return (true);
  }
  if (strcasecmp((*node)->elemento.palavra, string) > 0) {
    return(procuraItem(&(*node)->left, pag, string));

  }else{
    return(procuraItem(&(*node)->right, pag, string));
  }
}

int main(int argc, char const *argv[]) {

  if(argc!=3){
    printf(" Apenas 3 argumentos sao aceitos\n" );
    exit(1);
  }

  FILE *arqE, *arqS;
  int arquivo, pag;
  char str1[1000], *str2;

  PtrArvore raiz;

  arqE = fopen(argv[1],"r");// abre um arquivo para leitura
  arqS = fopen(argv[2],"w");// abre um arquivo para escrita
  //arqE = fopen("arquivoE.txt","r");// abre o arquivoEntrada.txt para leitura
  //arqS = fopen("arquivoS.txt","w");// abre o arquivoSaida.txt para escrita

  if(arqE == NULL || arqS == NULL ){ // Verifica se não ocorreu um erro ao tentar abrir os arquivos
    printf(" Erro ao executar o arquivo / ou arquivo inexistente\n");
    exit(1);
  }//if

    fseek(arqE, 0, SEEK_END);
  arquivo = ftell(arqE);//pega o tamanho do arquivoE
    fseek(arqE, 0, SEEK_SET);

  if (arquivo == 0) {//se o tamanho do arquivoE for igual a 0 o programa é encerrado
    printf(" O arquivo esta vazio\n");
    exit(1);
  }

  iniciaArvore(&raiz);

 // ler primeira linha com os termos

  if(fscanf(arqE,"<termos:%[^>\n]>\n", &str1)){
    str2 = strtok (str1," ,.");

  }else{
    printf(" Temos fora do padrao\n");
    exit(1);
  }

  while (str2 != NULL){
    insereItem(&raiz, str2);
      str2 = strtok (NULL, " ,.");
  }

  // ler o resto do arquivo pesquisando se a palavra é um termo presente na arvore

  while (!feof(arqE)){

    if (fscanf(arqE,"<page:%d>\n", &pag));
    else{
      printf(" Page fora dos padroes\n");
      exit(1);
    }
      fscanf(arqE,"%[^\n]\n", &str1);
        str2 = strtok (str1," ,.");

      while (str2 != NULL){

        procuraItem(&raiz, pag, str2);
        str2 = strtok (NULL, " ,.()");
      }
  }

// Imprime no arquivo as palavras pesquisadas e a pagina em que elas aparecem
  ImprimeEmOrdem(&raiz, arqS);

//fecha os arquivos
  fclose(arqE);
  fclose(arqS);

  return 0;
}
