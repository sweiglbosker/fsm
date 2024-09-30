#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nfa.h"
#include "fsm.h"

#define MAX(a, b)	((a > b) ? a : b)
#define MIN(a, b)	((a < b) ? a : b)

struct nfa *nfa_alloc(int n_states, int n_input, int initial_state, int accepting_state)
{
	struct nfa *nfa = malloc(sizeof(struct nfa));

	if (nfa == NULL) 
		return nfa;
	
	nfa->initial_state = initial_state;
	nfa->accepting_state = accepting_state;
	nfa->n_states = n_states;
	nfa->n_input = n_input;

	nfa->transition_table = malloc(sizeof(struct state_list **) * n_states);

	if (!nfa->transition_table) {
		free(nfa);
		return NULL;
	}

	for (int i = 0; i < n_states; i++) {
		nfa->transition_table[i] = calloc(nfa->n_input, sizeof(struct state_list *));
	}

	return nfa;
}

void nfa_free(struct nfa *nfa)
{
	for (int i = 0; i < nfa->n_states; i++) {
		nfa_table_free_state(nfa, i);
	}
	free(nfa->transition_table);
	free(nfa);
}

void nfa_table_alloc(struct nfa *nfa, int state)
{
	if (nfa->transition_table[state] != NULL) {
		fprintf(stderr, "[WARNING] nfa_table_alloc(): there is already a transition table allocated for this state\n");
	}

	nfa->transition_table[state] = calloc(nfa->n_input, sizeof(struct state_list *));
}

void nfa_table_free_state(struct nfa *nfa, int state)
{
	for (int i = 0; i < nfa->n_input; i++) {
		struct state_list *sl = nfa->transition_table[state][i];
		if (sl != NULL) {
			state_list_free(sl);
		}
	}
	free(nfa->transition_table[state]);
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

	struct state_list *sl = nfa->transition_table[from][input]; // error here

	nfa->transition_table[from][input] = state_list_push(sl, to);
}

int nfa_add_states(struct nfa *nfa, int n)
{
	if (n <= 0)
		return 0;

	int old_n_states = nfa->n_states;
	nfa->n_states += n;
	nfa->transition_table = realloc(nfa->transition_table, sizeof(struct state_list **) * nfa->n_states);

	if (nfa->transition_table == NULL) {
		fprintf(stderr, "libfsm: failed to realloc transition table\n");
		exit(1);
	}

	for (int i = old_n_states; i < nfa->n_states; i++) {
		nfa->transition_table[i] = calloc(nfa->n_input, sizeof(struct state_list *));
	}

	return old_n_states;
}

int nfa_table_grow(struct nfa *nfa, unsigned int n) 
{
	if (n <= 0)
		return 0;

	int new_state = nfa->n_states;
	nfa->n_states += n;

	nfa->transition_table = realloc(nfa->transition_table, nfa->n_states * sizeof(struct state_list **));
	memset(&(nfa->transition_table[new_state]), '\0', n * sizeof(struct state_list **));

	return new_state;
}

void nfa_table_shift(struct nfa *nfa, unsigned int n) 
{
	if (n >= nfa->n_states) {
		fprintf(stderr, "nfa_table_shift(): shifing transition table out of buffer\n");
		// maybe just zero transition table here?
		return;
	}

	memmove(&nfa->transition_table[n], nfa->transition_table, (nfa->n_states - n) * sizeof(struct state_list **));
	memset(nfa->transition_table, '\0', n * sizeof(struct state_list **));
	nfa->initial_state += n;
	nfa->accepting_state += n;

	for (int i = n; i < nfa->n_states; i++) {
		if (nfa->transition_table[i] == NULL)
			continue;

		for (int j = 0; j < nfa->n_input; j++) {
			struct state_list *sl = nfa->transition_table[i][j];
			while (sl != NULL) {
				sl->state += n;
				sl = sl->next;
			}
		}
	}
}

void nfa_table_fill(struct nfa *dst, struct nfa *src) {
	int copy_len = MIN(dst->n_states, src->n_states);
	if (src->n_states >= dst->n_states) {
		fprintf(stderr, "nfa_table_fill(): dst is smaller than target\n");
	}

	for (int i = 0; i < copy_len; i++) {
	//	if (dst->transition_table[i] != NULL) {
	//		nfa_table_free_state(dst, i);
	//	}	
	}

	memcpy(dst->transition_table, src->transition_table, copy_len * sizeof(struct state_list **));
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

void state_list_free(struct state_list *sl)
{
	struct state_list *next = NULL;

	while (sl) {
		next = sl->next;
		free(sl);
		sl = next;
	}
}

void state_list_print(struct state_list *sl)
{
	while (sl) {
		if (sl->next == NULL) {
			printf("%d", sl->state);
		} else {
			printf("%d, ", sl->state);
		}
		sl = sl->next;
	}
}

void nfa_print(struct nfa *nfa) 
{
	printf("nfa@%p\n", (void *)nfa);
	printf("--------------\n");
	printf("n_input: %d\n", nfa->n_input);
	printf("n_states: %d\n", nfa->n_states);
	printf("initial: %d, accepting: %d\n\n", nfa->initial_state, nfa->accepting_state);	

	for (int i = 0; i < nfa->n_states; i++) {
		printf("transitions for state %d:\n", i);
		for (int j = 0; j < nfa->n_input; j++) {
			if (nfa->transition_table[i][j] != NULL) {
				printf("%d->(", i);
				state_list_print(nfa->transition_table[i][j]);
				printf(") on input %d\n", j); 
				putchar('\n');
			}
		}
	}
}
