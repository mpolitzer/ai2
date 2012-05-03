#include <stdio.h>
#include <SWI-Prolog.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

static foreign_t f0(void)
{
	printf("printf safado do C!\n");
	return TRUE;
	/* return FALSE; */
}

static foreign_t f1(term_t t0)
{
	int i;

	PL_get_integer(t0, &i);
	
	printf("param1: %d\n", i);
	return TRUE;
}

int main(int argc, char *argv[])
{
	if (!PL_initialise(1, argv)) PL_halt(1);

	PL_register_foreign("f0", 0, f0, 1);
	PL_register_foreign("f1", 1, f1, 1);
	PL_halt(PL_toplevel() ? 0 : 1);
	return 0;
}
