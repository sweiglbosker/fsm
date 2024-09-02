#include <stdio.h>
#include <stdlib.h>

#include "nfa.h"

struct nfa *nfa_alloc(int n_states, int n_input, int initial_state, int accepting_state)
{
	struct nfa *nfa = malloc(sizeof(struct nfa));

	if (nfa == NULL) 
		return nfa;
	
	nfa->initial_state = initial_state;
	nfa->accepting_state = accepting_state;
	nfa->n_states = n_states;
	nfa->n_input = n_input;

	nfa->transition_table = malloc(sizeof(struct state_list *) * n_states);

	if (nfa->transition_table) {
		free(nfa);
		return NULL;
	}

	for (int i = 0; i < n_states; i++) {
		nfa->transition_table[i] = calloc(nfa->n_input, sizeof(struct state_list *));
	}

	return nfa;
}

bool nfa_validate_state(struct nfa *nfa, int state)
{
	if (state > (nfa->n_states - 1)) 
		return false;
	else
		return true;
}

bool nfa_validate_input(struct nfa *nfa, int input)
{
	if (input > nfa->n_input - 1)
		return false;
	else 
		return true;
}

void nfa_add_transition(struct nfa *nfa, int from, int to, int input)
{
	if (nfa == NULL) {
		fprintf(stderr, "nfa_add_transition: nfa is NULL\n");
		return;
	}

	nfa_validate_input(nfa, input);
	nfa_validate_state(nfa, from);
	nfa_validate_state(nfa, to);

	struct state_list *sl = nfa->transition_table[from][input];

	nfa->transition_table[from][input] = state_list_push(sl, to);
}

struct state_list *state_list_push(struct state_list *sl, int state)
{
	struct state_list *head = malloc(sizeof(struct state_list));

	if (head == NULL)
		return NULL;

	head->state = state;
	head->next = sl;

	return head;
}
