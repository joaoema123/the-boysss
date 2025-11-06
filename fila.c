#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria (){
    
    struct fila_t *L = malloc (sizeof (struct fila_t));
    if (L == NULL)
        return NULL;
    L->prim = NULL;
    L->fim = NULL;
    L->num = 0;
    return L;

}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fila_t *fila_destroi (struct fila_t *f){

    struct fila_t *temp = f;
    struct fila_nodo_t  *ptr = temp->prim;

    while (ptr != NULL)
    {
        ptr = ptr->prox;
        free (temp->prim->item);
        free (temp->prim);
        temp->prim = ptr;
    }

    if (ptr == NULL) /*lista vazia*/ 
        free (*lista);
    
    *lista = NULL;

}

// Insere o item na fila
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fila_insere (struct fila_t *f, void *item) {

}

// Retira o primeiro item da fila e o devolve
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fila_retira (struct fila_t *f) {

}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f) {

}