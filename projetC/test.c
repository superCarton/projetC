#include "stdio.h"

int main(void){
	int c;
	while((c==getchar())!=EOF)
		write(1, &c, 1);
}
