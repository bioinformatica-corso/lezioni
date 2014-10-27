#define _GNU_SOURCE
#include "uthash/src/uthash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct occurrence{
	uint32_t pos;
	uint32_t count;
	UT_hash_handle hh;
};

void insert(struct occurrence** occ, uint32_t pos);
void visit(struct occurrence** occ, uint32_t rounds, uint32_t m, char* text);