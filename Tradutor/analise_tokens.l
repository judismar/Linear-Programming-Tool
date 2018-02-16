%{
#include <stdio.h>
#include <string.h>
#include "Tradutor/erros.h"
int linha = 1;
char buffer_id[30];
char buffer_num[20];
int enunciado = 0; //flag para indicar que já passou por 's.t.'
int limiteVar = 0; //flag para indicar que passou por 'v.b.'
%}

letra [_a-zA-Z]
digito [0-9]
identificador {letra}({letra}|{digito})*
inteiro {digito}+
real {inteiro}("."{inteiro})?
frac {digito}+"/"{digito}+

%%
"max" {return max;}
"min" {return min;}
"s.t."|"subject to" {enunciado = 1; return s_a;}
"v.b."|"variable bounds" {return v_b;}
"free" {return tfree;}

{identificador} {
	
	erro_tam_variavel(strlen(yytext), linha, stdin);
	strcpy(buffer_id, yytext);
	return ident;
}

{real} {
	if(limiteVar && !strcmp(yytext, "0")) return zero;
	strcpy(buffer_num, yytext);
	return num;
}

{frac} {
	float numerador, denominador; 
	int i;
	char buffer[20];

	for(i = 0; yytext[i] != '/'; i++) buffer[i] = yytext[i];

	buffer[i] = '\0';
	numerador = atoi(buffer);

	int j;
	for(i++, j = 0; yytext[i] != '\0'; i++, j++) buffer[j] = yytext[i];

	buffer[j] = '\0';
	denominador = atoi(buffer);
	divisao_por_zero(denominador, linha, stdin);
	sprintf(buffer_num, "%f", numerador/denominador);
	return num;
}

"+" {return sinal_positivo;}
"-" {return sinal_negativo;}

"=" {return igual;}
"<=" {return menor_igual;}
">=" {return maior_igual;}

"," {return virgula;}

"\n" {linha++;}

[" \t"] {}
"%"[^"\n"]* {}
. {simbolo_desconhecido(linha, stdin);}

%%

/* se definir esta função não necessita compilar com o parâmetro -lfl */
int yywrap()
{
	return(1);
}

