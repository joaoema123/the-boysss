
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

    int vida = herois [h].status;

    if (vida == -1) 
        return;

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

    int vida = herois [h].status;

    if (vida == -1) 
        return;

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
    
    int h = ev->id_heroi;

    int vida = herois [h].status;

    if (vida == -1) 
        return;

    bool espera;

    // pega as informações do evento chega
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

    int h = ev->id_heroi;

    int vida = herois [h].status;

    if (vida == -1) 
        return;

    // heroi h que chegou na base b no instante tempo
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

    int vida = herois [h].status;

    if (vida == -1) 
        return;

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

    int vida = herois [h].status;

    if (vida == -1) 
        return;

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

    int vida = herois [h].status;

    if (vida == -1) 
        return;

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

void processa_morre (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF) {

    int h = ev->id_heroi;
    int b = ev->id_base; 
    int tempo = ev->tempo;

    // retira o herói h da base b
    cjto_retira (bases [b].presentes, h);

    herois [h].status = -1;

    struct evento *novo = evento_cria (AVISA, tempo, -1, -1, b);

    fprio_insere (LEF, novo, AVISA, tempo);

}

// função que verifica se determinada base consegue resolver uma missao ms
bool base_consegue_resolver (struct base b, struct missao ms) {

}

void processa_missao (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m, struct missao *ms) {

    int base_menor_d; //base com menor distância da função

    int id_missao = ev->id_missao;

    int tempo = ev->tempo;

    int menor = 1000000;

    int b = -1;

    for (int i = 0; i < m->NBases; i++) {
        
        // variação das coordenadas de x
        int dist_x = bases [i].localizacao [0] - ms [id_missao].localizacao [0];
        dist_x = dist_x * dist_x;

        // variação das coordenadas de y
        int dist_y = bases [i].localizacao [1] - ms [id_missao].localizacao [1];
        dist_y = dist_y * dist_y;

        // distância cartesiana entra a base B e a base D
        int distancia = (int) sqrt (dist_x + dist_y);
        
        //encontra a base com menor distância
        if (distancia < menor) {
            base_menor_d = i;
            menor = distancia;
        }

        // encontra a base com menor distância que tem todas habilidades para resolver a missão
        if (distancia < menor && base_consegue_resolver (bases [i], ms [id_missao])) {
            b = i;
        }
    }
   
    // se b = -1 não existe base que possa resolver a missao
    if (b == -1) {
        
        // verifica se existe compostosV e se o tempo é múltiplo de 2500
        if (m->NCompostosV > 0 && (tempo % 2500 == 0)){
        
            //decrementa o número de compostosV no mundo
            m->NCompostosV--;
            fprio_retira (LEF, ev, &tempo); //marca  a missão como concluída
        
            // descobre qual é o herói mais experiente da base
            int maior = 0;
            for (int j = 0; j < m->NHerois; j++){
                if (cjto_pertence (bases [base_menor_d].presentes, j)) {
                    if (herois [j].experiencia > maior)
                        maior = herois [j].experiencia;
                }
            }
            struct evento *novo = evento_cria (MORRE, tempo, maior, id_missao, base_menor_d);
            fprio_insere (LEF, novo, MORRE, tempo);
            
            // incrementa a experiência dos demais heróis
            for (int j = 0; j < m->NHerois; j++){
                if (cjto_pertence (bases [base_menor_d].presentes, j)) {
                    herois [j].experiencia++;
                }
            }
        
            return;
        }

        struct evento *novo = evento_cria (MISSAO, tempo + 24*60, -1, id_missao, -1);
        fprio_insere (LEF, novo, MISSAO, tempo + 24*60);
        return;

    }
    
    fprio_retira (LEF, ev, &tempo); //marca  a missão como concluída
    
    // incrementa a experiência dos demais heróis
    for (int k = 0; k < m->NHerois; k++){
        if (cjto_pertence (bases [b].presentes, k)) {
            herois [k].experiencia++;
        }
    }
    return;
}
        


