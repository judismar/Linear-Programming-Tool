#include "resolvedor_linear.h"
#include <stdio.h>
#include <math.h>
#include <glpk.h>

// Variável inteira que será usada como booleana. Quando o Simplex encontrar problema, ela será usada, de forma a não ter um segundo relatório vindo do resolvedor 'intopt'. (Usada apenas para problemas inteiros e binários.)
int problemaSemSolucao = 0;

// Declarações dos protótipos de funções ocultas neste módulo para auxiliar as funções principais - que serão usadas pelo usuário da biblioteca e são definidas em seguida.
void montaProblemaGeral(glp_prob*, DadosProblema*); //Monta o problema para os 3 casos (Linear, Inteiro ou Binário).
void liberaMemoriaProblema(glp_prob*); //Libera memória alocada em uma instância da estrutura glp_prob.
void resultadoProblemaLinear(glp_prob*, DadosProblema*); //Faz as verificações do resultado do problema de Programação Linear e imprime o que precisar.
void resultadoProblemaInteiro(glp_prob*, DadosProblema*); //Faz as verificações do resultado do problema de Programação Inteira ou Binária e imprime o que precisar.
void trataResultadoSemSolucaoProgInteira(glp_prob*, DadosProblema*); //Faz as verificações do resultado gerado por glp_simplex e, caso já for detectado que não há solução, relata o resultado e atualiza a variável booleana global 'problemaSemSolucao'.

// Declarações das funções para imprimir árvore de Branch-and-Bound
void imprimeArvoreBranchAndBoundMin(glp_prob *p, DadosProblema *d, double *x0);
void imprimeArvoreBranchAndBoundMax(glp_prob *p, DadosProblema *d, double *x0);

// Definições das funções principais disponibilizadas pela biblioteca.
void resolveProgLinear(DadosProblema *dp)
{
	int j;
	glp_prob *lp;
	glp_smcp parametros; //parâmetros de controle para o Simplex
	lp = glp_create_prob();
	glp_init_smcp(&parametros);

	montaProblemaGeral(lp, dp);
	parametros.msg_lev = GLP_MSG_OFF;

	glp_simplex(lp, &parametros);

	resultadoProblemaLinear(lp, dp);
	liberaMemoriaProblema(lp);
}

void resolveProgLinearInt(DadosProblema *dp)
{
	int j;
	glp_prob *ip;
	glp_smcp sparametros; //parâmetros de controle para o Simplex
	glp_iocp iparametros; //parâmetros de controle para o intopt (resolvedor de programação inteira-mista)
	ip = glp_create_prob();
	glp_init_smcp(&sparametros);
	glp_init_iocp(&iparametros);

	montaProblemaGeral(ip, dp);
	sparametros.msg_lev = GLP_MSG_OFF;
	iparametros.msg_lev = GLP_MSG_OFF;

	glp_simplex(ip, &sparametros); //fornece uma solução ótima relaxada para o branch-and-cut
	trataResultadoSemSolucaoProgInteira(ip, dp); //verificação que não pode ser feita pelo uso do intopt
	if(problemaSemSolucao)
	{
		liberaMemoriaProblema(ip);
		return;
	}

	for(j = 0; j < dp->m; j++) //atualiza o tipo de todas as variáveis para 'inteira'
		glp_set_col_kind(ip, j+1, GLP_IV);

	glp_intopt(ip, &iparametros);

	resultadoProblemaInteiro(ip, dp);
	liberaMemoriaProblema(ip);
}

void resolveProgLinearBin(DadosProblema *dp)
{
	int j;
	glp_prob *bp;
	glp_smcp sparametros; //parâmetros de controle para o Simplex
	glp_iocp iparametros; //parâmetros de controle para o intopt (resolvedor de programação inteira-mista)
	bp = glp_create_prob();
	glp_init_smcp(&sparametros);
	glp_init_iocp(&iparametros);

	montaProblemaGeral(bp, dp);

	for(j = 0; j < dp->m; j++) glp_set_col_bnds(bp, j+1, GLP_DB, 0, 1); //coloca a restrição especial para resolver problema binário, de forma que não vá para + ou - infinito ao usar Simplex no problema relaxado
	
	sparametros.msg_lev = GLP_MSG_OFF;
	iparametros.msg_lev = GLP_MSG_OFF;

	glp_simplex(bp, &sparametros); //fornece uma solução ótima relaxada para o branch-and-cut
	trataResultadoSemSolucaoProgInteira(bp, dp); //verificação que não pode ser feita pelo mip solver
	if(problemaSemSolucao)
	{
		liberaMemoriaProblema(bp);
		return;
	}

	for(j = 0; j < dp->m; j++) //atualiza o tipo de todas as variáveis para 'binária'
		glp_set_col_kind(bp, j+1, GLP_BV);

	glp_intopt(bp, &iparametros);
	resultadoProblemaInteiro(bp, dp);
	liberaMemoriaProblema(bp);		
}

// Defnições das funçoes auxiliares ocultas
void montaProblemaGeral(glp_prob *gp, DadosProblema *dp) //MELHORAR COMENTÁRIOS!
{
	double a[1+dp->n*dp->m];
	int ia[1+dp->n*dp->m], ja[1+dp->n*dp->m];
	int i, j, k;

	glp_set_obj_dir(gp, dp->opt); //inicia o problema como de minimização ou maximização

	if(dp->n > 0)
	{
		glp_add_rows(gp, dp->n); //cria linhas da matriz 'a' (restrições)
		for(i = 0; i < dp->n; i++)
			glp_set_row_bnds(gp, i+1, dp->eq[i], dp->a[i][dp->m], dp->a[i][dp->m]); //atualizando o tipo de restrição (<=, >= ou =) e o valor do coeficiente associado (que será colocado como valor de upper bound e lower bound para evitar o uso de if); i+1 pois o glpk usa índices de 1 a n
	}
	
	glp_add_cols(gp, dp->m); //cria as colunas da matriz 'a', de forma que as variáveis possam ser editadas
	for(j = 0; j < dp->m; j++) //inicia a constante associada e o seu limite
	{
		glp_set_col_bnds(gp, j+1, dp->ineq[j], 0, 0); //coloca o limite de cada variável
		glp_set_obj_coef(gp, j+1, dp->c[j]); //atribui o coeficiente associado à variável na função objetivo
	}

	for(i = 0, k = 1; i < dp->n; i++)
		for(j = 0; j < dp->m; j++, k++)
			ia[k] = i+1, ja[k] = j+1, a[k] = dp->a[i][j]; //a(i+1, j+1) <- dp->a[i][j]; deslocamento de 1 para cada índice da matriz 'a', pois índices vão de 1 a n

	glp_load_matrix(gp, dp->n*dp->m, ia, ja, a); //carrega a matriz para a variável 'gp'
}

void liberaMemoriaProblema(glp_prob *gp)
{
	glp_delete_prob(gp);
	glp_free_env();
}

void resultadoProblemaLinear(glp_prob *lp, DadosProblema *dp)
{
	int j;
	switch(glp_get_status(lp))
	{
		case GLP_OPT:
			printf("%s = %g", dp->nomeVariaveis[0], glp_get_obj_val(lp));
			for(j = 1; j <= dp->m; j++)
				printf("\n%s = %g", dp->nomeVariaveis[j], glp_get_col_prim(lp, j));
			printf("\n");
			break;
		case GLP_UNBND:
			if(dp->opt == GLP_MAX) printf("%s -> +∞\n", dp->nomeVariaveis[0]);
			else printf("%s -> -∞\n", dp->nomeVariaveis[0]);
			break;
#ifdef PT_BR
		case GLP_FEAS:
			printf("A solução é viável mas não ótima.\n");
			break;
		case GLP_INFEAS:
			printf("A solução é inviável.\n");
			break;
		case GLP_NOFEAS:
			printf("O programa não tem solução viável.\n");
			break;
		case GLP_UNDEF:
			printf("A solução é indefinida.\n");
			break;
#else
		case GLP_FEAS:
			printf("The solution is feasible but not optimal.\n");
			break;
		case GLP_INFEAS:
			printf("The solution is infeasible.\n");
			break;
		case GLP_NOFEAS:
			printf("The program has no feasible solution.\n");
			break;
		case GLP_UNDEF:
			printf("The solution is undefined.\n");
			break;
#endif
	}
}

void resultadoProblemaInteiro(glp_prob *ip, DadosProblema *dp)
{
	int j;
	switch(glp_mip_status(ip))
	{
		case GLP_OPT:
			printf("%s = %g", dp->nomeVariaveis[0], glp_mip_obj_val(ip));
			for(j = 1; j <= dp->m; j++)
				printf("\n%s = %g", dp->nomeVariaveis[j], glp_mip_col_val(ip, j));
			printf("\n");
			break;
#ifdef PT_BR
		case GLP_FEAS:
			printf("A solução é viável mas não ótima.\n");
			break;
		case GLP_NOFEAS:
			printf("O programa não tem solução viável.\n");
			break;
		case GLP_UNDEF:
			printf("A solução é indefinida.\n");
			break;
#else
		case GLP_FEAS:
			printf("The solution is feasible but not optimal.\n");
			break;
		case GLP_NOFEAS:
			printf("The program has no feasible solution.\n");
			break;
		case GLP_UNDEF:
			printf("The solution is undefined.\n");
			break;
#endif
	}
}

void trataResultadoSemSolucaoProgInteira(glp_prob *ip, DadosProblema *dp)
{
	switch(glp_get_status(ip))
	{
		case GLP_UNBND:
			if(dp->opt == GLP_MAX) printf("%s -> +∞\n", dp->nomeVariaveis[0]);
			else printf("%s -> -∞\n", dp->nomeVariaveis[0]);
			problemaSemSolucao = 1;
			break;
#ifdef PT_BR
		case GLP_INFEAS:
			printf("A solução é inviável.\n");
			problemaSemSolucao = 1;
			break;
		case GLP_NOFEAS:
			printf("O programa não tem solução viável.\n");
			problemaSemSolucao = 1;
			break;
#else
		case GLP_INFEAS:
			printf("The solution is infeasible.\n");
			problemaSemSolucao = 1;
			break;
		case GLP_NOFEAS:
			printf("The program has no feasible solution.\n");
			problemaSemSolucao = 1;
			break;
#endif
	}
}

void imprimeArvoreBranchAndBoundMin(glp_prob *p, DadosProblema *d, double *x0)
{
	int j;
	glp_prob *aux;
	glp_simplex(p, NULL);
	if(glp_get_obj_val(p) < *x0) //verificação de poda
		for(j = 1; j <= d->m; j++) //abrir todos os nós possíveis
			if(glp_get_col_prim(p, j) != floor(glp_get_col_prim(p, j))) //não é inteiro
			{
				glp_copy_prob(aux, p, GLP_OFF);
				glp_set_col_bnds(aux, j, GLP_UP, 0, floor(glp_get_col_prim(p, j)));
				imprimeArvoreBranchAndBoundMin(aux, d, x0); //chamada recursiva do piso

				glp_copy_prob(aux, p, GLP_OFF);
				glp_set_col_bnds(aux, j, GLP_LO, floor(glp_get_col_prim(p, j)) + 1, 0);
				imprimeArvoreBranchAndBoundMin(aux, d, x0); //chamada recursiva do teto
			}
	liberaMemoriaProblema(p);
}

void imprimeArvoreBranchAndBoundMax(glp_prob *p, DadosProblema *d, double *x0)
{
	
}
