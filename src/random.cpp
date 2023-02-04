#include "random.h"

void set_random_seed()
{
	srand(time(NULL));
}

int random_int(int max_exclusive)
{
	if (max_exclusive == 0) {
		return 0;
	}
	return rand() % max_exclusive;
}

int random_int(int min_inclusive, int max_exclusive)
{
	return rand() % (max_exclusive - min_inclusive) + min_inclusive;
}
