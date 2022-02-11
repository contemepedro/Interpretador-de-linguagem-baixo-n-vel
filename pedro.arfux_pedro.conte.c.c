/**************************************************
*
* Pedro Arfux Pereira Cavalcante de Castro
* Pedro Henrique Conte de Araujo
* Trabalho 2
* Professor(a): Diego Padilha Rubert
*
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define MAX_OP 3
#define MAX_LINE 128

typedef struct instr{ 
	char op[MAX_OP+1]; /* Operação */
  char reg1[MAX_OP+1]; /* Registrador do operando 1, se registrador */
  int val1; /* Valor do operando 1, se inteiro */
  char reg2[MAX_OP+1]; /* Registrador do operando 2, se houver */
	struct instr *prev; /* Anterior */
  struct instr *next; /* Próximo */
  
}instruction; 

void Insere_final(instruction **cabeca, char *operacao, char *registrador1, char *registrador2);
void Imprime_lista(instruction *lista);
void Executa_cap(instruction *lista);
void Apaga_lista(instruction **cabeca);

int main(int argc, char *argv[]) { 
  FILE *fp = fopen(argv[1], "r");
  char linha[MAX_LINE+1], operacao[MAX_OP+1], registrador1[MAX_LINE], registrador2[MAX_OP+1];
	instruction *cabeca = NULL; 
  //lê linha a linha do código .cap
  while(fgets(linha, MAX_LINE+1, (FILE *)fp) != NULL){
    int i=0, j=3;
    char *palavra = strtok(linha," ");
    //divide a linha em palavras. para se achar linha vazia, comentário ou fim de comando
    while(palavra!=NULL && strcmp(palavra, "#")!=0 && i < j && linha!=NULL && linha[0]!='\n'){
      if(i==0){
        //armazena a operação
        strcpy(operacao, palavra);
        //se o primeiro comando é "mov", temos 3 palavras. caso contrário temos duas
        if(strcmp(palavra, "mov")!=0)
          j--;
      }else if(i==1){
        //armazena o primeiro registrador
        strcpy(registrador1, palavra);
      }else{
        //armazena o segundo registrador
        strcpy(registrador2, palavra);
      }
      palavra = strtok(NULL, " ");
      i++;
    }
    //se i é diferente de zero, então há algum comando a ser lido
    if(i!=0 && operacao != ""){
      Insere_final(&cabeca, operacao, registrador1, registrador2);
    }
  }

  //descomentar para imprimir lista completa
	printf("lista: \n"); 
	Imprime_lista(cabeca); 
  printf("\n\n");

  //executa os comandos da lista
  //Executa_cap(cabeca);

  //apaga TODA a lista
  Apaga_lista(&cabeca);

  //descomentar para imprimir lista vazia
	// printf("lista: \n"); 
	// Imprime_lista(cabeca); 
  // printf("\n\n");

	return 0; 
} 

void Insere_final(instruction **cabeca, char *operacao, char *registrador1, char *registrador2){ 
	instruction *nova = (instruction *)malloc(sizeof(instruction)); 
	instruction *ultima = *cabeca;
  //verifica se há um \n colado a algum registro, se houver, substitui por \0
  if(registrador1[strlen(registrador1)-1]=='\n'){
    registrador1[strlen(registrador1)-1] = '\0';
  }
  if(registrador2[strlen(registrador2)-1]=='\n'){
    registrador2[strlen(registrador2)-1] = '\0';
  }
  //coloca dados na lista
  strcpy(nova->op, operacao);
  //verifica se registrador1 é um inteiro ou registro
  if(strcmp(registrador1, "acc")==0 || strcmp(registrador1, "dat")==0 || strcmp(registrador1, "ext")==0 || strcmp(registrador1, "pc")==0){
    strcpy(nova->reg1, registrador1);
    nova->val1 = -1;
  }else{
    nova->val1 = atoi(registrador1); 
    strcpy(nova->reg1, "\0");
  }
  //insere reg2 na lista, caso exista
  if(strcmp(nova->op, "mov")==0){
    strcpy(nova->reg2, registrador2);
  }else{
    strcpy(nova->reg2, "\0");
  }
  //nova célula aponta para NULL
	nova->next = NULL; 
  //se a lista não possuir cabeça, define a nova célula como cabeça
	if(*cabeca==NULL){
		nova->prev = NULL; 
		*cabeca = nova;  
  //senão, vai até o final da lista e atualiza o prev e next das duas ultimas celulas
	}else{
    while(ultima->next!=NULL) 
      ultima = ultima->next; 
    ultima->next = nova; 
    nova->prev = ultima; 
  }
}

void Imprime_lista(instruction* lista){ 
	while(lista!=NULL){ 
    printf("cel: /%s/ ", lista->op);
    printf("/%s/ ", lista->reg1);
    printf("/%d/ ", lista->val1);
		printf("/%s/", lista->reg2);
    printf("\n");
		lista = lista->next; 
	} 
} 

void Executa_cap(instruction *lista){
  int acc=0, dat=0, ext=0, pc=0, temp1=0;//registradores
	instruction *p = lista;

  while(p->next != NULL){

    if(strcmp(lista->reg1, "\0")==0){
      temp1 = lista->val1;
    }else if(strcmp(lista->reg1, "acc")==0){
      temp1 = acc;
    }else if (strcmp(lista->reg1, "dat")==0){
      temp1 = dat;
    }else if (strcmp(lista->reg1, "ext")==0){
      temp1 = ext;
    }else if (strcmp(lista->reg1, "pc")==0){
      temp1 = pc;
    }

    if(strcmp(lista->op, "mov")==0){
      if(strcmp(lista->reg2, "acc")==0){
        acc = temp1;
      }else if (strcmp(lista->reg2, "dat")==0){
        dat = temp1;
      }else if (strcmp(lista->reg2, "ext")==0){
        ext = temp1;
      }
      lista = lista->next;
      pc++; 
    }else if(strcmp(lista->op, "add")==0){
      acc += temp1;
      lista = lista->next;
      pc++; 
    }else if(strcmp(lista->op, "sub")==0){
      acc -= temp1;
      lista = lista->next;
      pc++; 
    }else if(strcmp(lista->op, "mul")==0){
      acc *= temp1;
      lista = lista->next;
      pc++; 
    }else if(strcmp(lista->op, "div")==0){
      acc /= temp1;
      lista = lista->next;
      pc++; 
    }else if(strcmp(lista->op, "mod")==0){
      acc = acc%temp1;
      lista = lista->next;
      pc++; 
    }else if(strcmp(lista->op, "jmp")==0){
      if(temp1 > 0){
        for(int i=0; i<temp1; i++){
          lista = lista->next;
          pc++; 
        }
      }else if(temp1 < 0){
        for(int i=temp1; i<0; i++){
          lista = lista->prev;
          pc--;
        }
      }
    }else if(strcmp(lista->op, "jeq")==0){
      if(acc == 0){
        if(temp1 > 0){
          for(int i=0; i<temp1; i++){
            lista = lista->next;
            pc++; 
          }
        }else if(temp1 < 0){
          for(int i=temp1; i<0; i++){
            lista = lista->prev;
            pc--;
          }
        }
      }else{
        lista = lista->next;
        pc++; 
      }

    }else if(strcmp(lista->op, "jlt")==0){
      if(acc < 0){
        if(temp1 > 0){
          for(int i=0; i<temp1; i++){
            lista = lista->next;
            pc++; 
          }
        }else if(temp1 < 0){
          for(int i=temp1; i<0; i++){
            lista = lista->prev;
            pc--;
          }
        }
      }else{
        lista = lista->next;
        pc++; 
      }

    }else if(strcmp(lista->op, "jgt")==0){
      if(acc > 0){
        if(temp1 > 0){
          for(int i=0; i<temp1; i++){
            lista = lista->next;
            pc++; 
          }
        }else if(temp1 < 0){
          for(int i=temp1; i<0; i++){
            lista = lista->prev;
            pc--;
          }
        }
      }else{
        lista = lista->next;
        pc++; 
      }
      
    }else if(strcmp(lista->op, "prt")==0){
      printf("%d\n", temp1);
      lista = lista->next;
      pc++; 

    }else{
      lista = lista->next;
    }
  }
}

void Apaga_lista(instruction **cabeca){
  instruction *atual = *cabeca;
  instruction *proximo;

  while(atual != NULL){
    proximo = atual->next;
    free(atual);
    atual = proximo;
  }

  *cabeca = NULL;
}