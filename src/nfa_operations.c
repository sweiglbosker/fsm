#include "nfa.h"
#include "fsm.h"

struct nfa *nfa_concat(struct nfa *a, struct nfa *b)
{
	struct nfa *c = b;

	nfa_table_grow(c, a->n_states);
	nfa_table_shift(c, a->n_states);
	nfa_table_fill(c, a);

	nfa_add_transition(c, a->accepting_state, c->initial_state, INPUT_EPSILON); // error here

	c->initial_state = a->initial_state;

	return b;
}

struct nfa *nfa_union(struct nfa *a, struct nfa *b)
{
	int initial_state = 0;
	int accepting_state = 1;

	struct nfa *c = b;

	nfa_table_grow(c, a->n_states + 2);
	nfa_table_shift(c, a->n_states);
	nfa_table_fill(c, a);
	nfa_table_shift(c, 2);

	/* our initial and accepting states don't have a transition table yet */
	nfa_table_alloc(c, initial_state);
	nfa_table_alloc(c, accepting_state);
	
	nfa_add_transition(c, initial_state, c->initial_state, INPUT_EPSILON);
	nfa_add_transition(c, initial_state, a->initial_state + 2, INPUT_EPSILON);

	nfa_add_transition(c, c->accepting_state, accepting_state, INPUT_EPSILON);
	nfa_add_transition(c, a->accepting_state + 2, accepting_state, INPUT_EPSILON);

	c->accepting_state = accepting_state;

	c->initial_state = initial_state;

	return c;
}

struct nfa *nfa_star(struct nfa *nfa)
{
	nfa_table_grow(nfa, 2);
	nfa_table_shift(nfa, 2);

	nfa_table_alloc(nfa, 0); // new final state
	nfa_table_alloc(nfa, 1); // new initial state

	nfa_add_transition(nfa, 0, nfa->initial_state, INPUT_EPSILON);
	nfa_add_transition(nfa, 0, 1, INPUT_EPSILON);
	nfa_add_transition(nfa, nfa->accepting_state, nfa->initial_state, INPUT_EPSILON);
	nfa_add_transition(nfa, nfa->accepting_state, 1, INPUT_EPSILON);

	nfa->initial_state = 0;
	nfa->accepting_state = 1;

	return nfa;
}

struct nfa *nfa_plus(struct nfa *nfa)
{
	nfa_table_grow(nfa, 1);
	nfa_table_shift(nfa, 1);

	nfa_table_alloc(nfa, 0);

	nfa_add_transition(nfa, nfa->accepting_state, nfa->initial_state, INPUT_EPSILON);
	nfa_add_transition(nfa, nfa->accepting_state, 0, INPUT_EPSILON);

	nfa->accepting_state = 0;

	return nfa;
}

struct nfa *nfa_question(struct nfa *nfa) 
{
	nfa_table_grow(nfa, 2);
	nfa_table_shift(nfa, 2);

	nfa_table_alloc(nfa, 0);
	nfa_table_alloc(nfa, 1);

	nfa_add_transition(nfa, 0, 1, INPUT_EPSILON);
	nfa_add_transition(nfa, 0, nfa->initial_state, INPUT_EPSILON);
	nfa_add_transition(nfa, nfa->accepting_state, 1, INPUT_EPSILON);

	nfa->initial_state = 0;
	nfa->accepting_state = 1;

	return nfa;
}
