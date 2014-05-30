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

int _flsbuf(unsigned char c, FILE * s) {
  
 // Test si le buffer a été créé
 if (!(s->_flag & _IOMYBUF)){
	
	s->_flag|= _IOMYBUF;
	s->_bufsiz = BUFSIZ;
	s->_ptr = s->_base = malloc(s->_bufsiz);
        if (!s->_base) {
	    s->_flag |= _IOERR;
	    return (int)EOF;
        }
 }
 if (s->_flag & _IOWRT) {
        *s->_ptr++ = c;
 }

 if (c == '\n') {
      
      if (write(s->_file, (char *)s->_base, -(s->_cnt)) != -(s->_cnt)) { 
          s->_flag |= _IOERR;
	  return (int)EOF;
      }
      s->_ptr = s->_base;
      s->_cnt = 0;
 }

} 

void tracer(FILE *f)
{
	char buffer[500];
	snprintf(buffer, 500, "TRACE FILE %p\n", f);
	write(2,buffer,strlen(buffer));
	snprintf(buffer, 500,
		"\tcnt: %d, ptr: %p, base: %p, bufsiz: %d, flag: %x, file:%d\n\n", f->_cnt, f->_ptr, f->_base, f->_bufsiz, f->_flag, f->_file);
	write(2,buffer,strlen(buffer));
}

int main(void){
	tracer(stdout);
	putc('X',stdout);
	tracer(stdout);
	putc('Y',stdout);
	tracer(stdout);
	putc('\n',stdout);
	tracer(stdout);
	return 0;
}