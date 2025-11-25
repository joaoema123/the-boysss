
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conjunto.h>
#include <math.h>
#include "fila.h"
#include "eventos.h"
#include "fprio.h"


struct evento *evento_cria (int tipo, int tempo, int id_heroi, int id_missao, int id_base){

    struct evento *e = malloc (sizeof(struct evento));
    e->id_heroi = id_heroi;
    e->id_missao = id_missao;
    e->tempo = tempo;
    e->tipo = tipo;
    e->id_base= id_base;
    return e;
}

void processa_espera (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF) {
    
    int h = ev->id_heroi;
    int b = ev->id_base;
    int tempo = ev->tempo;

    // insere o heroi na fila de espera da base
    fila_insere (bases [b].espera, h);

    int tipo = AVISA;
    struct evento *novo = evento_cria (tipo, tempo, -1, -1, b);
    fprio_insere (LEF, novo, tipo, tempo);
    return;
}

void processa_desiste (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int b = ev->id_base;
    int tempo = ev->tempo;
    int N_BASES = m->NBases;

    // base aleatória para a qual o herói vai viajar
    int base_aleatoria = rand () % N_BASES;

    struct evento *novo = evento_cria (VIAJA, tempo, h, -1, base_aleatoria);

    fprio_insere (LEF, novo, VIAJA, tempo);

    return;

}

void processa_chega (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF) {
    
    bool espera;

    // pega as informações do evento chega
    int h = ev->id_heroi;
    int b = ev->id_base;
    int tempo = ev->tempo;

    // atualiza a base do heroi
    struct heroi *H = &herois [h];
    H->base = b;

    struct base *B = &bases [b];

    //decide se o herói vai ficar ou não
    if (cjto_card (B->presentes) < B->lotacao) {
        espera = true;
    }
    else 
        espera = H->paciencia > 10 * (fila_tamanho (B->espera));

    if (espera) {
        int tipo = ESPERA;
        struct evento *novo = evento_cria (ESPERA, tempo, h, -1, b);
        fprio_insere (LEF, novo, ESPERA, tempo);
        return;
    }
    
    int tipo = DESISTE;
    struct evento *novo = evento_cria (DESISTE, tempo, h, -1, b);
    fprio_insere (LEF, novo, DESISTE, tempo);
    return;
}

void processa_avisa (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF) {

    // heroi h que chegou na base b no instante tempo
    int h = ev->id_heroi;
    int b = ev->id_base;
    int tempo = ev->tempo;

    // calcula quantas vagas tem sobrando na base
    int vagas_na_base = bases [b].lotacao - cjto_card (bases [b].presentes);

    // herói que vai ser removido da fila
    int h2;

    while (vagas_na_base > 0 && fila_tamanho(bases [b].espera) > 0) {
        fila_retira (bases [b].espera, &h2);
        cjto_insere (bases [b].presentes, h2);
        struct evento *novo = evento_cria (ENTRA, tempo, h2, -1, b);
        fprio_insere (LEF, novo, ENTRA, tempo);
        vagas_na_base--;
    }

    return;
}

void processa_entra (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF) {

    int h = ev->id_heroi;
    int b = ev->id_base;
    int tempo = ev->tempo;

    // calcula o tempo de permanência na base
    int TPB = 15 + herois [h].paciencia * (1 + rand () % 20);

    struct evento *novo = evento_cria (SAI, tempo + TPB, h, -1, b);
    fprio_insere (LEF, novo, SAI, tempo + TPB);

    return;
}

void processa_sai (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int b = ev->id_base;
    int tempo = ev->tempo;

    // retira o herói h da base
    cjto_retira (bases [b].presentes, h);

    // base para a qual o herói vai fiajar
    int base_aleatoria = rand () % m->NBases;

    struct evento *novo = evento_cria (VIAJA, tempo, h, -1, base_aleatoria);
    fprio_insere (LEF, novo, VIAJA, tempo);

    struct evento *novo2 = evento_cria (AVISA, tempo, -1, -1, b);
    fprio_insere (LEF, novo2, AVISA, tempo);

    return;
}

void processa_viaja (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF) {
    
    int h = ev->id_heroi;
    int b = herois [h].base; // base de que o herói saiu
    int D = ev->id_base; // base em que o herói vai chegar
    int tempo = ev->tempo;

    // variação das coordenadas de x
    int dist_x = bases [b].localizacao [0] - bases [D].localizacao [0];
    dist_x = dist_x * dist_x;

    // variação das coordenadas de y
    int dist_y = bases [b].localizacao [1] - bases [D].localizacao [1];
    dist_y = dist_y * dist_y;

    // distância cartesiana entra a base B e a base D
    int distancia = (int) sqrt (dist_x + dist_y);
    
    // duração da viagem

    int duracao = distancia / herois [h].velocidade;

    struct evento *novo = evento_cria (CHEGA, tempo + duracao, h, -1, D);

    fprio_insere (LEF, novo, CHEGA, tempo + duracao);

    return;



}

