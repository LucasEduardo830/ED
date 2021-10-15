#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


/******************************************************************************/
/******************************************************************************/

typedef struct {
    int chave;
    int desbalanceamento;
} Item;

typedef struct NoArvoreAVL *PtrArvoreAVL;

typedef struct NoArvoreAVL {
    Item elemento;
    PtrArvoreAVL right;
    PtrArvoreAVL left;
    int altura;
} NoArvoreAVL;

/******************************************************************************/
/******************************************************************************/

void iniciaArvoreAVL(PtrArvoreAVL *node){
  *node = NULL;
}

bool estaVaziaArvoreAVL(PtrArvoreAVL *node){
  return((*node) == NULL);
}

int alturaArvoreAVL(PtrArvoreAVL node){
  if(node == NULL) {
    return (0);
  } else {
    return ((node)->altura);
  }
}

int atualizaAlturaArvoreAVL(PtrArvoreAVL left, PtrArvoreAVL right){
  int ae = alturaArvoreAVL(left);
  int ad = alturaArvoreAVL(right);

  if(ae > ad) {
    return(ae + 1);
  } else {
    return(ad + 1);
  }
}

void destruirArvoreAVL(PtrArvoreAVL *node){
  if ((*node) != NULL) {
    destruirArvoreAVL(&(*node)->left);
    destruirArvoreAVL(&(*node)->right);
    free(*node);
    (*node) = NULL;
  }
}

void rotacaoSimplesEsquerda(PtrArvoreAVL *node){
  //printf("Rotacao simples para esquerda, com node: %d\n", (*node)->elemento.chave);

  PtrArvoreAVL u = (*node)->right;
  (*node)->right = u->left;
  u->left = (*node);

  //atualizar a altura dos nós modificados (node, u)
  (*node)->altura = atualizaAlturaArvoreAVL((*node)->left, (*node)->right);
  u->altura = atualizaAlturaArvoreAVL(u->left, u->right);

  //autualizacao da referencia do node
  (*node) = u;
}

void rotacaoSimplesDireita(PtrArvoreAVL *node){
  //printf("Rotacao simples para direita, com node: %d\n", (*node)->elemento.chave);

  PtrArvoreAVL u = (*node)->left;
  (*node)->left = u->right;
  u->right = (*node);

  //atualizar a altura dos nós modificados (node, u)
  (*node)->altura = atualizaAlturaArvoreAVL((*node)->left, (*node)->right);
  u->altura = atualizaAlturaArvoreAVL(u->left, u->right);

  //autualizacao da referencia do node
  (*node) = u;
}

void rotacaoDuplaEsquerda(PtrArvoreAVL *node){
  //printf("Rotacao dupla para esquerda, com node: %d\n", (*node)->elemento.chave);

  PtrArvoreAVL u, v;
  u = (*node)->right;
  v = u->left;

  (*node)->right = v->left;
  u->left = v->right;
  v->left = (*node);
  v->right = u;

  //atualizar a altura dos nós modificados (node, u, v)
  (*node)->altura = atualizaAlturaArvoreAVL((*node)->left, (*node)->right);
  u->altura = atualizaAlturaArvoreAVL(u->left, u->right);
  v->altura = atualizaAlturaArvoreAVL(v->left, v->right);

  //autualizacao da referencia do node
  (*node) = v;
}

void rotacaoDuplaDireita(PtrArvoreAVL *node){
  //printf("Rotacao dupla para direita, com node: %d\n", (*node)->elemento.chave);

  PtrArvoreAVL u, v;
  u = (*node)->left;
  v = u->right;

  (*node)->left = v->right;
  u->right = v->left;

  v->right = (*node);
  v->left = u;

  //atualizar a altura dos nós modificados (node, u, v)
  (*node)->altura = atualizaAlturaArvoreAVL((*node)->left, (*node)->right);
  u->altura = atualizaAlturaArvoreAVL(u->left, u->right);
  v->altura = atualizaAlturaArvoreAVL(v->left, v->right);

  //autualizacao da referencia do node
  (*node) = v;
}

void aplicarRotacoes(PtrArvoreAVL *node){
  int ad = alturaArvoreAVL((*node)->right);
  int ae = alturaArvoreAVL((*node)->left);

  // Verificar se é rotacao para direita
  if(ae > ad) {
    PtrArvoreAVL temp = (*node)->left;
    int temp_ad = alturaArvoreAVL(temp->right);
    int temp_ae = alturaArvoreAVL(temp->left);
    // temp_ae > ou >= temp_ad
    if(temp_ae >= temp_ad) {
      rotacaoSimplesDireita(&(*node));
    } else {
      rotacaoDuplaDireita(&(*node));
    }
  }
  // Senao é rotacao para esquerda
  else { //(ad > ae)
    PtrArvoreAVL tmp2 = (*node)->right;
    int tmp2_ad = alturaArvoreAVL(tmp2->right);
    int tmp2_ae = alturaArvoreAVL(tmp2->left);

    if(tmp2_ad >= tmp2_ae){
      rotacaoSimplesEsquerda(&(*node));
    } else {
      rotacaoDuplaEsquerda(&(*node));
    }
  }
}

bool inserirArvoreAVL(PtrArvoreAVL *node, Item x){

  if((*node) == NULL) {
    (*node) = (PtrArvoreAVL)malloc(sizeof(NoArvoreAVL));
    (*node)->right = (*node)->left = NULL;
    (*node)->elemento.chave = x.chave;
    (*node)->altura = 1;
    return true;
  }

  if((*node)->elemento.chave == x.chave) return false;

  bool auxiliar;

  if(x.chave < (*node)->elemento.chave) {
    auxiliar = inserirArvoreAVL(&(*node)->left, x);
  } else {
    auxiliar = inserirArvoreAVL(&(*node)->right, x);
  }

  if(!auxiliar) return (false);

  int ae = alturaArvoreAVL((*node)->left);
  int ad = alturaArvoreAVL((*node)->right);

  if((ae - ad) == +2 || (ae - ad) == -2) {
    aplicarRotacoes(&(*node));
  }

  (*node)->altura = atualizaAlturaArvoreAVL((*node)->left, (*node)->right);

  return(true);
}

int profundidadeArvoreAVL(PtrArvoreAVL *node) {
  if ((*node) == NULL) return 0;
  else {
    int ae = profundidadeArvoreAVL(&(*node)->left);
    int ad = profundidadeArvoreAVL(&(*node)->right);
    if (ae > ad){
      return(ae+1);

    }else{
      return(ad+1);
    }
  }
}

void printarNivelArq(PtrArvoreAVL *node, int nvl, FILE *arqS){
  if ((*node) != NULL && nvl == 0){
    int ae = alturaArvoreAVL((*node)->left);
    int ad = alturaArvoreAVL((*node)->right);

      fprintf(arqS, "%d (%d), ", (*node)->elemento.chave, ad - ae);

  }else if ((*node) != NULL){
      printarNivelArq(&(*node)->left, nvl - 1, arqS);
      printarNivelArq(&(*node)->right, nvl - 1, arqS);
  }
}

void imprimeArvoreAVLNivel(PtrArvoreAVL *node, FILE *arqS){
  int altura = profundidadeArvoreAVL(&(*node));

  for (int i = 0; i < altura; i++){
      printarNivelArq(&(*node), i, arqS);
      fprintf(arqS, "\n");
  }
}

PtrArvoreAVL MaiorElemento (PtrArvoreAVL *node) {
  PtrArvoreAVL aux;
  if ((*node)->right == NULL ) {
    aux = (*node);
    (*node) = (*node)->left;
    return (aux);
  }
  return ( MaiorElemento(&(*node)->right));
}

PtrArvoreAVL MenorElemento (PtrArvoreAVL *node) {
  PtrArvoreAVL aux;
  if ((*node)->left == NULL ) {
    aux = (* node);
    (*node) = (*node)->right;
    return (aux);
  }
  return ( MenorElemento(&(*node)->left));
}

bool removeArvoreAVL(PtrArvoreAVL *node, int key, char c) {

  bool teste;
  int ae, ad;

  if((*node) == NULL) {
    printf("Nao existe o elemento %d para ser removido!\n", key);
    return (false);
  }

  if((*node)->elemento.chave == key) {

    PtrArvoreAVL tmp = (*node);
    // case 1: sub-arvore esquerda é nula (cai aqui se for folha também)
    if((*node)->left == NULL) {
      (*node) = (*node)->right;

    }else if((*node)->right == NULL) {// case 2: sub-arvore direita é nula (poderia cair aqui tb no caso de folha)
      (*node) = (*node)->left;

    } else {// case 3: ambas subarvores existem: pegar o maior elemento da sub-arvore da esquerda
      if (c == 'e' || c == 'E') {
        tmp = MaiorElemento(&(*node)->left);
      }else{
        tmp = MenorElemento(&(*node)->right);
      }
      (*node)->elemento = tmp->elemento;
    }
    free(tmp);
    return true;
  }

  if((*node)->elemento.chave > key){
    teste = removeArvoreAVL(&(*node)->left, key, c);
  } else {
    teste = removeArvoreAVL(&(*node)->right, key, c);
  }

  if(teste == false){
     return (false);
  }else{
    ae = profundidadeArvoreAVL(&(*node)->left);
    ad = profundidadeArvoreAVL(&(*node)->right);
  if((ad - ae) == +2 || (ad - ae) == -2) {
    aplicarRotacoes(&(*node));
  }
    (*node)->altura = atualizaAlturaArvoreAVL((*node)->left, (*node)->right);
    return(true);
  }
}

int main(int argc, char const *argv[]) {

  if(argc!=3){
    printf(" Apenas 3 argumentos sao aceitos\n" );
    exit(1);
  }

  FILE *arqE, *arqS;
  int arquivo;
  char str1[1000], *str2, caracter;
  PtrArvoreAVL raiz;
  Item obj;

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

  iniciaArvoreAVL(&raiz);


// ler a linha dos numeros a serem inseridos na arvore
  fscanf(arqE,"%[^\n]\n", &str1);
    str2 = strtok (str1," ,.\n");

  while (str2 != NULL){
    obj.chave = atoi(str2);
    inserirArvoreAVL(&raiz, obj);
    str2 = strtok (NULL, " ,\n");
  }

// ler a linha dos numeros a serem removidos da arvore
  fscanf(arqE,"%[^\n]\n", &str1);
  str2 = strtok (str1," ,.\n");

// ler caractere que define qual estratégia será usada para realizar as remoções
  fscanf(arqE,"%c\n", &caracter);

  if(caracter != 'e' && caracter != 'E' && caracter != 'd' && caracter != 'D'){
    fprintf(arqS, " Caractere de estrategia de remoções invalido / Arquivo invalido\n");
    exit(1);
  }

// printa no arquivo o antes
  fprintf(arqS, "[*]antes\n");
  imprimeArvoreAVLNivel(&raiz, arqS);
  fprintf(arqS, "\n");

// remove da arvore
  while (str2 != NULL){
    obj.chave = atoi(str2);
    removeArvoreAVL(&raiz, obj.chave, caracter);
    str2 = strtok (NULL, " ,\n");
  }

// printa no arquivo o depois
  fprintf(arqS, "[*]depois\n");
    imprimeArvoreAVLNivel(&raiz, arqS);
      fprintf(arqS, "\n");

  destruirArvoreAVL(&raiz);

 //fecha os arquivos
   fclose(arqE);
   fclose(arqS);

  return 0;
}
