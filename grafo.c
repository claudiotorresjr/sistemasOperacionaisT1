#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"

/*------------------------------------------------------------------------------
        Coloca os pares de vertices na sua devida lista de adjacencia
    Recebe o grafo, cada linha lida do arquivo, o separador caso tenha duas strings
    e numero de espacos para que, caso tenha mais de um (mais de duas strings)
    essa linha nao seja utilizada.
------------------------------------------------------------------------------*/
void coloca_vertice_grafo(grafo g, char *line, unsigned int espaco, unsigned int numEspaco);

/*------------------------------------------------------------------------------
        Cria um vertice (no da lista de adjacencia)
    Recebe o nome do vertice que sera alocado e o tamanho dessa String e retorna
    o novo vertice criado
------------------------------------------------------------------------------*/
vertice cria_vertice_lista(char *nome, long unsigned int tam);

/*------------------------------------------------------------------------------
        Confere se os pares de vertices ja possuem uma aresta entre eles
    Recebe o grafo e um par de vertices e retorna 1 caso ja exista a aresta
------------------------------------------------------------------------------*/
int tem_aresta(grafo g, char *verticeA, char *verticeB);

/*------------------------------------------------------------------------------
        Cria o grafo e retorna o grafo criado
------------------------------------------------------------------------------*/
grafo cria_grafo(void);

/*------------------------------------------------------------------------------
        Adiciona uma aresta entre dois vertices (adiciona o verticeB na lista de 
    adjacencia de verticeA e vice versa)
    Caso verticeB esteja nulo (espaco em branco por exemplo), coloco o verticeA
    como "cabeca" de uma lista de adjacencia. 
------------------------------------------------------------------------------*/
void add_aresta(grafo g, char *verticeA, char *verticeB);


/*------------------------------------------------------------------------------
        Struct para a lista de adjacencia
    --> struct vertice *head; ponteiro para o inicio da lista
    --> struct vertice *tail; ponteiro para o fim da lista
------------------------------------------------------------------------------*/
struct AdjList
{
	struct vertice *head;
	struct vertice *tail;

};
typedef struct AdjList *AdjList;

/*------------------------------------------------------------------------------
        Struct para o grafo
    --> unsigned int numV; numero de vertices
	--> unsigned int numA; numero de arestas
	--> struct AdjList *array; um array de listas de adjacencias (cada indice uma lista)
------------------------------------------------------------------------------*/
struct grafo
{
	unsigned int numV; 
	unsigned int numA;
	struct AdjList *array;
};

/*------------------------------------------------------------------------------
        Struct para o vertice
    --> unsigned int grau; grau do vertice
	--> char *nome; nome do vertice
	--> struct vertice *next; ponteiro para o proximo vertice
------------------------------------------------------------------------------*/
struct vertice
{
	unsigned int grau;
	char *nome;
	struct vertice *next;

}__attribute__((packed));

/*------------------------------------------------------------------------------
       tamanho maximo do buffer (que eh o tamanho maximo de cada linha do arquivo)
------------------------------------------------------------------------------*/
#define buffSize 2050

/*------------------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------------------------*/
void coloca_vertice_grafo(grafo g, char *line, unsigned int espaco, unsigned int numEspaco)
{
	// aloco espaco para duas Strings. Cada uma representando um vertice
	char *verticeA = (char*)calloc(buffSize/2, sizeof(char));
	char *verticeB = (char*)calloc(buffSize/2, sizeof(char));

	//se nao houver o separador de Strings (espaco no caso), deixo o nome do verticeB em branco
	//e so crio o verticeA
	if(numEspaco == 0)
	{	
		strcpy(verticeA, line);					
		strcpy(verticeB, " ");
	}
	//caso contraio, o verticeA recebe tudo antes do espaco e o verticeB recebe o restante
	//como verticeA eh a primeira metade da linha, colocamos um \0 no final 
	else
	{	
		strncpy(verticeA, line, espaco);
		strncpy(verticeB, &line[espaco + 1], strlen(line));
		verticeA[strlen(verticeA)+1] = '\0';
	}
	//printf("%s %s\n", a, b);
	//o processo de inserir vertice continua somente se os nomes dos vertice forem de ate 1024 caracteres
	if(strlen(verticeA) <= 1024 && strlen(verticeB) <= 1024)
	{
		//printf("linha: %s\n", line);
		//printf("primeira coluna: %s de tamanho: %zu segunda coluna: %s de tamanho: %zu\n\n", a, strlen(a), b, strlen(b));
		
		//adiciono os vertices no grafo			
		add_aresta(g, verticeA, verticeB);

		//caso o array de lista de adjacencias esteja todo usado (numero de vertices atingiu o limite de listas criadas)
		//o array eh realocado para que caiba mais vertices (listas)
		//do modo que foi feito, a cada numero de vertices multiplo de 100, eh criado mais 100 espacos para listas
		if((g->numV%100 == 0) && (g->numV != 0))
		{
			//printf("realoquei mais 100 espacos na lista de adjacencia\n");
			g->array = (AdjList)realloc(g->array, (g->numV+100)*sizeof(struct AdjList));
			for(unsigned int k = 1; k <= 100; ++k)
			{
				g->array[g->numV+k].head = NULL;
			}
		}
	}
	else
	{
		//printf("Tamanho da String invalido\n\n");
	}
	free(verticeA);
	free(verticeB);
}

vertice cria_vertice_lista(char *nome, long unsigned int tam)
{
	//aloco espaco para um vertice e tambem para a String que representa seu nome
	vertice novo_vertice = (vertice)malloc(sizeof(struct vertice));
	novo_vertice->nome = (char*)malloc(tam*sizeof(char));
	strcpy(novo_vertice->nome, nome);
	novo_vertice->next = NULL;

	//retorna o vertice criado
	return novo_vertice;
}

int tem_aresta(grafo g, char *verticeA, char *verticeB)
{
	vertice aux;

	unsigned int i = 0;
	//printf("verificando %s com %s\n", verticeA, verticeB);
	//enquanto nao chegou ao final do array de lista de adjacencias, procuro a existencia de uma aresta
	//entre dois vertices dados
	while(g->array[i].head != NULL)
	{	
		//caso a "cabeca" de uma lista for igual ao verticeA, confiro se o verticeB esteja em sua lista
		if(strcmp(g->array[i].head->nome, verticeA) == 0)
		{
			aux = g->array[i].head->next;
			while(aux != NULL)
			{	
				//se verticeB estiver na lista de A, retorno verdadeiro, indicando que ha uma aresta entre
				//esses dois vertices
				if(strcmp(aux->nome, verticeB) == 0){
					//printf("Ja existe a aresta dos vertices %s----%s\n", verticeA, verticeB);
					return 1;
				}
				aux = aux->next;
			}
			//retorno falso caso nao tenha encontrado uma aresta
			return 0;
		}
		i++;
	}
	return 0;
}

void add_aresta(grafo g, char *verticeA, char *verticeB)
{
	vertice novo_vertice;
	
	unsigned int i = 0, achou = 0;

	//salvo o tamanho da String de cada vertice para alocar a String de tamanho correto
	long unsigned int tamA = strlen(verticeA)+1, tamB = strlen(verticeB)+1;

	//se houver uma aresta entre verticeA e verticeB, sai da funcao de adicionar aresta
	if(tem_aresta(g, verticeA, verticeB))
	{
		return;
	}
	//inserindo verticeB na lista do verticeA
	//enquanto nao foi encontrado uma posicao vazia no array de listas de adjacencia ou o elemento 
	//da "cabeca" nao for igual ao verticeA, o indice vai sendo incrementado 
	while(!achou)
	{	
		//se a "cabeca" estiver vazia, adiciono o verticeA 
		if(g->array[i].head == NULL)
		{			
			g->numV++;
			//crio um novo vertice
			novo_vertice = cria_vertice_lista(verticeA, tamA);
			g->array[i].head = novo_vertice;
			g->array[i].tail = novo_vertice;
			g->array[i].head->grau = 0;
			achou = 1;
		}
		//se o verticeA ja estiver na cabeca, adiciono o verticeB em sua lista de adjacencia
		if(strcmp(g->array[i].head->nome, verticeA) == 0 && strcmp(verticeB, " ") != 0)
		{	
			//a insercao eh feita pela cauda
			novo_vertice = cria_vertice_lista(verticeB, tamB);			
			g->array[i].tail->next = novo_vertice;
			g->array[i].tail = novo_vertice;
			g->array[i].head->grau++;
			achou = 1;
		}
		i++;
	}

	//inserindo verticeA na lista do verticeB
	//(todo comentario do while anterior serve para este pois como adicionamos verticeB na lista do verticeA, precisamos
	//adicionar o verticeA na lista do verticeB)
	i = 0, achou = 0;
	while(!achou && strcmp(verticeB, " ") != 0)
	{
		if(g->array[i].head == NULL)
		{
			g->numV++;
			novo_vertice = cria_vertice_lista(verticeB, tamB);
			g->array[i].head = novo_vertice;
			g->array[i].tail = novo_vertice;
			g->array[i].head->grau = 0;
			achou = 1;
		}
		if(strcmp(g->array[i].head->nome, verticeB) == 0)
		{	
			novo_vertice = cria_vertice_lista(verticeA, tamA);			
			g->array[i].tail->next = novo_vertice;
			g->array[i].tail = novo_vertice;
			g->array[i].head->grau++;
			achou = 1;
		}
		i++;
	}
	//incrementamos 1 no numero de arestas do grafo
	g->numA++;
}

grafo cria_grafo(void)
{
	//aloco memoria para o grafo e inicializo suas variaveis
	grafo g = (grafo)malloc(sizeof(struct grafo));
	g->numV = 0;
	g->numA = 0;

	//cria um array de listas de adjacencia
	g->array = (AdjList)malloc(100*sizeof(struct AdjList));

	//inicializa cada lista de adjacencia fazendo o no principal == NULL
	for(unsigned int i = 0; i < 100; ++i)
	{
		g->array[i].head = NULL;
		g->array[i].tail = NULL;
	}

	return g;
}

int destroi_grafo(grafo g)
{
	//para cada vertice (indice do array de listas de adjacencia) dou free() nas Strings,
	//em cada no e ao final, no array de listas e no grafo. Retorna 1 se houver sucesso.
	for(unsigned int i = 0; i < g->numV; ++i)
	{
		vertice aux, v = g->array[i].head;

		while(v != NULL)
		{
			aux = v->next;
			free(v->nome);
			free(v);
			v = aux;
		}
	}
	free(g->array);
	free(g);

	return 1;
}

grafo le_grafo(FILE *input)
{
	//recebe o grafo criado por cria_grafo()
	grafo g = cria_grafo();
	char *line = (char*)malloc(buffSize*sizeof(char));
	int ch;
	
	unsigned int j = 0, numEspaco = 0, espaco = 0;

	//enquanto houver letras no arquivo, vou atribuindo esta letra a um vetor chamado line
	while( (ch = fgetc(input)) != EOF )
	{	
		line[j] = (char)ch;
		j++;
		
		//sempre que encontrar um espaco em branco na linha, contamos o numero de ocorrencias
		//para tratar se inserimos os vertices ou nao (numEspaco) e salvamos a posicao do primeiro
		//espaco lido (espaco)
		if(ch == ' ')
		{
			numEspaco++;
			espaco = j - 1;
		}	

		//ao encontrar um \n, chegamos ao final de uma linha 
		if(ch == '\n')
		{	
			//o buffer da linha recebe \0 no final para apagar o \n e indicar final de String
			line[j - 1] = '\0';
			//se houver mais que 1 espaco, a linha eh invalida pois possui mais de duas Strings
			if(numEspaco > 1)
			{
				//printf("LINHA %s INVALIDA pois possui mais de duas Strings\n\n", line);
			}
			//se a linha estiver vazia, nao fazemos nada
			else if(line[0] == '\0')
			{
				//printf("Linha vazia\n\n");
			}
			//se houver uma ou duas Strings, criamos um vertice para cada e adicionamos no vertice
			else
			{	
				coloca_vertice_grafo(g, line, espaco, numEspaco);
			}

			free(line);
			j = 0;
			numEspaco = 0;
			line = (char*)malloc(buffSize*sizeof(char));
			
		}

	}
	//por estar sendo usado o fgetc(), se nao houver um \n no final do arquivo, a ultima linha nao eh lida.
	//por isso essa ultima linha, se existir (nao for vazia), deve ser tratada fora do while principal. 
	//eh colocado \0 no final para indicar final de String
	line[j] = '\0';
	//se o buffer nao estiver vazio, eh feito todo o tratamento do --if(ch == '\n')-- feito anteriormente
	if(line[0] != '\0')
	{
		if(numEspaco > 1)
		{
			//printf("LINHA %s INVALIDA pois possui mais de duas Strings\n\n", line);
		}
		else if(line[0] == '\0')
		{
			//printf("Linha vazia\n\n");
		}
		else
		{	
			coloca_vertice_grafo(g, line, espaco, numEspaco);
		}
	}

	free(line);
	//fechamos o arquivo que foi lido
	fclose(input);
	return g;
}

grafo escreve_grafo(FILE *output, grafo g)
{
	//caso o arquivo de saida seja vazio, avisamos o usuario e retornamos o grafo
	if(output==NULL){
		printf("output is NULL\n");
		return g;
	}

	//unsigned int v;
	vertice aux;

    //printf("\nGrafo com %d vertices e %d arestas\n\n", g->numV, g->numA);
    //for (v = 0; v < g->numV; ++v)
    //{
	//	printf("\n(vertice %s de grau %d)\n", g->array[v].head->nome, 
	//	g->array[v].head->grau);
	//	vertice vert = g->array[v].head->next;
	//	while(vert)
	//	{
	//		printf(" -> %s", vert->nome);
	//		vert = vert->next;
	//	}
	//	printf("\nhead %s and tail %s\n", g->array[v].head->nome, g->array[v].tail->nome );
	//	printf("\n");
	//}

	//para cada indice do array de listas de adjacencia, percorro vertice por vertice e cada par 
	//eh colocado em um arquivo de saida no mesmo estilo do arquivo de entrada (leitura)
	//nao foi tratado a repeticao de arestas, portanto cada aresta eh mostrada duas vezes 
	//exemplo: vertice a com vertice b e vertice b com vertice a eh mostrado como (a b) e (b a) 
	//respectivamente. (saida nao possui os parenteses)
	for(unsigned int i = 0; i < g->numV; ++i)
	{	
		aux = g->array[i].head;
		while(aux->next != NULL)
		{	
			fprintf(output, "%s %s\n", g->array[i].head->nome, aux->next->nome);
			aux = aux->next;
		}
	}
	fclose(output);
	return g;
}

double coeficiente_agrupamento_grafo(grafo g)
{
	double numTriade = 0.0, numTriadeFechada = 0.0;
	int ehVizinho = 0;
	double coeficienteAgrup;

	//se o numero de arestas e/ou numero de vertices for menor que 3, nao existe triade. Portanto retorna 0.0  
	if(g->numA < 3 || g->numV < 3)
	{
		return 0.0;
	}

	vertice aux1, aux2;

	//conferimos duas listas por vez. Esse primeiro for "segura" a primeira lista e o segundo
	//vai percorrendo todas as outras
	for(unsigned int i = 0; i < g->numV; ++i)
	{	
		for(unsigned int j = 0; j < g->numV; ++j)
		{	
			aux2 = g->array[j].head;
			//confiro nas duas listas i j se o elemento da lista i esta na lista j.
			while((aux2->next != NULL) && (!ehVizinho))
			{
				//Se estiver, isso indica que os dois vertices sao vizinhos 
				if(strcmp(aux2->next->nome, g->array[i].head->nome) == 0)
				{
					ehVizinho = 1;
				}
				aux2 = aux2->next;
			}

			//i != j para nao conferirmos uma lista com ela mesma
			if(i != j)
			{
				aux1 = g->array[i].head;

				//pegamos o primeiro elemento da primeira lista e para cada elemento da segunda lista, confiro
				//se este esta presente na primeira. 
				while(aux1->next != NULL)
				{
					aux2 = g->array[j].head;
					//
					while(aux2->next != NULL)
					{	
						//se estiver, achamos uma triade aberta. Se alem de estiver presente e os dois vertices
						//representados por essas listas forem vizinhos, encontramos uma triade fechada. 
						if(strcmp(aux1->next->nome, aux2->next->nome) == 0)
						{	
							if(ehVizinho)
							{
								numTriadeFechada++;
							}
							numTriade++;
						}
						aux2 = aux2->next;
					}
					aux1 = aux1->next;
				}
			}
			ehVizinho = 0;
		}
	}

	//numTriade = numTriade/2;
	//numTriadeFechada = numTriadeFechada/2;
	//o coeficiente de agrupamento eh a relacao do numero de triades fechadas pelo total (fechadas + abertas)
	coeficienteAgrup = numTriadeFechada/numTriade;

	//printf("NUMERO TOTAL DE TRIADES: %.0lf\n", numTriade);
	//printf("NUMERO TOTAL DE TRIADES FECHADAS: %.0lf\n", numTriadeFechada);
	//printf("COEFICIENTE DE AGRUPAMENTO: %lf\n", coeficienteAgrup);
	return coeficienteAgrup;
}