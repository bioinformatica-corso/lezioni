#include "hash.h"

void insert(struct occurrence** occ, uint32_t pos){
	struct occurrence* o;
	HASH_FIND_INT(*occ, &pos, o);
	/* Checks if the key is already in the hash or not */
	if(o == NULL){
		o = (struct occurrence*) malloc (sizeof(struct occurrence));
		o->pos = pos;
		o->count = 1;
		HASH_ADD_INT(*occ, pos, o);
	}else{
		HASH_DEL(*occ, o);
		o->count++;
		HASH_ADD_INT(*occ, pos, o);
	}
}

void visit(struct occurrence** occ, uint32_t rounds, uint32_t m, char* text){
	struct occurrence* tmp, *s;
	/* Deletion-safe iteration on the elements of the hash */
	HASH_ITER(hh, *occ, s, tmp){
		if(s->count == rounds){
			char* x = strndupa(text + s->pos, m);
                        printf("Occurrence %s at position %d\n", x, s->pos);
		}
		free(s);
	}
}