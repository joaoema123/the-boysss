#ifndef EVENTOS_H
#define EVENTOS_H

#define CHEGA   1
#define ESPERA  2
#define DESISTE 3
#define AVISA   4
#define ENTRA   5
#define SAI     6
#define VIAJA   7
#define MORRE   8
#define MISSAO  9
#define FIM     10

struct fprio_t;

struct heroi 
{
  int ID; // número que identifica o herói
  struct cjto_t *habilidades; // conjunto de habilidades que o herói possui
  int paciencia; // paciência do herói
  int velocidade; // velocidade de deslocamento do herói
  int experiencia; // número de missões que o heróis já participou
  int base; //ID da base onde o herói se encontra
  int status; // status = -1- > herói morto //status = 0 ->herói vivo
} ;

struct base
{
  int ID; // número inteiro que identifica a base
  int lotacao; // número máximo de heróis na base
  struct cjto_t *presentes; // ponteiro para o conjunto que contem todos heróis presentes na base
  struct fila_t *espera; // ponteiro para fila onde os heróis esperam para entrar na base
  int localizacao [2]; // par ordenado que indica as coordenadas da base
  int missoes_participadas; // número de missões em que a base participou
  int max_fila_espera; //maior número de heróis esperando na fila de espera
} ;

struct missao 
{
  int  ID; // número inteiro que identifica a missão
  struct cjto_t *habilidades; // conjunto de habilidades necessárias para realizar a missão
  int localizacao [2]; // par ordenado que indica as coordenadas da missão
  int tentativas; // número de tentativas para concluir a missão
} ;

struct mundo 
{
  int NHerois; // número total de heróis no mundo
  struct heroi *herois; // ponteiro para vetor que contem todos heróis
  int NBases; // número total de bases no mundo
  struct base *bases; // ponteiro para vetor que contem todas bases
  int NMissoes; // número total e missões a cumprir
  struct missao *missoes; // ponteiro para vetor que contem todas missões a se cumprir
  int NHabilidades; // número de habilidades distintas que existem no mundo
  int NCompostosV; // número de compostos V disponíveis
  int TamanhoMundo [2]; // coordenadas máximas do plano cartesiano que representam o tamanho do mundo
  int relogio; // indica o tempo atual do mundo
  int eventos_tratados; // quantidade de eventos tratados durante a simulação
  int missoes_cumpridas; // número de missões cumpridas durante a simulação
  int herois_mortos; // número de heróis que morreram durante a simulação
} ; 

struct evento {

  int tipo; // 1 CHEGA, 2 ESPERA ...
  int tempo;
  int id_heroi;
  int id_missao;
  int id_base;
} ;

struct evento *evento_cria (int tipo, int tempo, int id_heroi, int id_missao, int id_base);

void processa_chega (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_espera (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_desiste (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_avisa (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_entra (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_sai (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_viaja (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_morre (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m);

void processa_missao (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m, struct missao *ms);

void processa_fim (struct evento *ev, struct heroi *herois, struct base *bases, struct fprio_t *LEF, struct mundo *m, struct missao *ms);

#endif
