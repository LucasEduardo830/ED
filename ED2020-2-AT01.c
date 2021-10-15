#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

////////////////////   PILHA ESTATICA   ///////////////////////

#define MAXTAM 100


typedef struct {
  int valorE;
} ObjetoE;

typedef struct {
  ObjetoE vetor[MAXTAM];
  int topoE;
} pilhaEstatica;

void iniciaPilhaEstatica(pilhaEstatica *p) {
  p->topoE = 0;
}

bool estaVaziaEstatica(pilhaEstatica *p) {
  return(p->topoE == 0);
}

bool estaCheiaEstatica(pilhaEstatica *p) {
  return(p->topoE == MAXTAM);
}

void empilhaEstatica(ObjetoE item, pilhaEstatica *p) {
  if(!estaCheiaEstatica(p)){
    p->vetor[p->topoE] = item;
    p->topoE++;
  } else {
    printf(" Pilha cheia\n");
  }
 }

int desempilhaEstatica(ObjetoE *item, pilhaEstatica *p) {
  if(!estaVaziaEstatica(p)){
    *item = p->vetor[p->topoE-1];
    p->topoE--;
  } else{
    printf(" Pilha vazia\n");
  }
  return(item->valorE);
}

int tamanhoPilhaEstatica(pilhaEstatica *p) {
  return(p->topoE);
}

ObjetoE topoEstatica(pilhaEstatica *p) {
   return(p->vetor[p->topoE-1]);
}

void imprimePilhaEstatica(pilhaEstatica *p) {
  int i = 0;
  printf("\n Pilha = {");
  for(i = 0; i < p->topoE; i++) {
    printf("|%d| ", p->vetor[i]);
  }
  printf("}\n");
}


//////////////////////          PILHA DINAMICA      /////////////////////////



typedef struct NoPilha* PtrNoPilha;

typedef struct NoPilha {
  int valorD;
  PtrNoPilha proximo; // struct NoPilha*
} NoPilha;

typedef struct {
  PtrNoPilha topoD;   // struct NoPilha*
  int tamanhoD;
} PilhaDinamica;

void iniciaPilhaDinamica(PilhaDinamica *pd) {
  pd->topoD = NULL;
  pd->tamanhoD = 0;
}

bool estaVaziaDinamica(PilhaDinamica *pd) {
  return(pd->tamanhoD == 0);
}

void empilhaDinamica(PilhaDinamica *pd, int valor) {
  PtrNoPilha aux;
    aux = (PtrNoPilha)malloc(sizeof(PtrNoPilha));
      aux->valorD = valor;
      aux->proximo = pd->topoD;
      pd->topoD = aux;
      pd->tamanhoD++;
 }

int desempilhaDinamica(PilhaDinamica *pd) {
  int v = -999;
  if(!estaVaziaDinamica(pd)){
    PtrNoPilha aux;
      aux = pd->topoD;
      v = aux->valorD;
      pd->topoD = aux->proximo;
      free(aux);
      pd->tamanhoD--;
  } else{
    printf("Pilha esta vazia\n");
  }
    return(v);
}

int tamanhoPilhaDinamica(PilhaDinamica *pd) {
  return(pd->tamanhoD);
}

void imprimePilhaDinamica(PilhaDinamica *pd) {
  PtrNoPilha ptr;
    printf("\n Pilha = {");
      for(ptr = pd->topoD; ptr != NULL; ptr = ptr->proximo) {
        printf("|%d| ", ptr->valorD);
      }
    printf("}\n");
}

////////////////////     MAIN     //////////////////////////////


int main(int argc, char const *argv[]) {

  if(argc!=3){
    printf(" Apenas 3 argumentos sao aceitos\n" );
    exit(1);
  }

  FILE *arqE, *arqS;
  int retorno, arquivo, linhas = 0, contador = 0;
  char caracter;

  arqE = fopen(argv[1],"r");// abre um arquivo para leitura
  arqS = fopen(argv[2],"w");// abre um arquivo para escrita
  //arqE = fopen("arquivoE.txt","r");// abre o arquivoEntrada.txt para leitura
  //arqS = fopen("arquivoS.txt","w");// abre o arquivoSaida.txt para escrita

  if(arqE == NULL || arqS == NULL ){ // Verifica se não ocorreu um erro ao tentar abrir os arquivos
    printf(" Erro ao executar o arquivo / ou arquivo inexistente\n");
    exit(1);
  }//if

  fseek (arqE, 0, SEEK_END);
    arquivo = ftell(arqE);//pega o tamanho do arquivoE

  if (arquivo == 0) {//se o tamanho do arquivoE for igual a 0 o programa é encerrado
    printf(" O arquivo esta vazio\n");
    exit(1);
  }

  fseek(arqE, 0, SEEK_SET);

  while( (caracter = fgetc(arqE)) != EOF ){
      if(ispunct(caracter)){  //verifica se os numeros no arquivoE são inteiros, caso contrario encerra o programa
        printf(" Erro: Numeros nao inteiros presentes no arquivo\n");
        exit(1);
      }
      if(isalpha(caracter)){//verifica se o caracter atual é uma letra
        contador++;
      }
      if (contador > 1){//se o arquivoE possuir mais que uma letra o programa é encerrado
        printf(" Letras alem das necessarias no arquivo\n");
        exit(1);
      }
      if (caracter=='\n'){//conta quantas linhas o arquivoE possui
        linhas++;
      }//if
  }//while
  fseek(arqE, 0, SEEK_SET);

  while( (caracter = fgetc(arqE)) != EOF ){
    if(caracter != 'e' & caracter != 'd'){//verifica se o primeiro caracter é igual 'e' ou 'd', caso contrario encerra o programa
      printf(" Primeiro caracter diferente de 'e' ou 'd'\n");
      //fecha os arquivos
      fclose(arqE);
      fclose(arqS);
      exit(1);
    }else if (caracter == 'e') {

      printf(" Pilha estatica\n");

      pilhaEstatica pilhaE1, pilhaE2;
      ObjetoE oE;

      iniciaPilhaEstatica(&pilhaE1);
      iniciaPilhaEstatica(&pilhaE2);

      while(!feof(arqE)){
        fscanf(arqE,"%d", &oE.valorE);
          empilhaEstatica(oE ,&pilhaE1);
      }//while

      //Para correção do tamanho ocupado na pilha
      while(linhas <= tamanhoPilhaEstatica(&pilhaE1)){
        retorno = desempilhaEstatica(&oE, &pilhaE1);
      }//while

      imprimePilhaEstatica(&pilhaE1);
        printf(" Tamanho pilha %d\n", tamanhoPilhaEstatica(&pilhaE1));

      for (int i = 0; i < tamanhoPilhaEstatica(&pilhaE1);) {

        retorno = desempilhaEstatica(&oE, &pilhaE1);//recebe em ordem contraria a de entreda os valores que estavam no pilha

        if(oE.valorE == 0){// se o retorno for 0 o binario sera 0
            fprintf(arqS, "%d", retorno);
        }else{
          while(retorno > 0){
            oE.valorE = retorno % 2;//pega o resto 1 ou 0 da divisão por 2
              retorno = retorno / 2;
                empilhaEstatica(oE,&pilhaE2);
          }//while
        }//if else
          while(!estaVaziaEstatica(&pilhaE2)){
            oE.valorE = desempilhaEstatica(&oE, &pilhaE2);//recebe em ordem contraria a de entreda os valores que estavam no pilha
              fprintf(arqS, "%d", oE.valorE);
          }//while
          fprintf(arqS, "\n");
      }//for

      imprimePilhaEstatica(&pilhaE1);
        printf(" Tamanho pilhaE1 %d\n", tamanhoPilhaEstatica(&pilhaE1));
      imprimePilhaEstatica(&pilhaE2);
        printf(" Tamanho pilhaE2 %d\n", tamanhoPilhaEstatica(&pilhaE2));

    }////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      else if(caracter == 'd'){

      printf(" Pilha dinamica\n");

      PilhaDinamica pilhaD1, pilhaD2;
        int aux;

        iniciaPilhaDinamica(&pilhaD1);
        iniciaPilhaDinamica(&pilhaD2);

        while(!feof(arqE)){
          fscanf(arqE,"%d", &aux);
            empilhaDinamica(&pilhaD1, aux);
        }//while

        //Para correção do tamanho ocupado na pilha
        while(linhas <= tamanhoPilhaDinamica(&pilhaD1)){
          retorno = desempilhaDinamica(&pilhaD1);
        }//while

        imprimePilhaDinamica(&pilhaD1);
          printf(" Tamanho pilha %d\n", tamanhoPilhaDinamica(&pilhaD1));

        for (int i = 0; i < tamanhoPilhaDinamica(&pilhaD1);) {

          retorno = desempilhaDinamica(&pilhaD1);//recebe em ordem contraria a de entreda os valores que estavam no pilha

          if(retorno == 0){// se o retorno for 0 o binario sera 0
              fprintf(arqS, "%d", retorno);
          }else{
            while(retorno > 0){
              aux = retorno % 2;//pega o resto 1 ou 0 da divisão por 2
                retorno = retorno / 2;
                  empilhaDinamica(&pilhaD2, aux);
            }//while
          }//if else
            while(!estaVaziaDinamica(&pilhaD2)){
              aux = desempilhaDinamica(&pilhaD2);//recebe em ordem contraria a de entreda os valores que estavam no pilha
                fprintf(arqS, "%d", aux);
            }//while
            fprintf(arqS, "\n");
      }//while

      imprimePilhaDinamica(&pilhaD1);
        printf(" Tamanho pilhaD1 %d\n", tamanhoPilhaDinamica(&pilhaD1));
      imprimePilhaDinamica(&pilhaD2);
        printf(" Tamanho pilhaD2 %d\n", tamanhoPilhaDinamica(&pilhaD2));

    }
  }//while

//fecha os arquivos
  fclose(arqE);
  fclose(arqS);

  return 0;
}
