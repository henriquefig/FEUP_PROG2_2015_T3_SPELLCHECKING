spellChecker: spellChecker.o tabdispersao.o
	gcc -Wall spellChecker.o tabdispersao.o -o spellChecker
spellChecker.o: spellChecker.c
	gcc -Wall -c spellChecker.c
tabdispersao.o: tabdispersao.c tabdispersao.h
	gcc -Wall -c tabdispersao.c