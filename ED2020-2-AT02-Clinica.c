#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct  {
  int chave;
  char nome[50];
  char sexo;
  float peso;
  float altura;
} Cadastro;

typedef struct NoLista *PtrNoLista;

typedef struct NoLista {
    Cadastro elemento;
    PtrNoLista proximo;
    PtrNoLista anterior;
} NoLista;

typedef struct {
    PtrNoLista inicio;
    int tamanho;
} Lista;

void inicialista(Lista *lista){
  lista->inicio = NULL;
  lista->tamanho = 0;
}

bool estaVazia(Lista *lista){
  return(lista->inicio == 0);
}

void inserir(Lista *lista, Cadastro x){

  PtrNoLista novo = (PtrNoLista)malloc(sizeof(NoLista));
    novo->elemento = x;

  // Situação #1: Lista esta Vazia , ou
  // se o elemento que a gente quer inserir é menor do que o primeiro
  if(estaVazia(lista) || x.chave < lista->inicio->elemento.chave) {
    novo->proximo = lista->inicio;
    novo->anterior = NULL;

    if(!estaVazia(lista)){
      lista->inicio->anterior = novo;
    }

    lista->inicio = novo;

  } else {// Situacao # (Lista nao esta vazia)
    PtrNoLista aux;
      aux = lista->inicio;
    while((aux->proximo!= NULL) && (x.chave > aux->proximo->elemento.chave)) {
      aux = aux->proximo;
    }
    novo->proximo = aux->proximo;
    novo->anterior = aux;

    if(novo->proximo != NULL){
      novo->proximo->anterior = novo;
    }

    aux->proximo = novo;
  }
  lista->tamanho++;
}

int tamanholista(Lista *lista){
  return(lista->tamanho);
}

bool pesquisar(Lista *lista, int key){

    if(estaVazia(lista)){
      return false;
    }else{
      PtrNoLista percorre = lista->inicio;
      while(percorre != NULL && key >= percorre->elemento.chave) {
        if(percorre->elemento.chave == key) {
          return (true);
        }
        percorre = percorre->proximo;
      }
      return (false);
    }
}

PtrNoLista removePrimeiro(Lista *lista, Cadastro *item){
  PtrNoLista aux;

  if(lista->inicio != NULL && lista->inicio->proximo == NULL){
    aux = lista->inicio;
    lista->inicio->anterior = NULL;
    lista->inicio = NULL;
    lista->tamanho--;

  }else if(!estaVazia(lista)){
    aux = lista->inicio;
    lista->inicio = aux->proximo;
    lista->inicio->anterior = aux->anterior;
    lista->tamanho--;
  }
  return(aux);
}

PtrNoLista removeUltimo(Lista *lista, Cadastro *item){
  PtrNoLista percorre = lista->inicio;
  PtrNoLista aux,retorno;

  if (lista->inicio != NULL && lista->inicio->proximo == NULL) {
    lista->inicio = NULL;
    lista->tamanho--;

  }else if(!estaVazia(lista)){
    while(percorre->proximo != NULL) {
      aux = percorre;
      percorre = percorre->proximo;
      retorno = percorre;
    }
      aux->proximo = NULL;
      percorre->proximo = aux;
      lista->tamanho--;
  }
  return(retorno);
}

PtrNoLista removeElemento(Lista *lista, int chave, Cadastro *item){
  PtrNoLista percorre = lista->inicio;
  PtrNoLista retorno;

  if(estaVazia(lista) || chave < lista->inicio->elemento.chave || !pesquisar(lista, chave)){
    printf(" A lista esta vazia ou o elemento a ser excluido nao esta na lista. \n");
    return(false);

  }else if(chave == lista->inicio->elemento.chave){
    retorno = removePrimeiro(lista, item);

    return(retorno);
  }else{
    while(percorre->proximo != NULL && chave > percorre->proximo->elemento.chave) {
      percorre = percorre->proximo;
    }//while
      retorno = percorre->proximo;
      percorre->proximo = percorre->proximo->proximo;

      lista->tamanho--;
      return(retorno);
  }//else
}

int main(int argc, char const *argv[]) {

  if(argc!=3){
    printf(" Apenas 3 argumentos sao aceitos\n" );
    exit(1);
  }

  FILE *arqE, *arqS;
  int arquivo, linhas = 0, controle, reg = 0, busca;
  char caracter;

  Lista list;
  PtrNoLista aux;
  Cadastro paciente;

  arqE = fopen(argv[1],"r");// abre um arquivo para leitura
  arqS = fopen(argv[2],"w");// abre um arquivo para escrita
  //arqE = fopen("arquivoE.txt","r");// abre o arquivoEntrada.txt para leitura
  //arqS = fopen("arquivoS.txt","w");// abre o arquivoSaida.txt para escrita

  if(arqE == NULL || arqS == NULL ){ // Verifica se não ocorreu um erro ao tentar abrir os arquivos
    printf(" Erro ao executar o arquivo / ou arquivo inexistente\n");
    exit(1);
  }//if

  while( (caracter = fgetc(arqE)) != EOF ){
    if (caracter == '\n'){//conta quantas linhas o arquivoE possui
      linhas++;
    }//if
    if (caracter == '}') {
      reg++;
    }
    fscanf(arqE,"%d\n", &controle);
    fscanf(arqE,"%d\n", &busca);
  }//while

  arquivo = ftell(arqE);//pega o tamanho do arquivoE
    fseek(arqE, 0, SEEK_SET);

  if (arquivo == 0) {//se o tamanho do arquivoE for igual a 0 o programa é encerrado
    printf(" O arquivo esta vazio\n");
    exit(1);

  }else if ((linhas - reg) > 2) {
    printf(" Argumentos alem dos necessarios\n");
      exit(1);

  }else if (controle < 1 || controle > 3) {
    printf(" Argumento nao identificado ou inexistente\n", controle);
      exit(1);
  }

  inicialista(&list);

  while(fscanf(arqE,"{%d,%50[^,],%c,%f,%f}\n", &paciente.chave, &paciente.nome, &paciente.sexo, &paciente.peso, &paciente.altura )!=EOF && reg > 0){
    if(!isalpha(paciente.sexo)){//verifica se o caracter de paciente.sexo é uma letra
      printf(" Formato errado no arquivo\n");
      exit(1);
    }else if (pesquisar(&list, paciente.chave)) {//verifica se o codigo de paciente.chave ja existe
      printf(" Codigo de paciente ja existente ou formato errado no arquivo\n");
      exit(1);
    }
    inserir(&list ,paciente);
    reg--;
  }

  if (controle == 1) {// caso 1, imprime no arquivo em ordem crescente de codigo
    printf(" Escolha 1: Registros impressos em ordem crescente de codigo\n");
    for (int i = 0; i < tamanholista(&list);) {
      aux = removePrimeiro(&list, &paciente);
      fprintf(arqS,"{%d,%s,%c,%.1f,%.1f}\n", aux->elemento.chave, aux->elemento.nome,aux->elemento.sexo, aux->elemento.peso, aux->elemento.altura );
    }

  }else if (controle == 2) {// caso 2, imprime no arquivo em ordem decrescente de codigo
    printf(" Escolha 2: Registros impressos em ordem decrescente de codigo\n");
    for (int i = 0; i < tamanholista(&list);) {
        if (tamanholista(&list) == 1) {
          aux = removePrimeiro(&list, &paciente);
        }
        else{
          aux = removeUltimo(&list, &paciente);
        }
        fprintf(arqS,"{%d,%s,%c,%.1f,%.1f}\n", aux->elemento.chave, aux->elemento.nome,aux->elemento.sexo, aux->elemento.peso, aux->elemento.altura );
    }

  }else if (controle == 3) {// caso 3, imprime no arquivo apenas o codigo procurado, se ele existir
    printf(" Escolha 3: Consulta do registro %d e sua impressao no arquivo\n", busca);
    aux = removeElemento(&list, busca, &paciente);
    if (aux != false) {
      fprintf(arqS,"{%d,%s,%c,%.1f,%.1f}\n", aux->elemento.chave, aux->elemento.nome,aux->elemento.sexo, aux->elemento.peso, aux->elemento.altura );
    }else{
      fprintf(arqS," O codigo a ser passado ao arquivo nao esta na lista.\n");
    }
  }

//fecha os arquivos
  fclose(arqE);
  fclose(arqS);

  return 0;
}
