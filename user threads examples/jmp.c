/**
setjpm() genera un checkpoint di contesto esecuzione.
longjmp() attiva il checkpoint portando il programma allo stato di esecuzione salvato in precedenza

Nel corso di questa operazione, viene sovrascritto lo stack. Per questo motivo potrebbe darsi che
alcune variabili vengano sovrascritte e percui, ne si perdono i valori.

cushion.c mostra come risolvere il problema.
*/


#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>

static jmp_buf buf;

void dothings() {
	printf("Now I'm here\n");
	sleep(3);
	longjmp(buf, 42);
	printf("This is never printed\n");
}

int main() {
	if (!setjmp(buf)) { // the first time returns 0
		dothings();
	} else {
		printf("Now I'm there\n");
	}
	return 0;
}