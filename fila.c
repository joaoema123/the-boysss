#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fila_t *fila_cria (){
    
    struct fila_t *L = malloc (sizeof (struct fila_t));

    // alocação deu errado
    if (!L)
        return NULL;

    L->prim = NULL;
    L->ult = NULL;
    L->num = 0;
    return L; // alocação deu certo

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
        free (f);
    
    f = NULL;

    return NULL;
}

// Insere um item no final da fila (politica FIFO).
// Retorno: 1 se tiver sucesso ou 0 se falhar.
int fila_insere (struct fila_t *f, int item){
   
    struct fila_nodo_t *novo = malloc (sizeof (struct fila_nodo_t));
    
    // se a alocação der errado retorna 0
    if (!novo)
        return 0;

    novo->item = item;
    novo->prox = f->prim;
    f->prim = novo;

    return 1; //alocação deu certo
}

// Retira o primeiro item da fila e o devolve
// Retorno 1 se a operação foi bem sucedida e 0 caso contrário
int fila_retira (struct fila_t *f, int *item){
    struct fila_nodo_t *aux = f->prim->prox;

    if (f->prim == NULL) // erro se lista vazia
        return 0;
    
    *item = f->prim->item;
    free (f->prim->item);
    free (f->prim);
    f->prim = aux;
    return 1;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fila_tamanho (struct fila_t *f) {

    // se a lista for vazia retorna 0
    if (f->prim == NULL)
        return 0;

    struct fila_nodo_t *ptr = f->prim;
    int count = 1;

    while (ptr->prox != NULL){
        count++;
        ptr = ptr->prox;
    }

    return count;
}

// Imprime o conteúdo da fila 
void fila_imprime (struct fila_t *f) {
    
    struct fila_nodo_t *ptr = f->prim;

    while (ptr != NULL) {
        printf ("%2d ", ptr->item);
        ptr = ptr->prox;
    }
        
}