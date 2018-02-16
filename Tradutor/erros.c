#include "erros.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef PT_BR
void erro_tam_variavel(int tam, int linha, FILE *entrada)
{
	if(tam > 30)
	{
		fclose(entrada);
		printf("Erro na linha %d. Nome de variável de tamanho maior que 30.\n", linha);
		exit(1);
	}
}

void divisao_por_zero(float denominador, int linha, FILE *entrada)
{
	if(denominador == 0)
	{
		fclose(entrada);
		printf("Erro na linha %d. Divisão por 0.\n", linha);
		exit(1);
	}
}

void simbolo_desconhecido(int linha, FILE *entrada)
{
	fclose(entrada);
	printf("Erro na linha %d. Símbolo desconhecido.\n", linha);
	exit(1);
}

void variavel_repetida(TABELA *t, char *nome, int linha, FILE *entrada)
{
	if(simbolo_existente(t, nome))
	{
		fclose(entrada);
		printf("Erro na linha %d. Variável repetida.\n", linha);
		exit(1);
	}
}

void variavel_inexistente(TABELA *t, char *nome, int linha, FILE *entrada)
{
	if(!simbolo_existente(t, nome))
	{
		fclose(entrada);
		printf("Erro na linha %d. Variável não faz parte do problema.\n", linha);
		exit(1);
	}
}

void arquivo_nao_aberto(FILE *f)
{
	if(!f)
	{
		printf("Erro ao escrever no arquivo.\n");
		exit(1);
	}
}
#else
void erro_tam_variavel(int tam, int linha, FILE *entrada)
{
	if(tam > 30)
	{
		fclose(entrada);
		printf("Error in line %d. Variable name greater than 30.\n", linha);
		exit(1);
	}
}

void divisao_por_zero(float denominador, int linha, FILE *entrada)
{
	if(denominador == 0)
	{
		fclose(entrada);
		printf("Error in line %d. Division by 0.\n", linha);
		exit(1);
	}
}

void simbolo_desconhecido(int linha, FILE *entrada)
{
	fclose(entrada);
	printf("Error in line %d. Unknown symbol.\n", linha);
	exit(1);
}

void variavel_repetida(TABELA *t, char *nome, int linha, FILE *entrada)
{
	if(simbolo_existente(t, nome))
	{
		fclose(entrada);
		printf("Error in line %d. Variable repeated.\n", linha);
		exit(1);
	}
}

void variavel_inexistente(TABELA *t, char *nome, int linha, FILE *entrada)
{
	if(!simbolo_existente(t, nome))
	{
		fclose(entrada);
		printf("Error in line %d. Variable is not part of the program.\n", linha);
		exit(1);
	}
}

void arquivo_nao_aberto(FILE *f)
{
	if(!f)
	{
		printf("Error in writing to file.\n");
		exit(1);
	}
}
#endif
