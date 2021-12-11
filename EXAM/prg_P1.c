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
    int fdi;
    
    //if(-1 == (fdi = open("fifo_P0toP1",O_RDONLY)) )
	    //{  perror("Eroare: apelul open in primul fiu a esuat. Cauza erorii");  exit(1);  }
		
    //close(fdi);
    
    return 0;
}
