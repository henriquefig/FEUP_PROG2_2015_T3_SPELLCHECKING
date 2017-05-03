#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tabdispersao.h"

// #include & #define
// functions and procedures
long int erros=0, sugestoes=0;
void str_tolower(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}
int checktable_disp(tabela_dispersao *t,char *str)
{
	char wsugestion[15][30];
	char aux[30];
	int count=0;
	int wnsugest=0;
	str_tolower(str);
	printf("%s\n",str);
	for(int i=0;i<strlen(str);i++)
	{
		while(str[i]=='(' || str[i]=='"' || str[i]==')' || str[i]==',')
			i++;
		if(str[i]=='-' || str[i]=='\n' || isdigit(str[i]))
			str[i]=' ';
		if(str[i]==' ' || i==strlen(str)-1)
		{
			aux[count]='\0';
			if(i==strlen(str)-1)
				aux[count]=str[i];
			count=0;
			//printf("Palavra - %s\n",aux);
			if(tabela_valor(t,aux)!=NULL)
			{
			    memset(wsugestion,0,sizeof(wsugestion));
			    wnsugest=0;
			}
			else
			{
				printf("\tWord not found: %s\n",aux);
				printf("\tPerhaps you meant: ");
				for(int h=0;h<wnsugest;h++)
					if(h!=wnsugest-1)
			     	printf("%s, ",wsugestion[h]);
			     printf("%s\n",wsugestion[wnsugest-1]);
			     memset(wsugestion,0,sizeof(wsugestion));
			     sugestoes=sugestoes+wnsugest;
			     wnsugest=0;
			        		
				 erros++;
			}
		}
		else
		{
			aux[count]=str[i];
			count++;
		}
	}
}
int readworlist(tabela_dispersao *twords,FILE * finput)
{
	char *str=calloc(300,1);
	char c;
	int count=0;
	while(fscanf(finput,"%c",&c)!=EOF)
    {

    	//Se chegar ao final da linha
    	if(c=='.' || c=='!' || c=='?' || c==';')
    	{
    		if(strstr(str,"Online-Utility")!=NULL)
    		{
    			char rest[40];
    			fgets(rest,20,finput);
    			strcat(str,rest);
    			fgets(rest,20,finput);
    			strcat(str,rest);
    			fgets(rest,20,finput);
    			strcat(str,rest);
    		}
    		else
	    		str[count]='\0';
    		count=0;
    		if(strstr(str,"Online-Utility")==NULL)
			{
				checktable_disp(twords,str);
			}
			str=calloc(300,1);
    	}
    	else
    	{
	    	//Guarda o caracter na String
	    	str[count]=c;
	    	count++;	
    	}
    }
}
int main(int argc, char* argv[])
{
	FILE *input, *wordlist;
	clock_t inicio, fim;
  double tempo=0;

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
		tabela_dispersao *t=readfiletable(wordlist);
		readworlist(t,input);
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
