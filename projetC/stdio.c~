#include "stdio.h"

extern int _flsbuf(unsigned char c, FILE *f);
extern int _filbuf(FILE * f);
FILE *fopen(const char *path, const char *mode);
void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int mode, int size);
int fclose(FILE *fp);

#define MAX 50
struct _iobuf IOB[50] = {
	{0, NULL, NULL, 0, _IOREAD, 0}
	{0, NULL, NULL, 0, _IOREAD, 1}
	{0, NULL, NULL, 0, _IOREAD, 2}
}

int _filbuf(FILE *f){
	
	int n, c;
	
	/* verifie que f est en mode lecture */
	if (f->_flag & _IORW)
         	f->_flag |= _IOREAD;

	if ((f->_flag&_IOREAD) == 0)
        	return(EOF);

	/* alloue le buffer si il est nul */
	if (f->_base == NULL){
		f->_base = malloc(MAX);
		f->_ptr = f->base;
		f->_cnt = 0;
		f->_bufsiz = MAX;
	}

	if (f == stdin) {
          if (stdout->_flag&_IOLBF)
              fflush(stdout);
          if (stderr->_flag&_IOLBF)
               fflush(stderr);
        }

	/* compte le nombre de caracteres du fichier */
	n = read(f->_file, &c, 1);
	f->_cnt = n;
	
	return (n)?c:EOF;
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

int main(void){
	int c;
	while((c==getchar())!=EOF)
		write(1, &c, 1);
}
