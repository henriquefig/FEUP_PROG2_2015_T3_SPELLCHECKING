#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tabdispersao.h"

// #include & #define
// functions and procedures
long int erros=0, sugestoes=0;
int trocados(char * src,char *dst)
{
	if(strlen(src)==strlen(dst))
	{
		for(int i=0;i<strlen(src);i++)
		{
			for(int j=strlen(dst);j>0;j--)
			{

				if(i!=j && src[i]==dst[j] && src[j]==dst[i])
				{
					char aux[100];
					for(int h=0;h<strlen(src);h++)
					{
						aux[h]=src[h];
						if(h==i)
							aux[h]=src[j];
						if(h==j)
							aux[h]=src[i];
					}
					if(strcmp(aux,dst)==0)
						return 0;
				}
			}
		}
	}
	return 1;
}
void str_tolower(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}
int checktable_disp(tabela_dispersao *t,char *str,int selector)
{
	char wsugestion[15][30];
	char aux[30];
	int count=0;
	int wnsugest=0;
	printf("%s\n",str);
	str_tolower(str);
	for(int i=0;i<strlen(str);i++)
	{
		while(str[i]=='(' || str[i]=='"' || str[i]==')' || str[i]==',')
			i++;
		if(str[i]=='-' || str[i]=='\n' || isdigit(str[i]))
			str[i]=' ';
		if((str[i]==' ' || i==strlen(str)-1) && count!=0)
		{
			aux[count]='\0';
			if(i==strlen(str)-1)
				aux[count]=str[i];
			count=0;
			if(tabela_valor(t,aux)!=NULL)
			{
			    memset(wsugestion,0,sizeof(wsugestion));
			    wnsugest=0;
			}
			else
			{
			    memset(wsugestion,0,sizeof(wsugestion));
			    wnsugest=0;
				element ** troca=tabela_trocados(t,aux);
				if(troca!=NULL)
				{
					int cont=0;
					while(troca[cont]!=NULL)
					{
						int superflag=0;
		        		if(wnsugest==0)
		        		{
	        				strcat(wsugestion[wnsugest],troca[cont]->obj->chave);
	        				wnsugest++;
		        		}
				        for(int h=0;h<wnsugest;h++)
				        {
				        	if(strcmp(troca[cont]->obj->chave,wsugestion[h])==0)
		        			{
		        				superflag=1;
		        			}
		        		}
		        		if(superflag!=1)
		        		{
	        				strcat(wsugestion[wnsugest],troca[cont]->obj->chave);
	        				wnsugest++;
		        		}
		        		cont++;
					}
				}
				if(selector==1)
				{

					printf("\tWord not found: %s\n",aux);
					printf("\tPerhaps you meant: ");
					for(int h=0;h<wnsugest;h++)
						if(h!=wnsugest-1)
				     	printf("%s, ",wsugestion[h]);
				     printf("%s\n",wsugestion[wnsugest-1]);
				     memset(wsugestion,0,sizeof(wsugestion));
				}
			    sugestoes=sugestoes+wnsugest;
			    wnsugest=0;
			        		
				erros++;
			}
		    memset(aux,0,sizeof(aux));
		}
		else
		{
			if(str[i]!=' ')
			{
				aux[count]=str[i];
				count++;
			}
		}
	}
}
int readworlist(tabela_dispersao *twords,FILE * finput,int selector)
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
				checktable_disp(twords,str,selector);
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
		readworlist(t,input,1);
		tabela_apaga(t);
		// processa e imprime output como indicado no enunciado
		// guarda na variável erros o número de palavras com ortografia errada encontradas
		// guarda na variável sugestoes o número de palavras sugeridas para corrigir palavras com ortografia errada encontradas
		fim = clock();
	} else if(!strcmp(argv[2],"silencioso")){
		inicio = clock();
		tabela_dispersao *t=readfiletable(wordlist);
		readworlist(t,input,0);
		tabela_apaga(t);
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
