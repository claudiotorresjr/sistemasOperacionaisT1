#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grafo.h"

#define buffSize 2050

vertice cria_vertice_lista(char *nome)
{
	vertice novo_vertice = (vertice)malloc(sizeof(vertice));
	novo_vertice->nome = (char*)calloc(buffSize/2, sizeof(char));
	strcpy(novo_vertice->nome, nome);
	novo_vertice->next = NULL;

	return novo_vertice;
}

void add_aresta(grafo g, char *a, char *b)
{
	//cria uma aresta de a em b. Um novo no eh colocado
	//na lista de adjacencia do vertice a. 
	vertice novo_vertice, aux = (vertice)malloc(sizeof(vertice));
	
	int i = 0, achou = 0;
	while(!achou)
	{
		if(g->array[i].head == NULL)
		{			
			g->numV++;
			novo_vertice = cria_vertice_lista(a);
			g->array[i].head = novo_vertice;
			g->array[i].head->grau = 0;
			achou = 1;
		}
		if(strcmp(g->array[i].head->nome, a) == 0 && strcmp(b, " ") != 0)
		{	
			novo_vertice = cria_vertice_lista(b);
			aux = g->array[i].head;
			while(aux->next != NULL)
			{
				aux = aux->next;
			}
			aux->next = novo_vertice;
			g->array[i].head->grau++;
			achou = 1;
		}
		i++;
	}

	i = 0, achou = 0;
	while(!achou && strcmp(b, " ") != 0)
	{
		if(g->array[i].head == NULL)
		{
			g->numV++;
			novo_vertice = cria_vertice_lista(b);
			g->array[i].head = novo_vertice;
			g->array[i].head->grau = 0;
			achou = 1;
		}
		if(strcmp(g->array[i].head->nome, b) == 0)
		{	
			novo_vertice = cria_vertice_lista(a);
			aux = g->array[i].head;
			while(aux->next != NULL)
			{
				aux = aux->next;
			}
			aux->next = novo_vertice;
			g->array[i].head->grau++;
			achou = 1;
		}
		i++;
	}
	//free(novo_vertice);
	//free(aux);
}

grafo cria_grafo()
{
	grafo g = (grafo)malloc(sizeof(grafo));
	g->numV = 0;
	g->numA = 0;

	//cria um array de listas de adjacencia
	g->array = (AdjList)malloc(100*sizeof(AdjList));

	//inicializa cada lista de adjacencia fazendo o no principal == NULL
	for(int i = 0; i < 100; ++i)
	{
		g->array[i].head = NULL;
	}

	return g;
}

int destroi_grafo(grafo g)
{
	for(int i = 0; i < g->numV; ++i)
	{
		vertice aux, v = g->array[i].head;

		while(v->next != NULL)
		{
			aux = v->next;
			free(v->nome);
			free(v);
			v = aux;
		}
		free(g->array[i].head);
	}
	free(g->array);

	return 1;
}

grafo le_grafo(FILE *input)
{
	grafo g = cria_grafo();
	char *a, *b, *line, ch;
	
	line = (char*)calloc(buffSize, sizeof(char));
	
	int i = 0, j = 0, numEspaco = 0, espaco;

	while( (ch = fgetc(input)) != EOF )
	{	
		line[j] = ch;
		j++;
		
		if(ch == ' ')
		{
			numEspaco++;
			if(numEspaco == 1)
			{
				espaco = j - 1;
			}
		}	

		if(ch == '\n')
		{	
			i++;
			//printf("Final da %d linha\n", i);
			if(line[strlen(line) - 2] == ' ')
			{
				line[strlen(line) - 2] = '\0';
				numEspaco--;
			}
			else
			{
				line[strlen(line) - 1] = '\0';
			}
			if(numEspaco > 1)
			{
				printf("LINHA %s INVALIDA pois possui mais de duas Strings\n\n", line);
			}
			else if(line[0] == '\0')
			{
				//printf("Linha vazia\n\n");
			}
			else
			{	
				a = (char*)calloc(buffSize/2, sizeof(char));
				b = (char*)calloc(buffSize/2, sizeof(char));

				//printf("%s %s\n", a, b);
				if(strlen(a) <= 1024 && strlen(b) <= 1024)
				{
					//printf("linha: %s\n", line);
					//printf("primeira coluna: %s de tamanho: %zu segunda coluna: %s de tamanho: %zu\n\n", a, strlen(a), b, strlen(b));
					if(numEspaco == 0)
					{	
						strcpy(a, line);
						add_aresta(g, a, " ");
					}
					else
					{	
						strncpy(a, line, espaco);
						strncpy(b, &line[espaco + 1], strlen(line));
						add_aresta(g, a, b);
						g->numA++;
					}
					if((g->numV%100 == 0) && (g->numV != 0))
					{
						printf("realoquei mais 100 espacos na lista de adjacencia\n");
						g->array = (AdjList)realloc(g->array, (g->numV+100)*sizeof(AdjList));
						for(int i = 1; i <= 100; ++i)
						{
							g->array[g->numV+i].head = NULL;
						}
					}
					free(a);
					free(b);
				}
				else
				{
					printf("Tamanho da String invalido\n\n");
				}
			}
			free(line);
			j = 0;
			numEspaco = 0;
			line = (char*)calloc(buffSize, sizeof(char));
			
		}

	}
	fclose(input);
	return g;
}

grafo escreve_grafo(FILE *output, grafo g)
{
    int v;
	vertice aux = (vertice)malloc(sizeof(vertice));

    printf("\nGrafo com %d vertices e %d arestas\n\n", g->numV, g->numA);
    for (v = 0; v < g->numV; ++v)
    {
        printf("\n(vertice %s de grau %d)\n", g->array[v].head->nome, 
			g->array[v].head->grau);
        vertice vert = g->array[v].head->next;
        while (vert)
        {
            printf(" -> %s", vert->nome);
            vert = vert->next;
        }
        printf("\n");
    }
    for(int i = 0; i < g->numV; ++i)
    {	
    	aux = g->array[i].head;
   	 	while(aux->next != NULL)
		{	
			fprintf(output, "%s %s\n", g->array[i].head->nome, aux->next->nome);
			aux = aux->next;
		}
	}
	//free(aux);
	return g;
}

double coeficiente_agrupamento_grafo(grafo g)
{
	double numTriade = 0.0, numTriadeFechada = 0.0;
	int ehVizinho = 0;
	double coeficienteAgrup;

	if(g->numA < 3 || g->numV < 3)
	{
		return 0.0;
	}

	vertice aux1 = (vertice)malloc(sizeof(vertice));
	vertice aux2 = (vertice)malloc(sizeof(vertice));

	for(int i = 0; i < g->numV; ++i)
	{	
		for(int j = 0; j < g->numV; ++j)
		{	
			aux2 = g->array[j].head;
			while((aux2->next != NULL) && (!ehVizinho))
			{
				if(strcmp(aux2->next->nome, g->array[i].head->nome) == 0)
				{
					ehVizinho = 1;
				}
				aux2 = aux2->next;
			}

			if(i != j)
			{
				aux1 = g->array[i].head;

				while(aux1->next != NULL)
				{
					aux2 = g->array[j].head;
					while(aux2->next != NULL)
					{	
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
	//free(aux1);
	//free(aux2);

	numTriade = numTriade/2;
	numTriadeFechada = numTriadeFechada/2;
	coeficienteAgrup = numTriadeFechada/numTriade;

	printf("NUMERO TOTAL DE TRIADES: %.0lf\n", numTriade);
	printf("NUMERO TOTAL DE TRIADES FECHADAS: %.0lf\n", numTriadeFechada);
	printf("COEFICIENTE DE AGRUPAMENTO: %.2lf\n", coeficienteAgrup);
	return 1.0;
}


int main(int argc, char const *argv[])
{	

	FILE *output = fopen("output", "w");

	grafo g = le_grafo(stdin);
	escreve_grafo(output, g);

	coeficiente_agrupamento_grafo(g);
	//destroi_grafo(g);

	return 0;
}