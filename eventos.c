
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <conjunto.h>
#include <math.h>
#include "conjunto.h"
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

void processa_espera (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int tempo = ev->tempo;
    int b = ev->id_base;

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

    m->eventos_tratados++;

    printf ("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, h, b, fila_tamanho (bases [b].espera));

    return;
}

void processa_desiste (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int tempo = ev->tempo;
    int b = ev->id_base;

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

    m->eventos_tratados++;

    printf ("%6d: DESISTE HEROI %2d BASE %d\n", tempo, h, b);

    return;

}

void processa_chega (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {
    
    int h = ev->id_heroi;
    int tempo = ev->tempo;
    int b = ev->id_base;

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
        printf ("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, h, b, cjto_card (bases [b].presentes), bases [b].lotacao);
        return;
    }
    
    int tipo = DESISTE;
    struct evento *novo = evento_cria (DESISTE, tempo, h, -1, b);
    fprio_insere (LEF, novo, DESISTE, tempo);
    printf ("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, h, b, cjto_card (bases [b].presentes), bases [b].lotacao);

    return;
}

void processa_avisa (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int maior_tamanho = 0;

    int tempo =  ev->tempo, b = ev->id_base;

    // avisa que vem do evento ESPERA
    if (ev->id_heroi == -1) {

        // devolve a cada chamada da função o maior tamanho que a fila de espera já teve
        if (fila_tamanho (bases [b].espera) > maior_tamanho) {
            maior_tamanho = fila_tamanho (bases [b].espera);
            bases [b].max_fila_espera = maior_tamanho;
        }

        // calcula quantas vagas tem sobrando na base
        int vagas_na_base = bases [b].lotacao - cjto_card (bases [b].presentes);

        // herói que vai ser removido da fila
        int h2;

        printf ("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", tempo, b, cjto_card(bases [b].presentes), bases [b].lotacao);
        fila_imprime (bases [b].espera);
        printf (" ]\n");

        while (vagas_na_base > 0 && fila_tamanho(bases [b].espera) > 0) {
            fila_retira (bases [b].espera, &h2);
            cjto_insere (bases [b].presentes, h2);
            struct evento *novo = evento_cria (ENTRA, tempo, h2, -1, b);
            fprio_insere (LEF, novo, ENTRA, tempo);
            vagas_na_base--;
            printf ("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, b, h2);
            m->eventos_tratados++;
            return;

        }
    }

    // avisa que vem do evento MORRE

    int h = ev->id_heroi;

    cjto_retira (bases [b].presentes, h); // retira o herói morto da base

    m->eventos_tratados++;

    return;
}

void processa_entra (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int tempo = ev->tempo;
    int b = ev->id_base;

    int vida = herois [h].status;

    if (vida == -1) 
        return;

    int b = ev->id_base;
    int tempo = ev->tempo;

    // calcula o tempo de permanência na base
    int TPB = 15 + herois [h].paciencia * (1 + rand () % 20);

    struct evento *novo = evento_cria (SAI, tempo + TPB, h, -1, b);
    fprio_insere (LEF, novo, SAI, tempo + TPB);

    m->eventos_tratados++;

    printf ("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d)\n", tempo, h, b, cjto_card (bases [b].presentes) + 1, bases [b].lotacao);

    return;
}

void processa_sai (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int tempo = ev->tempo;
    int b = ev->id_base;

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

    m->eventos_tratados++;

    printf ("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo, h, b, cjto_card (bases [b].presentes) - 1, bases [b].lotacao);


    return;
}

void processa_viaja (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {
    

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

    m->eventos_tratados++;

    printf ("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, h, b, D, distancia, herois [h].velocidade, tempo + duracao);

    return;
}

void processa_morre (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m) {

    int h = ev->id_heroi;
    int b = ev->id_base; 
    int tempo = ev->tempo;
    int missao = ev->id_missao;

    // retira o herói h da base b
    cjto_retira (bases [b].presentes, h);

    herois [h].status = -1;

    struct evento *novo = evento_cria (AVISA, tempo, h, -1, b);

    fprio_insere (LEF, novo, AVISA, tempo);

    m->eventos_tratados++;

    printf ("%6d: MORRE HEROI %2d MISSAO %d\n", tempo, h, missao);

    return;

}

// função que verifica se determinada base consegue resolver uma missao ms
bool base_consegue_resolver (struct base b, struct missao ms, struct heroi *herois, struct mundo *m) {

    // conjunto com todas habilidades em uma base
    struct cjto_t *habilidades = cjto_cria(m->NHabilidades);

    struct cjto_t *temp;

    for (int i = 0; i < m->NHerois; i++) {
        // se o herói pertence a base, coloca no conjunto "habilidades" todas habilidades do herói
        if (cjto_pertence (b.presentes, i)) {
            temp = cjto_uniao (habilidades, herois [i].habilidades);
            cjto_destroi (habilidades);
            habilidades = temp;
        }
    }
        
    if (cjto_contem (habilidades, ms.habilidades)) {
        cjto_destroi (habilidades);
        return true;
    }
    
    cjto_destroi (habilidades);
    return false;


}

void processa_missao (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m, struct missao *ms) {

    int base_menor_distancia = -1; //base com menor distância da missão

    int base_menor_distancia2 = -1; // base com menor distância que consiga resolver a missão

    int id_missao = ev->id_missao;

    int tempo = ev->tempo;

    int menor = 1000000; // representa a menor distância entre as bases e missão

    int menor2 = 1000000; // represente a menor distância entre a missão e a base que consiga resolver a missão 

    // encontra a base mais próxima da missão
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
            base_menor_distancia = i;
            menor = distancia;
        }
    }

    // encontra a base mais próxima que consegue resolver a missão
    for (int i = 0; i < m->NBases; i++) {
        
        // variação das coordenadas de x
        int dist_x = bases [i].localizacao [0] - ms [id_missao].localizacao [0];
        dist_x = dist_x * dist_x;

        // variação das coordenadas de y
        int dist_y = bases [i].localizacao [1] - ms [id_missao].localizacao [1];
        dist_y = dist_y * dist_y;

        // distância cartesiana entra a base B e a base D
        int distancia = (int) sqrt (dist_x + dist_y);
        
        if (distancia < menor2 && base_consegue_resolver (bases [i], ms [id_missao], herois, m)) {
            base_menor_distancia2 = i;
            menor2 = distancia;
        }
     }


    // se base_menor_distancia2 = -1 não existe base que possa resolver a missao
    if (base_menor_distancia2 == -1) {
        
        // verifica se existe compostosV e se o tempo é múltiplo de 2500
        if (m->NCompostosV > 0 && (tempo % 2500 == 0)){
            
            //pensei em colocar uma condição que verifica se existem heróis na base, mas como não está no enunciado não coloquei

            //decrementa o número de compostosV no mundo
            m->NCompostosV--;
        
            // descobre qual é o herói mais experiente da base
            int melhor_heroi = -1;
            // descobre qual é a maior experiência entre os heróis
            int maior_exp = -1;
            for (int i = 0; i < m->NHerois; i++){
                if (cjto_pertence (bases [base_menor_distancia].presentes, i) && herois [i].status == 0) {
                    if (herois [i].experiencia >  maior_exp) {
                        melhor_heroi = i;
                        maior_exp = herois [i].experiencia;
                    }
                }
            }
            struct evento *novo = evento_cria (MORRE, tempo, melhor_heroi, id_missao, base_menor_distancia);
            fprio_insere (LEF, novo, MORRE, tempo);
            
            // incrementa a experiência dos demais heróis
            for (int i = 0; i < m->NHerois; i++){
                if (cjto_pertence (bases [base_menor_distancia].presentes, i) && herois [i].status == 0) {
                    herois [i].experiencia++;
                }
            }

            //aumenta o número de missões de que a base participou
            bases [base_menor_distancia].missoes_participadas++;

            m->herois_mortos++;

            m->eventos_tratados++;

            // conjunto com todas habilidades em uma base
            struct cjto_t *habilidades = cjto_cria(m->NHabilidades);
            struct cjto_t *temp;

            for (int i = 0; i < m->NHerois; i++) {
                // se o herói pertence a base, coloca no conjunto "habilidades" todas habilidades do herói
                if (cjto_pertence (bases [base_menor_distancia].presentes, i)) {
                    temp = cjto_uniao (habilidades, herois [i].habilidades);
                    cjto_destroi (habilidades);
                    habilidades = temp;
                }
            }
                

            printf ("%6d: MISSAO %d CUMPRIDA BASE %d HABS [ ", tempo, ev->id_missao, base_menor_distancia);

            cjto_imprime (habilidades);

            printf (" ]\n");
        
            return;
        }

        struct evento *novo = evento_cria (MISSAO, tempo + 24*60, -1, id_missao, -1);
        fprio_insere (LEF, novo, MISSAO, tempo + 24*60);
        m->eventos_tratados++;
        ms [id_missao].tentativas++;

        printf ("%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo, id_missao, ms [id_missao].tentativas);
        cjto_imprime (ms [id_missao].habilidades);
        printf (" ]\n");
        printf ("%6d: MISSAO %d IMPOSSÍVEL\n", tempo, id_missao);

        return;

    }
    
    // incrementa a experiência dos demais heróis
    for (int k = 0; k < m->NHerois; k++){
        if (cjto_pertence (bases [base_menor_distancia2].presentes, k) && herois [k].status == 0) {
            herois [k].experiencia++;
        }
    }

    // conjunto com todas habilidades em uma base
    struct cjto_t *habilidades = cjto_cria(m->NHabilidades);
    struct cjto_t *temp;

    for (int i = 0; i < m->NHerois; i++) {
        // se o herói pertence a base, coloca no conjunto "habilidades" todas habilidades do herói
        if (cjto_pertence (bases [base_menor_distancia].presentes, i)) {
            temp = cjto_uniao (habilidades, herois [i].habilidades);
            cjto_destroi (habilidades);
            habilidades = temp;
        }
    }
        

    printf ("%6d: MISSAO %d CUMPRIDA BASE %d HABS [ ", tempo, ev->id_missao, base_menor_distancia2);

    cjto_imprime (habilidades);

    printf (" ]\n");


    // incrementa o número de missões de que a base participou
    bases [base_menor_distancia2].missoes_participadas++;

    m->eventos_tratados++;

    return;
}

// encontra o maior número de tentativas gastas em uma missão
int maior_tentativas (struct missao *ms, struct mundo *m) {

    int maior = 0; 

    for (int i = 0; i < m->NMissoes; i++) {
        if (ms [i].tentativas > maior)
            maior = ms [i].tentativas;
    }

    return maior;
}
        

int media_tentativas (struct missao *ms, struct mundo *m) {

    long soma = 0;

    for (int i = 0; i < m->NMissoes; i++) {
        soma = soma + ms [i].tentativas;
    }

    double media = soma / m->NMissoes;

    return media;
}

void processa_fim (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m, struct missao *ms) {

    // libera a lista de eventos futuros
    fprio_destroi (LEF);

    //libera as características que foram alocadas dinamicamente dos heróis
    for (int i = 0; i < m->NHerois; i++) {
        free (herois [i].habilidades);
    }

    //libera as características que foram alocadas dinamicamente das bases
    for (int i = 0; i < m->NBases; i++) {
        fila_destroi (bases [i].espera);
        cjto_destroi (bases [i].presentes);
    }

    //libera as características que foram alocadas dinamicamente das missões
    for (int i = 0; i < m->NMissoes; i++)  {
        cjto_destroi (ms [i].habilidades);
    }

    int tempo = ev->tempo;

    printf ("%d: FIM\n\n", tempo);


    // imprime as estatísticas dos heróis
    for (int i = 0; i < m->NHerois; i++) {
        if (herois [i].status == 0) {
            printf ("HEROI %2d VIVO PAC %3d VEL %4d EXP %4d HABS []", i, herois [i].paciencia, herois [i].velocidade, herois [i].experiencia);
            cjto_imprime (herois [i].habilidades);
            printf ("]\n");
        }
        else 
        {
            printf ("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS []", i, herois [i].paciencia, herois [i].velocidade, herois [i].experiencia);
            cjto_imprime (herois [i].habilidades);
            printf ("]\n");
        }
    }

    // imprime as estatísticas das bases 
    for (int i = 0; i < m->NBases; i++) {
        // adicionar o número máximo da fila de espera e no número de missões em que a missão participou
        printf ("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", bases [i].ID, bases [i].lotacao, bases [i].max_fila_espera, bases [i].missoes_participadas);
    }

    // adicionar a quantidades de eventos tratados
    printf ("EVENTOS TRATADOS %d\n", m->eventos_tratados);

    //adicionar a quantidade de missões cumpridas
    printf ("MISSOES CUMPRIDAS %d/%d (%.1%%)\n", m->missoes_cumpridas, m->NMissoes, m->missoes_cumpridas/m->NMissoes);

    printf ("TENTATIVAS/MISSAO: MIN 1 MAX %d, MEDIA %.1f\n", maior_tentativas (ms, m), media_tentativas (ms, m));

    printf ("TAXA DE MORTALIDADE %.1%%\n", m->herois_mortos / m->NHerois);

    return;

}

