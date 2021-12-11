#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>

int main()
{
    int fd;
    //if(-1 == (fd = open("fifo_P2toP0",O_WRONLY)) )
	    //{  perror("Eroare: apelul open in primul fiu a esuat. Cauza erorii");  exit(1);  }
	    
	int min, nr;
	int max = 0;
	int first = 1;
	
	/*while (0 != read(stdin, &nr, sizeof(nr)))
	{
	    if (nr % 2 == 0 && nr > max)
	        max = nr;
	    else 
	    {
	        if (first)
	        {	            
	            min = nr;
	        }
	        else if (min > nr)
	            min = nr;
	    }
	}
	    
	write(fd,&max,sizeof(max));
	write(fd,&min,sizeof(min));
	        
	close(fd);*/
	
    return 0;
}
