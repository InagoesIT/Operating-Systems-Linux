
/*
*  
*Să se scrie un program C care realizează urmatoarele: mai întâi creează două procese fii, după care generează aleator într-un fișier linii de text. După închiderea fișierului în care *scrie acele linii de text aleatoare, procesul inițial va trimite fiilor creați semnalul SIGUSR2 (primului fiu) și respectiv SIGUSR1 (celui de-al doilea fiu). La recepționarea *semnalului, procesul fiu care a primit SIGUSR1 va calcula numărul de linii de text din fișier (i.e., numărul de caractere '\n') și îl va afișa pe ecran, iar procesul fiu care a *primit SIGUSR2 va sorta liniile de text din fișier în ordine lexicografică crescătoare și va afișa pe ecran rezultatul sortării.
*Cerință: sortarea să se facă în paralel cu calculul numărului de linii, dar rezultatele să fie afișate pe ecran de cele două procese fii în așa fel încât numărul total de linii de *text să apară obligatoriu înaintea liniilor de text sortate.
*(Indicație: pentru a realiza acest tip de sincronizare la afișarea pe ecran, procesul care face sortarea va aștepta un semnal (e.g. SIGUSR1) de la celălalt proces fiu, înainte de a *afișa rezultatul sortării.)

*/


#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int nr_of_messages = 0;

void treaterror(const char *message)
{
    fprintf(stderr, "\nEroare la %s.", message);
    perror(" Cauza erorii: ");
    exit(nr_of_messages);
}

void generate_file()
{
    int i, j;
    int fd;
    
    srand(time(NULL));
    if (-1 == (fd = open("generated.txt", O_WRONLY | O_TRUNC)))
        treaterror("Cannot open generated.txt");


    N = rand() % 20 + 10;
    int size = 0;
    for (i = 1; i <= N; i++)
    {
        int lg = rand() % 20 + 10;
        size += lg;
        for (j = 1; j <= lg; j++)
        {
            char ch = 'a' + rand() % 26;
            write(fd, &ch, sizeof(char));
        }
        char newline = '\n';
        write(fd, &newline, sizeof(char));
        size++;
    }
    nr_of_messages++;
    if (-1 == (close(fd)))
        treaterror("Cannot close generated.txt");
}

void handler_sigusr1(int nr_sem)
{
	fprintf(stderr,"Debug: procesul cu PID-ul %d a primit semnalul de notificare.\n", getpid());
	/*
		Notă: acesta este doar un mesaj cu rol de debugging; se poate "ignora" la execuție prin redirectarea stderr:
		UNIX> ./a.out  2> /dev/null
	*/
}

int main()
{
	int pid1, pid2;
	sigset_t masca_semnale_usr12_blocat, masca_semnale_usr1_deblocat, masca_semnale_usr2_deblocat;

	sigemptyset(&masca_semnale_usr12_blocat);
	sigaddset(&masca_semnale_usr12_blocat,SIGUSR1);
	sigaddset(&masca_semnale_usr12_blocat,SIGUSR2);

	sigfillset(&masca_semnale_usr1_deblocat);
	sigdelset(&masca_semnale_usr1_deblocat,SIGUSR1);

	sigfillset(&masca_semnale_usr2_deblocat);
	sigdelset(&masca_semnale_usr2_deblocat,SIGUSR2);

    nr_of_messages++;
	if(SIG_ERR == signal(SIGUSR1,handler_sigusr1) )
	  treaterror("Eroare la signal");

    nr_of_messages++;
    if(SIG_ERR == signal(SIGUSR2,handler_sigusr1) )
	    treaterror("Eroare la signal");
	    
	nr_of_messages++;
	if(-1 == sigprocmask(SIG_BLOCK,&masca_semnale_usr12_blocat,NULL) )
	    treaterror("Eroare la sigprocmask");
    
    nr_of_messages++;
	if(-1 == (pid1=fork()) )
	    treaterror("Eroare la sigprocmask");

	if(pid1 == 0) // fiu 1 -> SIGUSR2
	{ /* Fiul : va fi responsabil cu sortarea liniilor de text... */	    
	    
	    sigsuspend(&masca_semnale_usr2_deblocat);  /* Aștept notificarea de la tata pu a continua. */
	
	    system("sort generated.txt -o sorted.txt");
	    
	    sigsuspend(&masca_semnale_usr1_deblocat);  /* Aștept notificarea de la celalt fiu pentru a afisa. */
	    
	    int fd,cod_r;
	    char ch;

	    if(-1 == (fd = open("sorted.txt",O_RDONLY)) )
	    {
		    perror("Eroare la deschiderea fisierului sorted.txt");  exit(1);
	    }

	    printf("Sorted:\n");
	    do{
	        nr_of_messages++;
		    if(-1 == (cod_r = read(fd,&ch,sizeof(char))) )
		        treaterror("Eroare la citirea din fisier");
		    if(cod_r == 1) /* Dacă a citit cu succes un octet, îl afișăm. */
		    {
			    printf("%c",ch);
			    fflush(stdout);
		    }
	    }while(cod_r != 0);  /* Executăm bucla de citire și afișare până întâlnim EOF în fișier. */
	    printf("\n");

	close(fd);
	}
	else
	{ 
	    nr_of_messages++;
	    if(-1 == (pid2=fork()) )
	         treaterror("Eroare la fork");
	    
	    if (pid2 == 0) /* Fiul 2 -> SIGUSR 1*/
	    { /* Fiul : va fi responsabil cu numararea liniilor de text... */
	        
	        sigsuspend(&masca_semnale_usr1_deblocat);  /* Aștept notificarea de la tata pu a continua. */
	
	        int fd,cod_r, lineCount;
	        char ch;

            nr_of_messages++;
	        if(-1 == (fd = open("generated.txt",O_RDONLY)) )
	            treaterror("Eroare la fork");

	        do{
	            nr_of_messages++;
		        if(-1 == (cod_r = read(fd,&ch,sizeof(char))) )
		            treaterror("Eroare la citirea din fisier");
		            
		        if(cod_r == 1 && ch == "\n") /* Dacă a citit cu succes un octet, îl afișăm. */
			        lineCount++;
			        
	        }while(cod_r != 0);  /* Executăm bucla de citire și afișare până întâlnim EOF în fișier. */
	        printf("Avem %d linii \n", lineCount);
	            
	        kill(pid1,SIGUSR1);
	    }
	    else
	    {
	        generate_file();

            kill(pid1, SIGUSR2);

            kill(pid2, SIGUSR1);
            
            wait(NULL);
            wait(NULL);      
		
		    printf("\nSfarsit.\n");
	    }
	}
    
	return 0;
}
