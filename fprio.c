// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Marcos Castilho, inclui as structs no arquivo.c, Out 2025
// Implementação com lista encadeada simples

// descreve um nodo da fila de prioridades
// NAO altere estas estruturas

#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"


// A COMPLETAR

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria (){
  struct fprio_t *ptr = malloc(sizeof (struct fprio_t));
  ptr->prim = NULL;
  ptr->num = 0;
  ptr->fim = NULL;
  return ptr;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f){
  struct fprio_t *temp = f;                                   
  struct fpnodo_t *ptr = temp->prim;
  
  while (ptr != NULL){
    free (ptr->item);
    ptr = ptr->prox;
    free (temp->prim);
    temp->prim = ptr;
  } 

  if (ptr == NULL) /*lista vazia*/
    free (f);

  f = NULL;
  return f;

}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){
    if (!f || !item) 
      return -1; // validações mínimas

    int count = 0;
    struct fpnodo_t *contador = f->prim;
    struct fpnodo_t *fim = f->prim; // vai ser usado para arrumar o f->fim
    struct fpnodo_t *novo;
    struct fpnodo_t *atual = f->prim;
    struct fpnodo_t *anterior = NULL;  

    // conta itens e verifica repetidos
    while (contador != NULL){
        if (contador->item == item) // evita item repetido
            return -1;
        contador = contador->prox;
        count++;
    }

    // colocando o malloc aqui, evita de alocar memória caso o item seja repetido
    novo = malloc(sizeof(struct fpnodo_t));

    if (!novo) return -1;

    novo->tipo = tipo;
    novo->prio = prio;
    novo->item = item;
    novo->prox = NULL;

    // lista vazia
    if (f->prim == NULL) {
        f->prim = novo;
        return count + 1;
    }

    // percorre para encontrar posição
    while (atual != NULL && prio >= atual->prio){
        anterior = atual;
        atual = atual->prox;
    }

    // insere no início
    if (anterior == NULL) {
        novo->prox = f->prim;
        f->prim = novo;
    } 
    // insere no meio ou no fim
    else {
        novo->prox = atual;
        anterior->prox = novo;
    }

    // se o fim da lista mudar, arruma o ponteiro f->fim
    while (atual->prox != NULL) 
        atual = atual->prox; 
    f->fim = atual; 

    return count + 1;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){
  
  if (!f || !tipo || !prio || f->prim == NULL) 
      return NULL;

  // Se for retirar de uma lista com um elemento, f->fim recebe NULL
  if (f->prim->prox == NULL) 
    f->fim = NULL; 
  
  struct fpnodo_t *ptr = f->prim;
  void *item = ptr->item;

  *tipo = f->prim->tipo;
  *prio = f->prim->prio;
  f->prim = ptr->prox;
  free (ptr);



  return item;

}


// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f){
  
  if (!f)
    return -1;

  if (f->prim){ /*fila não vazia*/
   
    int count = 1;
    struct fpnodo_t *ptr = f->prim;

    while (ptr->prox != NULL){
      ptr = ptr->prox;
      count++;
    }

    return count;
  }

  return 0;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f){
  struct fpnodo_t *ptr = f->prim;
  int primeiro = 1;
  
  while (ptr != NULL){
    if (!primeiro) // se não for o primeiro da um espaço
      printf (" ");
    printf ("(%d %d)", ptr->tipo, ptr->prio);
    primeiro = 0;
    ptr = ptr->prox;
  }
}
