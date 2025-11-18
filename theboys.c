// programa principal do projeto "The Boys - 2024/2"
// Autor: Joãio Emanuel da Cruz, GRR 20250532

// seus #includes vão aqui
#include <stdlib.h>
#include <stdio.h>
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"
// seus #defines vão aqui
struct heroi 
{
  int ID; // número que identifica o herói
  struct cjto_t habilidades; // conjunto de habilidades que o herói possui
  int paciencia; // paciência do herói
  int velocidade; // velocidade de deslocamento do herói
  int experiencia; // número de missões que o heróis já participou
  int base; //ID da base onde o herói se encontra
} ;

struct base
{
  int ID; // número inteiro que identifica a base
  int lotaçao; // número máximo de heróis na base
  int *presentes; // ponteiro para vetor que contem todos heróis presentes na base
  struct fila_nodo_t espera; // fila onde os heróis esperam para entrar na base
  int localizacao [2]; // par ordenado que indica as coordenadas da base
} ;

struct missao 
{
  int  ID; // número inteiro que identifica a missão
  struct cjto_t habilidades; // conjunto de habilidades necessárias para realizar a missão
  int localizacao [2]; // par ordenado que indica as coordenadas da missão
} ;

struct mundo 
{
  int NHerois; // número total de heróis no mundo
  int *herois; // ponteiro para vetor que contem todos heróis
  int NBases; // número total de bases no mundo
  int *bases; // ponteiro para vetor que contem todas bases
  int NMissoes; // número total e missões a cumprir
  int *missoes; // ponteiro para vetor que contem todas missões a se cumprir
  int NHabilidades; // número de habilidades distintas que existem no mundo
  int NCompostosV; // número de compostos V disponíveis
  int TamanhoMundo [2]; // coordenadas máximas do plano cartesiano que representam o tamanho do mundo
  int relogio; // indica o tempo atual do mundo
} ; 
// minimize o uso de variáveis globais

// programa principal
int main ()
{
  // iniciar o mundo

  // executar o laço de simulação

  // destruir o mundo

  return (0) ;
}

