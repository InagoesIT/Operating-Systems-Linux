#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NR 10000
void calcul_tata()
{
	// TODO: de implementat calculul efectuat de către supervisor, conform indicațiilor cuprinse în diagramă!
	
}

void generare_fiu1()
{
	int fd;
	
	srand((unsigned) time(&t));
	fd = open("numere.txt", O_WRONLY);
	
	for (int i = 0; i < n*2; ++i)
	    write(fd, rand() % MAX_NR, sizeof(int));
	    
	close(fd);
}

void generare_fiu2()
{
	int fd, nr;
	
	srand((unsigned) time(&t));
	fd = open("operatori.txt", O_WRONLY);
	
	for (int i = 0; i < n; ++i)
    {   
        nr = rand() % 4;
        switch (nr)
        {
            case 0:
                write(fd, '+', sizeof(int));
                break;
            case 1:
                write(fd, '-', sizeof(int));
                break;
            case 2:
                write(fd, '*', sizeof(int));
                break;
            case 3:
                write(fd, '/', sizeof(int));
                break;
        }    
        if ()
    }
	    
	    
	close(fd);
}

int N=0;

int main()
{
	pid_t pid_fiu1, pid_fiu2;

	printf("Dati numarul intreg N:");	
	while (!scanf("%d", &N)) 
	{
	    printf("Nu ati dat un nr intreg... Incercati din nou:")	
	}

	/* Crearea procesului fiu #1. */
	if(-1 == (pid_fiu1=fork()) )
	{
		perror("Eroare la fork #1");  return 1;
	}

	/* Ramificarea execuției după primul apel fork. */
	if(pid_fiu1 == 0)
	{   /* Zona de cod executată doar de către fiul #1. */
		printf("\n[P1] Procesul fiu 1, cu PID-ul: %d.\n", getpid());

		generare_fiu1();
		return 0;
	}
	else
	{   /* Zona de cod executată doar de către părinte. */
		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());

		/* Crearea procesului fiu #2. */
		if(-1 == (pid_fiu2=fork()) )
		{
			perror("Eroare la fork #2");  return 2;
		}
		
		/* Ramificarea execuției după al doilea apel fork. */
		if(pid_fiu2 == 0)
		{   /* Zona de cod executată doar de către fiul #2. */
			printf("\n[P2] Procesul fiu 2, cu PID-ul: %d.\n", getpid());

			generare_fiu2();
			return 0;
		}
		else
		{   /* Zona de cod executată doar de către părinte. */

			wait(NULL);
			wait(NULL);
			// Mai întâi aștept terminarea ambilor fii, iar apoi fac calculul.

			calcul_tata();
			return 0;
		}
	}
}
