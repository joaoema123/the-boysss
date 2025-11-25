#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

// Cria uma fila vazia.
struct fila_t *fila_cria() {
    struct fila_t *f = malloc(sizeof(struct fila_t));
    if (!f) return NULL;

    f->prim = NULL;
    f->ult = NULL;
    f->num = 0;
    return f;
}

// Destroi a fila e os nodos.
struct fila_t *fila_destroi(struct fila_t *f) {
    if (!f) return NULL;

    struct fila_nodo_t *p = f->prim;
    while (p != NULL) {
        struct fila_nodo_t *prox = p->prox;
        free(p);    // item é int, não é ponteiro -> não dar free(p->item)
        p = prox;
    }

    free(f);
    return NULL;
}

// Insere no final da fila (FIFO)
int fila_insere(struct fila_t *f, int item) {

    struct fila_nodo_t *novo = malloc(sizeof(struct fila_nodo_t));
    if (!novo) return 0;

    novo->item = item;
    novo->prox = NULL;

    if (f->ult == NULL) {   // fila vazia
        f->prim = novo;
        f->ult = novo;
    } else {
        f->ult->prox = novo;
        f->ult = novo;
    }

    f->num++;
    return 1;
}

// Retira do início da fila (FIFO)
int fila_retira(struct fila_t *f, int *item) {

    if (f->prim == NULL)  // fila vazia
        return 0;

    struct fila_nodo_t *p = f->prim;
    *item = p->item;

    f->prim = p->prox;

    if (f->prim == NULL)
        f->ult = NULL;

    free(p);
    f->num--;
    return 1;
}

// Retorna o número de itens
int fila_tamanho(struct fila_t *f) {
    if (!f) return -1;
    return f->num;
}

void fila_imprime(struct fila_t *f) {
    struct fila_nodo_t *p = f->prim;
    while (p != NULL) {
        printf("%d ", p->item);
        p = p->prox;
    }
}
