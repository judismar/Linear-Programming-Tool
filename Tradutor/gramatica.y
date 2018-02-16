%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tradutor/tabela_simbolos.h"
#include "Tradutor/erros.h"
#define TAM_MAX_SAIDA 1000000

extern int linha;
extern char buffer_id[30];
extern char buffer_num[20];
extern int enunciado;
extern int limiteVar; 

extern FILE *yyin;

TABELA *tabela;

char saida[TAM_MAX_SAIDA]; //uma string grande para problemas grandes
char buffer_saida[TAM_MAX_SAIDA];
int cont;
char *tipo_eq;
char buffer_sinal[21];

void concatena_nome_simbolos(char *s)
{
	strcat(s, retorna_nome_f_objetivo(tabela));
	int tam = tamanho_tabela(tabela);
	int i;
	for(i = 0; i < tam; i++)
	{
		strcat(s, " ");
		strcat(s, nome_simbolo(tabela, i));
	}
	strcat(s, "\n");
}

void cria_var_verifica_erro()
{
	if(!enunciado)
	{
		cont++;
		variavel_repetida(tabela, buffer_id, linha, yyin);
		insere_simbolo(tabela, buffer_id);
	}
	else variavel_inexistente(tabela, buffer_id, linha, yyin);
}

%}

%token max
%token min
%token s_a
%token v_b
%token tfree

%token zero

%token ident

%token num

%token sinal_positivo
%token sinal_negativo

%token igual
%token menor_igual
%token maior_igual

%token virgula

%start PPL

%%

PPL:
	PROCED5 OPT ident PROCED1 igual INICIO_EXP PROCED8 s_a ENUNCIADO
	{
		int i;
		strcat(saida, "\n");
		for(i = 0; i < tamanho_tabela(tabela); i++) //preencher o arquivo com as todos os limites padrão de variáveis
			strcat(saida, "2 ");
		strcat(saida, "\n");
	}
	|
	PROCED5 OPT ident PROCED1 igual INICIO_EXP PROCED8 s_a ENUNCIADO v_b
	{
		zera_valores_simbolos(tabela);
		strcat(saida, "\n");
		limiteVar = 1;
	} 
	VB
	{
		int i, len;
		char *valor;
		for(i = 0; i < tamanho_tabela(tabela); i++)
		{
			valor = retorna_valor_simbolo(tabela,  nome_simbolo(tabela, i));
			len = strlen(valor);
			if(len > 0)
			{
				strcat(saida, valor);
				strcat(saida, " ");
			}
			else strcat(saida, "2 ");
		}
		strcat(saida, "\n");
	}
;

OPT:
	max {strcpy(saida, "2");}
	|
	min {strcpy(saida, "1");}
;

SINAL:
	sinal_positivo {$$ = 1;}
	|
	sinal_negativo {$$ = 0;}
;

TERMO:
	SINAL ident
	{
		cria_var_verifica_erro();

		if($1) altera_valor_simbolo(tabela, buffer_id, "+1");
		else altera_valor_simbolo(tabela, buffer_id, "-1");
	}
	|
	SINAL num ident
	{
		cria_var_verifica_erro();

		if($1) strcpy(buffer_sinal, "+");
		else strcpy(buffer_sinal, "-");
		strcat(buffer_sinal, buffer_num);

		altera_valor_simbolo(tabela, buffer_id, buffer_sinal);
		strcpy(buffer_sinal, "");
	}
;

TERMO_INICIAL:
	ident
	{
		cria_var_verifica_erro();
		altera_valor_simbolo(tabela, buffer_id, "1");
	}
	|
	num ident
	{
		cria_var_verifica_erro();
		altera_valor_simbolo(tabela, buffer_id, buffer_num);
	}
	|
	TERMO
;

INICIO_EXP:
	TERMO_INICIAL EXP
	|
	TERMO_INICIAL
;

EXP:
	TERMO EXP
	|
	TERMO
;

ENUNCIADO:
	PROCED4 INICIO_EXP PROCED6 EQ CONST ENUNCIADO
	|
	PROCED4 INICIO_EXP PROCED6 EQ CONST
	{
		sprintf(buffer_num, "\n%d\n", cont);
		strcat(saida, buffer_num);
		strcat(saida, buffer_saida);
		zera_valores_simbolos(tabela);
	}
;

VB:
	ident tfree {tipo_eq = "1";} PROCED9 virgula VB
	|
	ident INEQ zero PROCED9 virgula VB
	|
	ident tfree {tipo_eq = "1";} PROCED9
	|
	ident INEQ zero PROCED9
;

INEQ:
	menor_igual {tipo_eq = "3";}
	|
	maior_igual {tipo_eq = "2";}
;

EQ:
	menor_igual {tipo_eq = "3";}
	| maior_igual {tipo_eq = "2";}
	| igual {tipo_eq = "5";}
;

CONST:
	num 
	{
		strcat(buffer_saida, "+"); strcat(buffer_saida, buffer_num);
		strcat(buffer_saida, " ");
		strcat(buffer_saida, tipo_eq);
	}
	|
	SINAL num
	{
		if($1) strcat(buffer_saida, "+");
		else strcat(buffer_saida, "-");
		strcat(buffer_saida, buffer_num);
		strcat(buffer_saida, " ");
		strcat(buffer_saida, tipo_eq);
	}
;

//--------- Símbolos que enterram ações intermediárias ------------

PROCED1:
{
	insere_nome_f_objetivo(tabela, buffer_id);
	cont = 0;
}
;

PROCED4:
{
	cont++;
	strcat(buffer_saida, "\n");
	zera_valores_simbolos(tabela);
}
;

PROCED5:
{
	strcpy(buffer_saida, "");
}
;

PROCED6:
{
	int i, len;
	char *valor;
	for(i = 0; i < tamanho_tabela(tabela); i++)
	{
		valor = retorna_valor_simbolo(tabela,  nome_simbolo(tabela, i));
		len = strlen(valor);
		if(len > 0)
		{
			strcat(buffer_saida, valor);
			strcat(buffer_saida, " ");
		}
		else strcat(buffer_saida, "0 ");
	}
}
;

PROCED8:
{
	int i;
	char *valor;
	sprintf(buffer_num, "\n%d", cont); 
	strcat(saida, buffer_num); 
	cont = 0;
	for(i = 0; i < tamanho_tabela(tabela); i++) //imprimir coeficientes da f_objetivo
	{
		valor = retorna_valor_simbolo(tabela,  nome_simbolo(tabela, i));
		strcat(buffer_saida, valor);
		strcat(buffer_saida, " ");	
	}
}
;

PROCED9:
{
	variavel_inexistente(tabela, buffer_id, linha, yyin);
	altera_valor_simbolo(tabela, buffer_id, tipo_eq);
}
;

%%

#include "lex.yy.c"
#include "Tradutor/traduz.h"

char* parsing(char *arquivoEntrada) //retorna toda a saída em uma string
{
	yyin = fopen(arquivoEntrada, "r");
	tabela = cria_tabela();
	yyparse();
	fclose(yyin);
	concatena_nome_simbolos(saida);
	destroi_tabela(tabela);
	return saida;
}

void traduz(char *arq) //função que será usada por uma interface
{
	char *dados = parsing(arq);
	FILE *arqSaida = fopen("Easter_Egg----1235813-20", "w");
	fprintf(arqSaida, "%s", saida);
	fclose(arqSaida);
}

//rotina chamada por yyparse quando encontra erro
yyerror (void)
{
	fclose(yyin);
#ifdef PT_BR
	printf("Erro de sintaxe na linha %d.\n", linha);
#else
	printf("Syntax error on line %d.\n", linha);
#endif
	exit(1);
}
