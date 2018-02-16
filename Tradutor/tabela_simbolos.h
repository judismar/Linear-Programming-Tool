#ifndef TABELA_SIMBOLOS_H
#define TABELA_SIMBOLOS_H
//Tipo TABELA, que armazenará todos os símbolos lidos no problema de entrada. Os símbolos devem ter no máximo 10 caracteres.
//Obs: não serão armazenadas as referências, mas cópias de strings, então constantes podem ser passadas como argumento.

typedef struct tabela_simbolos TABELA;

TABELA* cria_tabela(); //retorna a tabela, ou então NULL, caso haja erro na alocação de memória.

int insere_nome_f_objetivo(TABELA*, char*);
char* retorna_nome_f_objetivo(TABELA*);
int insere_simbolo(TABELA*, char*); //retorna 1 se tudo der certo e 0, caso o elemento tenha tamanho maior que 10.
int simbolo_existente(TABELA*, char*); //retorna 1 caso o símbolo já esteja na tabela, ou 0 caso contrário
int tamanho_tabela(TABELA*); //não conta a função objetivo no tamanho!
char* nome_simbolo(TABELA*, int);
int posicao_simbolo(TABELA*, char*);
void printt(TABELA*);
void zera_valores_simbolos(TABELA*);
int altera_valor_simbolo(TABELA*, char *id, char *valor); //altera o valor de um símbolo; retorna 1 se símbolo existente, 0 caso contrário
char* retorna_valor_simbolo(TABELA*, char *id); //retorna NULL caso símbolo não exista

void destroi_tabela(TABELA*);

#endif
