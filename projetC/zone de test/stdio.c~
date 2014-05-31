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
int readed = EOF;
	
/* verifie que f est en mode lecture */
if (f->_flag & _IORW){
       	f->_flag |= _IOREAD;
}

if (!(f->_flag&_IOREAD)){
       	return(EOF);
}


/* alloue le buffer si il est nul */
if (!f->_base){
	f->_flag|= _IOMYBUF;
	f->_base = malloc(BUFSIZ);
	f->_ptr = f->_base;
	f->_cnt = 0;
	f->_bufsiz = BUFSIZ;
}

/* calcule la taille initiale du compteur */
if (f->_bufsiz) {
	size = 1;
	read(fileno(f),&courant,1);
	while(courant!='\n')
	{
		size++;
		read(fileno(f),&courant,1);
	}
	f->_cnt = size;
} else {
   	f->_cnt = 0;
}

/* verifie que la valeur du compteur est correcte */
if (f->_cnt < 0) {
	f->_flag |= _IOERR;
} else if (!f->_cnt) {
	f->_flag |= _IOEOF;
} else {
	f->_flag &= ~_IOEOF;
	f->_cnt--;
	readed = *f->_ptr++;
}

return readed;
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

int fflush(FILE *f)
{
	if (f->_flag & _IOWRT && (f->_flag & _IOMYBUF) && f->_base-f->_ptr!=0)
	{
		if (write(f->_file,f->_base,-f->_cnt)!=-f->_cnt){
			return EOF;
		} else {
			f->_ptr=f->_base;
			f->_cnt=0;
		}
	}
	return 0;
	
}

int fclose(FILE *fp)
{
	int r = EOF;
	if (fp->_flag & (_IOREAD|_IOWRT|_IORW))
	{
		r = fflush(fp);
    		close(fp->_file);
		if (fp->_flag & _IOMYBUF){
			free(fp->_base);
		}
	}
        if (fp == stdin || fp == stdout || fp == stderr){
        	r = 0;
	}
	fp->_cnt = 0;
	fp->_base=(char *)NULL;
	fp->_ptr=(char *)NULL;
	fp->_bufsiz=0;
	fp->_flag=0;
	fp->_file=0;
	return r;
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
	fclose(stdout);
	//putc('\n',stdout);
	tracer(stdout);
/*tracer(stdin);
getchar();
tracer(stdin);
getchar();
tracer(stdin);*/

	return 0;
}
