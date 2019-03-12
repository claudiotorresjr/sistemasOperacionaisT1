#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// struct que representa uma lista de adjacencia
//
// possui um ponteiro para cada vertice

struct AdjList
{
	struct vertice *head;
};
typedef struct AdjList *AdjList;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"
// é um array de listas de adjacencia
// numV: numero de vertices do grafo
// numA: numero de arestas do grafo
struct grafo
{
	int numV; 
	int numA;
	struct AdjList *array;
};
typedef struct grafo *grafo;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
// 
// o vértice tem um nome, que é uma "string"

struct vertice
{
	char *nome;
	int grau;
	struct vertice *next;

};
typedef struct vertice *vertice;

//------------------------------------------------------------------------------
// cria um vertice (no) com seus atributos
// para serem inseridos na lista de adjacencia

vertice cria_vertice_lista(char *nome);

//------------------------------------------------------------------------------
// cria a estrutura de um grafo com uma lista de adjacencia
// com todos os indices apontados para NULL

grafo cria_grafo();

//------------------------------------------------------------------------------
// adiciona uma aresta entre dois vertices
// 

void add_aresta(grafo g, char *a, char *b);

//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// lê um grafo de input.
// um grafo é um arquivo onde cada linha tem zero, uma ou duas
// strings (sequência de caracteres sem "whitespace").
// 
// devolve o grafo lido. Caso o arquivo esteja mal formado o
// comportamento da função é indefinido

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// escreve o grafo g em output, no mesmo formato que o usado por le_grafo()
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o coeficiente de agrupamento de g
// ou seja, o número de tríades fechadas divido pelo 
// total de tríades (ou seja, a soma de tríades abertas e fechadas).
// 

double coeficiente_agrupamento_grafo(grafo g);

//------------------------------------------------------------------------------
#endif
