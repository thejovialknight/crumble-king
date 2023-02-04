#pragma once
#include <stdlib.h>
#include <time.h>

void set_random_seed();
int random_int(int max_exclusive);
int random_int(int min_inclusive, int max_exclusive);
