#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "resolvedor_linear.h"
#include "../Tradutor/traduz.h"

void finaliza(void*); //função de callback para 'erro', caso os dados não sejam desalocados

int main(int argc, char *argv[])
{
#ifdef PT_BR
	if(argc <= 2)
		erro("Erro: informar o nome do arquivo com o problema de entrada e o tipo de problema.\nOpções: -lin, -int e -bin\n", NULL, NULL);
	if(argc > 3)
		erro("Erro: informar apenas o nome do arquivo com o problema de entrada e uma das três opções disponíveis.\nOpções: -lin, -int e -bin\n", NULL, NULL);
#else
	if(argc <= 2)
		erro("Error: inform the name of the file with the input program and the program type.\nOptions: -lin, -int and -bin\n", NULL, NULL);
	if(argc > 3)
		erro("Error: inform only the name of the file with the input program and one of the three available options.\nOptions: -lin, -int and -bin\n", NULL, NULL);
#endif

	traduz(argv[1]); //traduz o arquivo em argv[1] registrando o resultado em um arquivo 'Easter_Egg----1235813-20'

	DadosProblema dados;
	entradaDados(&dados, "Easter_Egg----1235813-20");
	remove("Easter_Egg----1235813-20");

	if(strcmp(argv[2], "-lin") == 0) resolveProgLinear(&dados);
	else if(strcmp(argv[2], "-int") == 0) resolveProgLinearInt(&dados);
	else if(strcmp(argv[2], "-bin") == 0) resolveProgLinearBin(&dados);
#ifdef PT_BR
	else erro("Erro: opção inválida.\nOpções: -lin, -int e -bin\n", finaliza, &dados);
#else
	else erro("Error: invalid option.\nOptions: -lin, -int and -bin\n", finaliza, &dados);
#endif

	desalocaDados(&dados);	
	return 0;
}

void finaliza(void *dados)
{
	desalocaDados((DadosProblema*)dados);
	exit(1);
}
