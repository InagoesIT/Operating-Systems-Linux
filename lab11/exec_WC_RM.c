/*
  Program: soluția problemei [Exec command #4: wc ...; rm ... ].
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>  // necesar pentru PATH_MAX

int main(int argc, char* argv[])
{
	char nume_fis[PATH_MAX];
	int cod_term;
	struct stat sb;

	// Pasul #1: obtinerea numelui fisierului de prelucrat
	
	if(argc > 1)
	{
		strcpy(nume_fis,argv[1]);
	}
	else
	{
		printf("Introduceti numele fisierului dorit:");  
		scanf("%s", nume_fis);
	}

	// Pasul #2: invocarea primei comenzi: wc nume_fis
	
	switch( fork() )
	{
		case -1:	perror("Eroare la primul fork");  exit(1);

		case  0:	execlp("wc","wc","-w",nume_fis,NULL);
				perror("Eroare la primul exec");  exit(2);
		
		default:	wait(&cod_term);
				if( WIFEXITED(cod_term) )
					printf("Comanda wc a rulat, terminandu-se cu codul de terminare: %d.\n", WEXITSTATUS(cod_term) );
				else
					printf("Comanda wc a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );
	}

	// Pasul #3: apelarea la necesitate a comenzii rm
	
	if ( stat(nume_fis, &sb) == -1 )
	{
	    perror("Eroare la stat"); exit (3);
	} 
	else if (sb.st_size == 0) // fisierul este vid, deci il stergem cu rm -i nume_fis
	{
	    char* args[] = {"/usr/bin/rm", "-i", nume_fis, NULL};
	    
	    switch( fork() )
	    {
		    case -1:	perror("Eroare la al doilea fork");  exit(4);

		    case  0:	execv(args[0], args);
				        perror("Eroare la al doilea exec");  exit(5);
	    
		    default:	wait(&cod_term);
				        if( WIFEXITED(cod_term) )
					        printf("Am sters fisierul!\n");
				        else
					        printf("Comanda rm a fost terminata fortat de catre semnalul: %d.\n", WTERMSIG(cod_term) );
	    }
	}
	else
	{
	    printf("Am pastrat fisierul!\n");
	}            
}
