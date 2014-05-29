#include "stdio.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

/*
extern int _flsbuf(unsigned char c, FILE *f);
extern int _filbuf(FILE * f);
FILE *fopen(const char *path, const char *mode);
void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int mode, int size);
int fclose(FILE *fp);*/

#define MAX 50

int _filbuf(FILE *f){

	int n, c, size, courant;
	
	/* verifie que f est en mode lecture */
	if (f->_flag & _IORW){
         	f->_flag |= _IOREAD;
	}

	if ((f->_flag&_IOREAD) == 0){
        	return(EOF);
	}


	/* alloue le buffer si il est nul */
	if (!f->_base){
		f->_base = malloc(BUFSIZ);
		f->_ptr = f->_base;
		f->_cnt = 0;
		f->_bufsiz = BUFSIZ;
	}


	/*if (f == stdin) {
          if (stdout->_flag&_IOLBF)
              fflush(stdout);
          if (stderr->_flag&_IOLBF)
               fflush(stderr);
        }*/


	if(f == stdin){
		f->_cnt = MAX;
	} else {
		size = 1;
		read(fileno(f),&courant,1);
		while(courant!='\n')
		{
		        size++;
		        read(fileno(f),&courant,1);
		}

		f->_cnt = size;
	}

	write(1, f->_cnt, sizeof(int));
	return (f->_cnt>0)?size:EOF;
}

int main(void){

	char c;
	while((c==getchar())!=EOF){
		write(1, &c, 1);
	}
}
