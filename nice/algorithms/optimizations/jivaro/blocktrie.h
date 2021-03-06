/* NICE
 * Copyright (C) 2017 - Authors of NICE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You can be released from the requirements of the license by purchasing a
 * commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities as mentioned in the GNU Affero General Public
 * License version 3 without disclosing the source code of your own applications.
 */

/*
 * This is the implementation of a dinamically allocated trie.
 * Definition of trie: http://en.wikipedia.org/wiki/Trie
 *
 * Each node stores an positive int value.
 *
 * To avoid syscalls for memory allocation and improve the CPU cache hit ratio,
 * it asks for TRIE_BLOCK_SIZE * sizeof(node) memory to the C library each time
 * it needs more memory and administrates that block of memory independently.
 *
 * The number of sons each node can have is limited by TRIE_MAX_SYMBOLS.
 *
 * The maximum amount of memory blocks that this trie can get is TRIE_MAX_BLOCKS.
 */



#ifndef blocktrie_h
#define blocktrie_h

#define FALSE 0
#define TRUE 1

/* The maximum amount of symbols that composes the string to be defined */
#ifndef TRIE_MAX_SYMBOLS
#define TRIE_MAX_SYMBOLS        32
#endif

/* The amount of nodes en each block of memory allocated */
#ifndef TRIE_BLOCK_SIZE
#define TRIE_BLOCK_SIZE         1600
#endif

/* The maximum amount of blocks that can be allocated */
#ifndef TRIE_MAX_BLOCKS
#define TRIE_MAX_BLOCKS         4096
#endif

#include <types.h>

/* Basic node implementation */
typedef struct nodo {
    struct nodo *		sons[TRIE_MAX_SYMBOLS];
	int					value; /* Value. If negative, then its undefined */
} nodo_t __attribute__((__aligned__(8)));


/* Trie definition structure */
typedef struct trie {
    int     lastfreeslot; 	/* Last free node in the last block */
    int		ndefs;			/* Number of elements defined */
    int     nblocks;		/* Number of blocks in use */
	size_t	memused;		/* Amount of memory used */
    nodo_t* blocks[TRIE_MAX_BLOCKS]; /* Memory blocks */
} trie_t __attribute__((__aligned__(8)));

/* Create an empty trie */
trie_t * trie_create(int nsymbols);

/* Print debugging info */
void trie_dinfo(trie_t * trie);

/* Destroy and free resources */
void trie_free(trie_t **trie);

/* Define a substring of buf starting from st and ending in end with the corresponding value*/
void trie_define(trie_t *trie, unsigned char * buf, int st, int end, int value);

/* Define a substring of buf in reverse, starting from the end and ending in st with the corresponding value*/
void trie_define_reverse(trie_t *trie, unsigned char * buf, int st, int end, int value);

/*
 * Check if a substring of buf starting from st and ending in end is defined
 * If it's defined, then it returns the value, else -1.
 */
int trie_defined(trie_t *trie, unsigned char * buf, int st, int end);

/*
 * Check if buf is defined. It should be a NULL ended string.
 * If it's defined, then it returns the value, else -1.
 */
int trie_defined_all(trie_t *trie, unsigned char * buf);

/*
 * Check if the string generated by the index value of each element in
 * the t_pair array buf, staring from st and ending in end, is defined.
 *
 * If it's defined, then it returns the value, else -1.
 */
int trie_defined_pair(trie_t *trie, t_pair * buf, int st, int end);

#endif
