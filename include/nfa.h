#ifndef _NFA_H
#define _NFA_H

#include <stdbool.h>

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

bool nfa_validate_state(struct nfa *nfa, int state);
bool nfa_validate_input(struct nfa *nfa, int input);

struct state_list *state_list_push(struct state_list *sl, int state);

void nfa_add_transition(struct nfa *nfa, int from, int to, int input);

#endif /* _NFA_H */
