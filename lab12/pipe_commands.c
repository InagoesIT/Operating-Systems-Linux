/*
*   Programul #1: executia lantului w -h | tr -s " " | cut -d" " -f1,8 | sort -t " " -k 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	pid_t pid1, pid2, pid3;
	int pipe_Cmd1toCmd2[2], pipe_Cmd2toCmd3[2], pipe_Cmd3toCmd4[2];

	if(-1 == pipe(pipe_Cmd3toCmd4) )
	{  fprintf(stderr, "Eroare: nu pot crea al treilea canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(1);  }

	if(-1 == (pid1=fork()) )
	{  perror("Eroare: nu am putut crea primul fiu. Cauza erorii");  exit(2);  }

	if(pid1 == 0)
	{ // Secțiune de cod executată doar de către fiul procesului inițial.
		if(-1 == pipe(pipe_Cmd2toCmd3) )
		{  fprintf(stderr, "Eroare: nu pot crea al doilea canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(3);  }
	
		if(-1 == (pid2=fork()) )
		{  perror("Eroare: nu am putut crea al doilea proces fiu. Cauza erorii");  exit(4);  }

		if(pid2 == 0)
		{ // Secțiune de cod executată doar de către "nepotul" procesului inițial.
			
			if(-1 == pipe(pipe_Cmd1toCmd2) )
		    {  fprintf(stderr, "Eroare: nu pot crea primul canal anonim, errno=%d\n",errno);  perror("Cauza erorii");  exit(5);  }
		    
		    if(-1 == (pid3=fork()) )
		    {  perror("Eroare: nu am putut crea al treilea proces fiu. Cauza erorii");  exit(6);  }
	
	
	        if (pid3 == 0)
	        { //stra-nepotul procesului initial
	            if(-1 == dup2(pipe_Cmd1toCmd2[1],1) )
			    {  perror("Eroare: redirectarea iesirii standard in al doilea proces fiu a esuat. Cauza erorii");  exit(7);  }
	
	            execlp("w","w","-h",NULL);
			    perror("Eroare la execlp() in al treilea proces fiu. Cauza erorii");  exit(8);
	        }
	        else
	        {	        			    
			    close(pipe_Cmd1toCmd2[1]); // E necesar acest close pentru ca să se termine execuția comenzii tr
	
			    if(-1 == dup2(pipe_Cmd1toCmd2[0],0) )
			    {  perror("Eroare: redirectarea intrarii standard in al doilea fiu a esuat. Cauza erorii"); exit(9);  }

			    if(-1 == dup2(pipe_Cmd2toCmd3[1],1) )
			    {  perror("Eroare: redirectarea iesirii standard in primul fiu a esuat. Cauza erorii");  exit(10);  }
	    
			    execlp("tr","tr","-s"," ",NULL);
			    perror("Eroare la execlp() in al doilea fiu. Cauza erorii");  exit(11);
	        }
		}
		else
		{ // Secțiune de cod executată doar de către fiul procesului inițial.
			close(pipe_Cmd2toCmd3[1]); // E necesar acest close pentru ca să se termine execuția comenzii cut
	
			if(-1 == dup2(pipe_Cmd2toCmd3[0],0) )
			{  perror("Eroare: redirectarea intrarii standard in primul fiu a esuat. Cauza erorii"); exit(12);  }

			if(-1 == dup2(pipe_Cmd3toCmd4[1],1) )
			{  perror("Eroare: redirectarea iesirii standard in primul fiu a esuat. Cauza erorii");  exit(13);  }
	
			execlp("cut","cut","-d ","-f1,8",NULL);
			perror("Eroare la execlp() in primul fiu. Cauza erorii");  exit(14);
		}
	}
	else
	{ // Secțiune de cod executată doar de către părinte (i.e., procesul inițial).
		close(pipe_Cmd3toCmd4[1]); // E necesar acest close pentru ca să se termine execuția comenzii sort

		if(-1 == dup2(pipe_Cmd3toCmd4[0],0) )
		{  perror("Eroare: redirectarea intrarii standard in parinte a esuat. Cauza erorii");  exit(15);  }

		execlp("sort","sort","-t"," ","-k","1",NULL);
		perror("Eroare la execlp() in parinte. Cauza erorii");  exit(16);
	}

	return 0;  // Fluxul de execuție nu va ajunge niciodată aici!
}
