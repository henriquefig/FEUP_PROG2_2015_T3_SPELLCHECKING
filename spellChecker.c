#include <stdio.h>
#include <string.h>
#include <time.h>

// #include & #define
// functions and procedures

int main(int argc, char* argv[])
{
	FILE *input, *wordlist;
	clock_t inicio, fim;
  double tempo;
	long int erros, sugestoes;

	if(argc != 3)
	{
		printf("utilização: %s <nome_ficheiro> <modo>\n", argv[0]);
		printf("modo: verboso ou silencioso\n");
		return 1;
	}

	// processa lista de palavras
	wordlist = fopen("wordlist.txt", "r");
	if(!wordlist)
	{
		printf("erro ao abrir ficheiro com lista de palavras\n");
		return 2;
	}
	// armazena lista de palavras em estrutura de dados adequada

	// processa ficheiro de entrada
	input = fopen(argv[1], "r");
	if(!input)
	{
		printf("erro ao abrir ficheiro %s\n", argv[1]);
		return 3;
	}

	if(!strcmp(argv[2],"verboso")){
		inicio = clock();
		// processa e imprime output como indicado no enunciado
		// guarda na variável erros o número de palavras com ortografia errada encontradas
		// guarda na variável sugestoes o número de palavras sugeridas para corrigir palavras com ortografia errada encontradas
		fim = clock();
	} else if(!strcmp(argv[2],"silencioso")){
		inicio = clock();
		// processa e não apresenta qualquer output
		// guarda na variável erros o número de palavras com ortografia errada encontradas
		// guarda na variável sugestoes o número de palavras sugeridas para corrigir palavras com ortografia errada encontradas
		fim = clock();
	} else{
		printf("modo %s desconhecido\n", argv[2]);
		return 4;
	}

	tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
	printf("Tempo gasto na verificação (em segundos): %lf\n", tempo);
	printf("Número de palavras com ortografia errada encontradas: %li\n", erros);
	printf("Número de sugestões efetuadas: %li\n", sugestoes);

	fclose(input);
}
