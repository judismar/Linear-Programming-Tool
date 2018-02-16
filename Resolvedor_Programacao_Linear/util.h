#ifndef UTIL_H
#define UTIL_H
//Utilitários. Módulo que contém funções a serem usadas pelos outros módulos.

//Estrutura que armazena os dados para cada um dos três tipos de problema: de Programação Linear, Inteira ou Binária
typedef
struct
{
	int opt; //minimizar = 1, maximizar = 2
	int m; //número de variáveis 
	int n; //número de restrições
	float *c; //vetor de coeficientes das variáveis x1, x2, ..., xm
	float **a; //matriz de restrições, contendo o elemento 'bi' do vetor b em cada posição 'n' da linha
	int *eq; //vetor de tipos de inequalidade para cada uma das n restrições; <= é 3, >= é 2 e = é 5
	int *ineq; //vetor de inequalidades usado para mudar limite padrão de variável
	char **nomeVariaveis; //vetor de m+1 nomes
}DadosProblema;

/* entradaDados:
	Le os dados de um arquivo (nomeArquivoEntrada) e insere na variável cujo endereço é passado como argumento. Aloca estruturas dinamicamente, que precisam ser desalocadas.
	*retorno para erros:
		0 - sem erros;
		1 - arquivo 'temp' inexistente
*/
int entradaDados(DadosProblema*, char *nomeArquivoEntrada);

/* desalocaDados:
	Desaloca todas as estruturas alocadas dinamicamente na variável composta DadosProblema (endereço como argumento).
*/
void desalocaDados(DadosProblema*);

/* erro:
	Imprime mensagem de erro e executa uma ação por meio de callback (recebe ponteiro genérico); caso o segundo argumento seja NULL, a ação será finalizar o programa com retorno 1, independente do terceiro argumento.
*/
void erro(char *msgErro, void (*callbackParaExecPosErro)(void *arg), void *argCallback);

#endif
