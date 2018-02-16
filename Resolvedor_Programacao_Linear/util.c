#include "util.h"
#include <stdio.h>
#include <stdlib.h>

int entradaDados(DadosProblema *dp, char *arq)
{
	int i, j;
	FILE *entrada = fopen(arq, "r");
	if(entrada == NULL) return 1;

	fscanf(entrada, "%d\n%d\n%d\n", &dp->opt, &dp->m, &dp->n);

	dp->c = (float*) malloc(dp->m*sizeof(float));
	for(i = 0; i < dp->m; i++) fscanf(entrada, "%f", dp->c+i);

	dp->eq = (int*) malloc(dp->n*sizeof(int));
	dp->a = (float**) malloc(dp->n*sizeof(float*));
	for(i = 0; i < dp->n; i++)
		dp->a[i] = (float*) malloc((dp->m+1)*sizeof(float)); //folga de 1 para adicionar também, na posição 'm' do vetor, cada elemento 'b'
	for(i = 0; i < dp->n; i++)
	{
		for(j = 0; j < dp->m+1; j++)
			fscanf(entrada, "%f", dp->a[i]+j);
		fscanf(entrada, "%d", dp->eq+i);
	}

	dp->ineq = (int*) malloc(dp->m*sizeof(int)); //limites de variáveis
	for(i = 0; i < dp->m; i++)
		fscanf(entrada, "%d", dp->ineq+i);

	dp->nomeVariaveis = (char**) malloc((dp->m+1)*sizeof(char*)); //um a mais para o nome da função objetivo
	for(i = 0; i < dp->m+1; i++)
		dp->nomeVariaveis[i] = (char*) malloc(10*sizeof(char));
	for(j = 0; j < dp->m+1; j++)
		fscanf(entrada, "%s", dp->nomeVariaveis[j]);

	fclose(entrada);
	return 0;
}

void desalocaDados(DadosProblema *dp)
{
	int i;
	free(dp->c);
	for(i = 0; i < dp->n; i++)
		free(dp->a[i]);
	free(dp->a);
	free(dp->eq);
	for(i = 0; i < dp->m+1; i++)
		free(dp->nomeVariaveis[i]);
	free(dp->nomeVariaveis);
}

void erro(char *s, void (*cb)(void *arg), void *argcb)
{
	printf("%s", s);
	if(cb == NULL)
		exit(1);
	cb(argcb);
}
