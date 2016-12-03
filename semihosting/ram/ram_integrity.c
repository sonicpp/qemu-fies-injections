#include <angelio.h>
#include <stdlib.h>
#include <string.h>
#include "hvd.h"

void do_stuff()
{
	for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 10000; j++)
			;
}

int main(void)
{
	struct HVD hvd;

	aprintf("Data address %p - %p (size %d)\n", hvd.data,
		&hvd.data[ROWS - 1], ROWS * sizeof(int));

	/* Generate data */
	srand(42);
	for (int i = 0; i < ROWS; i++)
		hvd.data[i] = rand();


	/* Calc parity */
	HVD_calc_parity(&hvd);	

	do_stuff();

	/* Check/repair data */
	if (!HVD_check_integrity(&hvd)) {
		aprintf("Data mismatch!\n");
		aprintf("%d bit flips repaired\n", HVD_repair_data(&hvd));
	}
	if (!HVD_check_integrity(&hvd))
		aprintf("Data still erroneous\n");
	else
		aprintf("Data valid\n");


	return 0;
}

