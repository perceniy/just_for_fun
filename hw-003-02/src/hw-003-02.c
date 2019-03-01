/*
 ============================================================================
 Name        : hw-003-02.c
 Author      : Arceniy Sysoev
 Version     : 1.0
 Description : GeekBraibs homework № 3
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

char* binNum(unsigned short int num, char* target) {

	unsigned short int a = 0;
	char buf[16];
	short flag = 0;

	for (unsigned short int i = 0; i <= 15; i++) {
		//Будем считывать строку по биту, используя смещение
		a = (num >> (15 - i))&1u;
		if ((a == 1 )|(flag == 1)) {
			//Начинаем запись в target только когда встретили первый значащий бит
			flag = 1;
			sprintf(buf,"%d", a);
			strcat(target, buf);
		}
	}
	return target;
}

struct rectangle {
	int len;
	int wid;
	int perim;
	int square;
};

typedef struct rectangle Rectangle;

void setRectangleMeasures(Rectangle* rec) {
	rec->perim = 2*rec->len + 2*rec->wid;
	rec->square = rec->len*rec->wid;
}

int* getMemory(short length) {
	int* area = (int*) calloc(length,sizeof(int));
	return area;
}

struct point {
	double x;
	double y;
};

typedef struct point Point;

struct line {
	struct point pointA;
	struct point pointB;
	double length;
};

typedef struct line Line;

Line initLine(double x1, double y1, double x2, double y2) {
	Line l;
	l.pointA.x = x1;
	l.pointA.y = y1;
	l.pointB.x = x2;
	l.pointB.y = y2;
	l.length = sqrt(pow((l.pointA.x - l.pointB.x),2) + pow((l.pointA.y - l.pointB.y),2));
	return l;
}


int main(int argc, const char *argv[]) {
/*Task 1*/

	puts("Task 1");
	puts("");

	unsigned int num = 0;
	char target[16];
	char flag = 'n';
	do {
		//Oчищаем строку
		memset(target,0,sizeof(target));
		printf("Enter the integer number: \n");
		scanf("%d", &num);
		if (num <= USHRT_MAX) {
			printf("Your number in binary is: \n");
			puts(binNum(num, target));
		} else {
			printf("Please enter number less then %d \n",USHRT_MAX);
		}
			printf("Do you want to continue? (y/n): \n");
			scanf("%s",&flag);

	} while (flag == 'y');

/*Task 2*/
	puts("");
	puts("Task 2");
	puts("");

	const int l = 10;
	const int w = 4;
	Rectangle my_rectangle;

	my_rectangle.len = l;
	my_rectangle.wid = w;
	setRectangleMeasures(&my_rectangle);

	printf("Length: %d \n", my_rectangle.len);
	printf("Width: %d \n", my_rectangle.wid);
	printf("Perim: %d \n", my_rectangle.perim);
	printf("Square: %d \n", my_rectangle.square);

/*Task 3*/

	puts("");
	puts("Task 3");
	puts("");

	const short SIZE = 12;
	int* a = getMemory(SIZE);
	puts("Check that array contains only 0 elements:");
	for (short j = 0; j<SIZE; j++) printf("%d ", a[j]);
	puts("");
	puts("Filled array:");
	for (short j = 0; j<SIZE; j++) {a[j] = 12*j;  printf("%d ", a[j]);}

/*Task 4*/

	puts("");
	puts("Task 4");
	puts("");

	flag = 'n';
	do {
		Line l1;
		float x1, y1, x2, y2 = 0;
		puts("Please enter coordinates x1 y1 x2 y2:");
		scanf("%f %f %f %f", &x1, &y1, &x2, &y2);
		l1 = initLine(x1, y1, x2, y2);
		printf("The length of the line is: %.2f \n", l1.length);
		printf("Do you want to continue? (y/n): \n");
		scanf("%s",&flag);
	} while (flag == 'y');

 return 0;
}
