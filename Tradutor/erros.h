#ifndef ERROS_H
#define ERROS_H
#include <stdio.h>
#include "tabela_simbolos.h"

void erro_tam_variavel(int tamanho, int linha, FILE *entrada);
void divisao_por_zero(float denominador, int linha, FILE *entrada);
void simbolo_desconhecido(int linha, FILE *entrada);
void variavel_repetida(TABELA*, char *simbolo, int linha, FILE *entrada);
void variavel_inexistente(TABELA*, char *simbolo, int linha, FILE *entrada);
void arquivo_nao_aberto(FILE*);

#endif
