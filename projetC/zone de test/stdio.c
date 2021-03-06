#include "stdio.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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

FILE *fopen(const char *filename, const char *openmode)
{

    int fd;
    unsigned mode;
    FILE *file;

    if (!strcmp(openmode, "r")){
        mode |= (unsigned)O_RDONLY;
    } else if (!strcmp(openmode, "w")){
        mode |= (unsigned)(O_WRONLY | O_CREAT | O_TRUNC);
    } else if (!strcmp(openmode, "rw")){
	mode |= (unsigned)(O_RDONLY | O_WRONLY | O_CREAT | O_TRUNC);
    }    

    /* open a file */
    fd = open((char *)filename, mode,
              (unsigned)(S_IRUSR | S_IWUSR |
                         S_IRGRP | S_IWGRP |
                         S_IROTH | S_IWOTH));

    if (fd == -1){ /* erreur a l'ouverture */
	errno = ENOENT;
	return NULL;
    }
    else { /* creer le fichier */

        if (!(file = (FILE *)malloc(sizeof(FILE)))) {
	    close(fd);
	    errno = ENOMEM;
	    return NULL;
	}

        file->_ptr = NULL;
        file->_cnt = 0;
        file->_base = NULL;
        file->_flag = 0;
	file->_bufsiz = (isatty(fd)) ? 1 : BUFSIZ;

        if (!strcmp(openmode, "rw"))
            file->_flag |= _IORW;
        else if (!strcmp(openmode, "w"))
            file->_flag |= _IOWRT;
        else if (!strcmp(openmode, "r"))
            file->_flag |= _IOREAD;
	else {
	    close(fd);
	    return NULL;
	}
  
        return file;
    }
}

void setbuf(FILE *stream, char *buf){
    if (buf != NULL)
        setvbuf(stream, buf, _IOFBF, BUFSIZ);
    else
        setvbuf(stream, NULL, _IONBF, 0);
}

int setvbuf(FILE *stream, char *buf, int mode, int size){

    int flag = 0;
    int buf_size;
    unsigned char *base = NULL;
    
    
    if (stream->_cnt || stream->_base)
        return -1;

    if (mode == _IOFBF) {		/* I/O bufferisé totaalement */
        flag &= ~_IOLBF & ~_IONBF;
        flag |= _IOFBF;
        buf_size = BUFSIZ;

    } else if (mode == _IOLBF) {	/* I/O bufferisé ligne par ligne */
        flag &= ~_IONBF & ~_IOFBF;
        flag |= _IOLBF;
        buf_size = BUFSIZ;

    } else if (mode == _IONBF) {	/* I/O débufferisé */
        flag &= ~_IOFBF & ~_IOLBF;
        flag |= _IONBF;
        buf_size = 1;

    } else {
        return -1;
    }

    if (buf) {
        base = (unsigned char *)buf;
        flag |= _IOMYBUF;
    }

    stream->_flag |= flag;
    stream->_bufsiz  = buf_size;

    if (base)  {
    	stream->_ptr = base;
    	stream->_base = base;
    }

    return 0;
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
