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

  int T_INICIO = 0, T_FIM_DO_MUNDO = 525600, N_TAMANHO_MUNDO = 20000, N_HABILIDADES = 10;
  int N_HEROIS = N_HABILIDADES * 5, N_BASES = N_HEROIS / 5;
  int N_MISSOES = T_FIM_DO_MUNDO / 100, N_COMPOSTOS_V = N_HABILIDADES * 3;

  struct fprio_t *LEF = fprio_cria ();

  struct heroi herois [N_HEROIS]; // vetor que contém todos heróis

  //inicialização de cada herói

  srand (0);
  for (int i = 0; i < N_HEROIS; i++) {
    herois[i].experiencia = 0;
    herois[i].paciencia = rand() % 101; 
    herois[i].velocidade = 50 + rand() % 4951;
    int tam = 1 + rand() % 3; // número aleatório entre 1 e 3
    int hab = 1 + rand() % 10; // número aleatório entre 1 e 10
    herois[i].habilidades = cjto_aleat(hab, tam); // cria um conjunto de habilidades aleatórias
  }

  //inicialização de cada base
  struct base bases [N_BASES]; // vetor que contém todas bases

  for (int i = 0; i < N_BASES; i++) {   
    bases [i].ID = i;
    bases [i].localizacao [0] = rand () % N_TAMANHO_MUNDO;
    bases [i].localizacao [1] = rand () % N_TAMANHO_MUNDO;
    bases [i].lotacao = 3 + rand () % 8; // número entre 3 e 10
    bases [i].presentes = cjto_cria (bases [i].lotacao);
    bases [i].espera = fila_cria ();
  }

  
  struct missao missoes [N_MISSOES]; // vetor que contém todas missoes

  //inicialização de cada missão
  for (int i = 0; i < N_MISSOES; i++) {
    missoes [i].ID = i;
    missoes [i].localizacao[0] = rand () % N_TAMANHO_MUNDO;
    missoes [i].localizacao[1] = rand () % N_TAMANHO_MUNDO; 
    int tam = 6 + rand () % 5;
    //conjunto com as habilidades necessárias para concluir a missão
    missoes [i].habilidades = cjto_aleat (10, tam); 
  }

  //cada herói chega em alguma base dentro dos primeiros três dias (4320 minutos)
  for (int i = 0; i < N_HEROIS; i++) {
    // base em que o herói vai estar
    int b = rand () % N_BASES;
    // número aleatório entre 0 e 4320
    int tempo = rand () % 4321;
    
  }

  // insere cada missão na LEF
  for (int i = 0; i < N_MISSOES; i++) {
    int tempo_missao = rand () % (T_FIM_DO_MUNDO + 1);
    fprio_insere (LEF, missao (tempo_missao, missoes [i]), 0, tempo_missao);
  }

  // variável que contém o tempo desde o início da simulação
  int tempo_atual = 0;

  if (tempo_atual == T_FIM_DO_MUNDO)
    fprio_insere (LEF, fim (tempo_atual), 0, tempo_atual);

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}
