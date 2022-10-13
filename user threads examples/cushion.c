/**
Dal momento in cui longjpm() sovrascrive lo stack, il valore di z viene perso.
Creando un contesto "cuscinetto", ci permette di generare sufficiente spazio in maniera che
il contesto della funzione dothings() sia sufficientemente traslato verso il basso nello stack,
che il suo contenuto sia salvo dalla sovrascrittura.

Questa operazione è possibile solo ed unicamente qualora dothings() non abbia un valore di ritorno.
Nel caso ci si provasse, il contesto di cushion() è con grande probabilità corrotto, generando un errore.
*/
#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <unistd.h>

static jmp_buf main_buf, dothings_buf;

void dothings() {
	int z = 1313;
	if(!setjmp(dothings_buf)) {
		printf("Now I'm here, z=%d\n", z);
		longjmp(main_buf, 42);
	} else {
		printf("Now I'm back here, z=%d\n", z);
		exit(0);
	}
}

void cushion() {
	/* Allocare 1000 byte, senza mai usarli, potrebbe portare il compilatore ad ottimizzare
	il codice. Definendo l'ultimo valore, si obbliga ad allocare la memoria.*/
	char data_cushion[1000];
	data_cushion[999] = 1;
	dothings();
}

int main() {
	if (!setjmp(main_buf)) {
		cushion();
	} else {
		longjmp(dothings_buf, 17);
	}
	return 0;
}
