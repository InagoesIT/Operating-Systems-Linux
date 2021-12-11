#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
	pid_t pid1, pid2;
	int fdi, fdo;

	if(-1 == mkfifo("fifo_P0toP1", 0600) )
	{
		if(errno != EEXIST)   // errno=17 for "File already exists"
		{  perror("Eroare la crearea canalului fifo. Cauza erorii");  exit(1);  }
	}

	if(-1 == mkfifo("fifo_P2toP0", 0600) )
	{
		if(errno != EEXIST)   // errno=17 for "File already exists"
		{  perror("Eroare la crearea canalului fifo. Cauza erorii");  exit(2);  }
	}

	if(-1 == (pid1=fork()) )
	{  perror("Eroare: nu am putut crea primul fiu. Cauza erorii");  exit(3);  }

	if(pid1 == 0)
	{
	    int p1to2[2];

        /* Creare canal anonim. */
        if(-1 == pipe(p1to2) ) // pentru comunicare dintre P1 si P2
        {
            perror("Eroare la crearea canalului anonim 1 to 2");  exit(1);
        }

		if(-1 == (pid2=fork()) )
		{  perror("Eroare: nu am putut crea al doilea proces fiu. Cauza erorii");  exit(4);  }

		if(pid2 == 0) // P2
		{
			close(p1to2[1]);
        
            if ( -1 == dup2(p1to2[0], 0))
                {perror("Eroare: nu am duplica intrarea standart pu P2");  exit(4);}
            close(p1to2[0]);
            
            execl("P2","P2",NULL);
			perror("Eroare la execl() in al doilea proces fiu. Cauza erorii");  exit(7);
		}
		else // P1
		{
		    close(p1to2[0]);
			if(-1 == dup2(p1to2[1],1) )
			    //{  perror("Eroare: redirectarea iesirii standard in primul fiu a esuat. Cauza erorii");  exit(11);  }    
	        close(p1to2[1]);
	        execl("P1","P1",  NULL);
			perror("Eroare la execl() in primul fiu. Cauza erorii");  exit(12);
		}

	}
	else
	{
	    
		//if(-1 == (fdo = open("fifo_P0toP1",O_WRONLY)) )
		//{  perror("Eroare: apelul open in parinte a esuat. Cauza erorii");  exit(13);  }
	
		//if(-1 == (fdi = open("fifo_P2toP0",O_RDONLY)) )
		//{  perror("Eroare: apelul open in parinte a esuat. Cauza erorii");  exit(13);  }
		
		//close(fdi);
		//close(fdo);
	}

	return 0;  // Fluxul de execuție nu va ajunge niciodată aici!
}
