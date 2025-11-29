// programa principal do projeto "The Boys - 2024/2"
// Autor: Joãio Emanuel da Cruz, GRR 20250532

// seus #includes vão aqui
#include <stdlib.h>
#include <stdio.h>
#include "eventos.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main ()
{
  // inicialização do mundo

  struct mundo *m;

  int T_INICIO = 0, T_FIM_DO_MUNDO = 525600, N_TAMANHO_MUNDO = 20000, N_HABILIDADES = 10;
  int N_HEROIS = N_HABILIDADES * 5, N_BASES = N_HEROIS / 5;
  int N_MISSOES = T_FIM_DO_MUNDO / 100, N_COMPOSTOS_V = N_HABILIDADES * 3;

  struct fprio_t *LEF = fprio_cria ();

  struct heroi herois [N_HEROIS]; // vetor que contém todos heróis
  struct base bases [N_BASES]; // vetor que contém todas bases
  struct missao missoes [N_MISSOES]; // vetor que contém todas missoes

  int tam[2] = {20000, 20000};

  m->bases = bases;
  m->eventos_tratados = 0;
  m->herois = herois;
  m->herois_mortos = 0;
  m->missoes = missoes;
  m->missoes_cumpridas = 0;
  m->NBases = N_BASES;
  m->NCompostosV = N_COMPOSTOS_V;
  m->NHabilidades = N_HABILIDADES;
  m->NHerois = N_HEROIS;
  m->NMissoes = N_MISSOES;
  m->relogio = 0;
  m->TamanhoMundo [0] = tam [0];
  m->TamanhoMundo [1] = tam [1];


  //inicialização de cada herói

  srand (0);
  for (int i = 0; i < N_HEROIS; i++) {
    herois[i].status = 0;
    herois[i].experiencia = 0;
    herois[i].paciencia = rand() % 101; 
    herois[i].velocidade = 50 + rand() % 4951;
    int tam = 1 + rand() % 3; // número aleatório entre 1 e 3
    int hab = 1 + rand() % 10; // número aleatório entre 1 e 10
    herois[i].habilidades = cjto_aleat(hab, tam); // cria um conjunto de habilidades aleatórias
    herois [i].status = 0;
  }

  //inicialização de cada base

  for (int i = 0; i < N_BASES; i++) {   
    bases [i].ID = i;
    bases [i].localizacao [0] = rand () % N_TAMANHO_MUNDO;
    bases [i].localizacao [1] = rand () % N_TAMANHO_MUNDO;
    bases [i].lotacao = 3 + rand () % 8; // número entre 3 e 10
    bases [i].presentes = cjto_cria (bases [i].lotacao);
    bases [i].espera = fila_cria ();
    bases [i].missoes_participadas = 0;
    bases [i].max_fila_espera = 0;
  }
 
  //inicialização de cada missão
  for (int i = 0; i < N_MISSOES; i++) {
    missoes [i].ID = i;
    missoes [i].localizacao[0] = rand () % N_TAMANHO_MUNDO;
    missoes [i].localizacao[1] = rand () % N_TAMANHO_MUNDO; 
    int tam = 6 + rand () % 5;
    //conjunto com as habilidades necessárias para concluir a missão
    missoes [i].habilidades = cjto_aleat (10, tam); 
    missoes [i].tentativas = 0;
  }

  //cada herói chega em alguma base dentro dos primeiros três dias (4320 minutos)
  for (int i = 0; i < N_HEROIS; i++) {
    //tipo do evento
    int tipo = CHEGA;
    // base em que o herói vai estar
    int id_base = rand () % N_BASES;
    // número aleatório entre 0 e 4320
    int tempo = rand () % 4321;
    // heroi que vai chegar na base
    int id_heroi = i;

    struct evento *ev = evento_cria (CHEGA, tempo, id_heroi, -1, id_base); //missão = -1 porque não tem missão associada ao evento chega
    fprio_insere (LEF, ev, tipo, tempo);
  }

  // insere cada missão na LEF
  for (int i = 0; i < N_MISSOES; i++) {
    int tempo_missao = rand () % (T_FIM_DO_MUNDO + 1);
    int tipo = MISSAO;
    int id_heroi = -1; //porque não tem herói associado à missão
    int id_missao = i;
    int id_base = -1; //porque não tem base associada à missão
    struct evento *ev = evento_cria (MISSAO, tempo_missao, id_heroi, id_missao, id_base);
    fprio_insere (LEF, ev, tipo, tempo_missao);
  }

  
  // variável que contém o tempo desde o início da simulação

  // executar o laço de simulação
  
  // destruir o mundo

  return (0) ;
}
