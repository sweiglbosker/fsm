#ifndef _NFA_H
#define _NFA_H

#include <stdbool.h>
#include "fsm.h"

// TODO: handle allocation of per-state transition table better. (ex: no need to alloc (nfa->n_input * sizeof(ptr)) sized buffer for dead state) 
// in most cases, each state will only have a few transitions, so a simple hashing algorithm might work well
// TODO: transition to 0 state vs no transition
struct state_list {
	int state;
	struct state_list *next;
};

struct nfa {
	int n_input;	/* decides the size of jump table */
	int n_states;
	int initial_state, accepting_state;
	struct state_list ***transition_table;
};

struct nfa *nfa_alloc(int n_states, int n_input, int initial_state, int final_state);
void nfa_free(struct nfa *nfa);

bool nfa_validate_state(struct nfa *nfa, int state);
bool nfa_validate_input(struct nfa *nfa, int input);

struct state_list *state_list_push(struct state_list *sl, int state);
void state_list_print(struct state_list *sl);
void state_list_free(struct state_list *sl);

void nfa_add_transition(struct nfa *nfa, int from, int to, int input);

/* add n new states and return the first new state */
int nfa_add_states(struct nfa *nfa, int n);

void nfa_table_alloc(struct nfa *nfa, int state);
/* free input transition table for individual state */ 
void nfa_table_free_state(struct nfa *nfa, int state);
void nfa_table_fill(struct nfa *dst, struct nfa *src);
/* warning: nfa_table_grow does not allocate transition table for the new states, mainly for internal use */
int nfa_table_grow(struct nfa *nfa, unsigned int n);

void nfa_table_shift(struct nfa *nfa, unsigned int n);

struct nfa *nfa_concat(struct nfa *a, struct nfa *b); /* concatenate two nfa fragments into a single fragment */

struct nfa *nfa_union(struct nfa *a, struct nfa *b); /* a|b */

struct nfa *nfa_star(struct nfa *nfa); /* zero or more occurence of nfa */

struct nfa *nfa_plus(struct nfa *nfa); /* one or more occurence of nfa */

struct nfa *nfa_question(struct nfa *nfa); /* zero or one occurence of nfa */

void nfa_print(struct nfa *nfa);

#endif /* _NFA_H */
