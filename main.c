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

int main(int argc, char *argv[])
{
	if (!PL_initialise(1, argv)) PL_halt(1);

	PL_register_foreign("f0", 0, f0, 1);
	PL_halt(PL_toplevel() ? 0 : 1);
	return 0;
}
