/*
 * main.c
 *
 *  Created on: 1 Mar 2019
 *  Author: Arceniy Sysoev
 *  Abstract: Two recursive algorithm of Fibonacci sequence.
 */

#include <stdio.h>

long fibnext(int N, long* a, long* p) {
	*p = *p+1;
	if (N == 1) {
		a[N] = 1;
		return a[N];
	} else if (N == 0) {
		a[N] = 0;
		return a[N];
	} else {
		if ((N > 1)&(a[N-1]==0)) a[N-1] = fibnext(N-1, a, p);
		a[N] = a[N-1] + a[N-2];
		return a[N];
	}
}

long fib(int N, long* p) {
	/*Дурной алгоритм работающий за 2^n*/
	*p = *p+1;
	if (N == 1) {
		return 1;
	} else if (N == 0) {
		return 0;
	} else 	{
		return fib(N-1, p) + fib(N-2, p);
	}
}


int main (int argc, const char* argv[]) {
	const int SIZE = 46;
	long p =0,p1 = 0;
	long f = 0,f2 = 0;
	long a[SIZE];

	for (int i = 1; i < SIZE; i++) {
			f2 = fibnext(i,a,&p1);
			printf("%3d: %15ld iterations: %10ld \n", i, f2, p1);
			//Очистим массив для честности
			for (int j =0; j<SIZE; j++) a[j] = 0;
		}

	printf("\n Second way \n");

	for (int i = 0; i < SIZE; i++) {
		f = fib(i,&p);
		printf("%3d: %15ld iterations: %10ld \n", i, f, p);
	}

	return 0;
}


