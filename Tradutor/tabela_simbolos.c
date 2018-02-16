#include "tabela_simbolos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tabela_simbolos
{
	char simbolo[30];
	char valor[50];
	TABELA *prox;
};

TABELA* cria_tabela()
{
	TABELA *t = (TABELA*) malloc(sizeof(TABELA));
	if(!t) return NULL;
	t->prox = NULL;
	return t;
}

int insere_nome_f_objetivo(TABELA *t, char *s)
{
	if(strlen(s) > 30) return 0;
	strcpy(t->simbolo, s);
	return 1;
}


char* retorna_nome_f_objetivo(TABELA *t)
{
	return t->simbolo;
}


int insere_simbolo(TABELA *t, char *s)
{
	if(strlen(s) > 30) return 0;

	TABELA *aux = t;
	for(;aux->prox != NULL; aux = aux->prox);

	aux->prox = (TABELA*) malloc(sizeof(TABELA));
	strcpy(aux->prox->simbolo, s);
	strcpy(aux->prox->valor, "");
	aux->prox->prox = NULL;
	return 1;
}

int simbolo_existente(TABELA *t, char *s)
{
	TABELA *aux = t;
	for(;aux != NULL && strcmp(aux->simbolo, s); aux = aux->prox);
	if(!aux) return 0;
	return 1;
}

int tamanho_tabela(TABELA *t)
{
	int cont = 0;
	TABELA *aux = t->prox;
	for(; aux != NULL; aux = aux->prox) cont++;
	return cont;
}

char* nome_simbolo(TABELA *t, int indice)
{
	int i;
	TABELA *aux;
	for(i = 0, aux = t->prox; aux != NULL && i < indice; i++, aux = aux->prox);
	if(!aux) return NULL;
	return aux->simbolo;
}

int posicao_simbolo(TABELA *t, char *s)
{
	int i;
	TABELA *aux = t->prox;
	for(i = 0; aux != NULL && strcmp(aux->simbolo, s); aux = aux->prox, i++);
	if(!aux) return -1;
	return i;
}

void printt(TABELA *t)
{
	TABELA *aux = t->prox;
	for(;aux != NULL; aux = aux->prox) printf("%s = %s\n", aux->simbolo, aux->valor);
}

void zera_valores_simbolos(TABELA *t)
{
	TABELA *aux = t->prox;
	for(;aux != NULL; aux = aux->prox) strcpy(aux->valor, "");
}

int altera_valor_simbolo(TABELA *t, char *s, char *val)
{
	TABELA *aux = t->prox;
	for(;aux != NULL && strcmp(aux->simbolo, s); aux = aux->prox);
	if(!aux) return 0;
	strcat(aux->valor, val);
	return 1;
}

char* retorna_valor_simbolo(TABELA *t, char *s)
{
	TABELA *aux = t;
	for(;aux != NULL && strcmp(aux->simbolo, s); aux = aux->prox);
	if(!aux) return NULL;
	return aux->valor;
}

void destroi_tabela(TABELA *t)
{
	TABELA *aux1 = t, *aux2 = t->prox;
	for(;aux2 != NULL; aux1 = aux2, aux2 = aux2->prox) free(aux1);
	free(aux1);
}
