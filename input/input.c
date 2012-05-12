#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char mat[20][20];

int func(int x, int y)
{
	int vet[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
	int i;
	
	for(i = 0; i < 4; i++)
	{
		int nx = vet[i][0]+x;
		int ny = vet[i][1]+y;

		if(nx < 0) continue;
		if(ny < 0) continue;
		if(nx >= 20) continue;
		if(ny >= 20) continue;

		if(mat[ny][nx] == 'R') return 1;
	}
	return 0;
}

int main(int argc, const char *argv[])
{
	int i,j, x, y;

	srand ( time(NULL) );

	for(i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++) 
		{
			scanf(" %c", &mat[i][j]);
		}
	}

	for (i = 0; i < 60; i++) 
	{
		do
		{
			x = rand()%20;	
			y = rand()%20;
		}
		while(mat[y][x] == 'R');

		j = rand()%10;

		if(j) printf("Z:%d,%d,%d\n", x, y, j);
	}

	for (i = 0; i < 4; i++) 
	{
		do
		{
			x = rand()%20;	
			y = rand()%20;
		}
		while(mat[y][x] == 'X' || !func(x, y));

		printf("H:%d,%d,%d\n", x, y, rand()%10);
	}

	for (i = 0; i < 4; i++) 
	{
		do
		{
			x = rand()%20;	
			y = rand()%20;
			j = rand()%100;
		}
		while(mat[y][x] == 'X' || !func(x, y) || j < 30);

		printf("M:%d,%d,%d\n", x, y, j);
	}

	for (i = 0; i < 20; i++) {
		for (j = 0; j < 20; j++) {
			printf("%c", mat[i][j]);
		}
		printf("\n");
	}

	return 0;
}
