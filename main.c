#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <SWI-Prolog.h>

#define NUM_HOSPITAIS	4
#define NUM_DELEGACIAS	4
#define NUM_ZUMBIS	60

#define MAPAW 20
#define MAPAH 20

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

struct hospital {
	int x, y;
	int antidotos;
};

struct delegacia {
	int x, y;
	int municao;
};

struct zumbi {
	int x, y;
	int quantidade;
};

struct game_info {
	struct hospital hospitais[NUM_HOSPITAIS];
	struct delegacia delegacias[NUM_DELEGACIAS];
	struct zumbi zumbis[NUM_ZUMBIS];
	int num_hospitais;
	int num_delegacias;
	int num_zumbis;
	char mapa[MAPAH][MAPAW];
} GI;

void lerarquivo(FILE *file)
{
	char c;
	int i,j,x,y,z;

	while (fscanf(file, " %c:%d,%d,%d", &c, &x, &y, &z) == 4) {
		switch (c) {
			case 'H': case 'h':
				GI.hospitais[GI.num_hospitais].x = x;
				GI.hospitais[GI.num_hospitais].y = y;
				GI.hospitais[GI.num_hospitais].antidotos = z;
				GI.num_hospitais++;
				break;
			case 'M': case 'm':
				GI.delegacias[GI.num_delegacias].x = x;
				GI.delegacias[GI.num_delegacias].y = y;
				GI.delegacias[GI.num_delegacias].municao = z;
				GI.num_delegacias++;
				break;
			case 'Z': case 'z':
				GI.zumbis[GI.num_zumbis].x = x;
				GI.zumbis[GI.num_zumbis].y = y;
				GI.zumbis[GI.num_zumbis].quantidade = z;
				GI.num_zumbis++;
				break;
		}
	}
	ungetc(c, file);

	for (i=0; i<MAPAH; i++) {
		char buf[255];

		fgets(buf, 255, file);

		for (j=0; !isspace(buf[j]); j++) {
			GI.mapa[i][j] = buf[j];
		}
	}
}

void dump(void)
{
	int i,j;

	for (i=0; i<GI.num_hospitais; i++) {
		struct hospital *h = &GI.hospitais[i];
		printf("hospitais: %d, %d, %d\n", h->x, h->y, h->antidotos);
	}
	for (i=0; i<GI.num_delegacias; i++) {
		struct delegacia *d = &GI.delegacias[i];
		printf("delegacias: %d, %d, %d\n", d->x, d->y, d->municao);
	}
	for (i=0; i<GI.num_zumbis; i++) {
		struct zumbi *z = &GI.zumbis[i];
		printf("zumbis: %d, %d, %d\n", z->x, z->y, z->quantidade);
	}

	for (i=0; i<MAPAH; i++) {
		for (j=0; j<MAPAW; j++) {
			printf("%c", GI.mapa[i][j]);
		}
		printf("\n");
	}
}

static foreign_t ouve(term_t _x, term_t _y, term_t ret)
{
	int i, x, y, num_zumbies=0;

	PL_get_integer(_x, &x);
	PL_get_integer(_y, &y);

	for (i=0; i<GI.num_zumbis; i++) {
	}

	if (num_zumbies) {
		return TRUE;
	}
	return FALSE;
}

int main(int argc, char *argv[])
{
	lerarquivo(stdin);
	dump();

	if (!PL_initialise(argc, argv)) {
		fprintf(stderr, "failed to initialize prolog\n");
		exit(1);
	}
	PL_register_foreign("ouve", 2, ouve, 1);

	return 0;
}
