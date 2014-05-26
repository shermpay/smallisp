#include "ltypes.h"
/* Environment code */
/* Constructs a new environment */
lenv *lenv_new(void)
{
    lenv *e = malloc(sizeof(lenv));
    e->par = NULL;
    e->count = 0;
    e->syms = NULL;
    e->lvals = NULL;
    return e;
}

void lenv_del(lenv *e)
{
    int i;
    for (i = 0; i < e->count; i++) {
	free(e->syms[i]);
	lval_del(e->lvals[i]);
    }
    free(e->syms);
    free(e->lvals);
    free(e);
}

lval* lenv_lkup(lenv *e, lval *k)
{
    int i;
    for (i = 0; i < e->count; i++) {
	if (strcmp(e->syms[i], k->sym) == 0) {
	    return lval_copy(e->lvals[i]);
	}
    }

    if (e->par) {
	return lenv_lkup(e->par, k);
    } else {
	return lval_err("Unbound symbol: %s", k->sym);
    }
}

void lenv_update(lenv *e, lval *k, lval *v)
{
    int i;
    for (i = 0; i < e->count; i++) {
	if (strcmp(e->syms[i], k->sym) == 0) {
	    lval_del(e->lvals[i]);
	    e->lvals[i] = lval_copy(v);
	    return;
	}
    }

    e->count++;
    e->lvals = realloc(e->lvals, sizeof(lval*) * e->count);
    e->syms = realloc(e->syms, sizeof(char*) * e->count);
    int idx = e->count - 1;
    e->lvals[idx] = lval_copy(v);
    e->syms[idx] = malloc(strlen(k->sym) + 1);
    strcpy(e->syms[idx], k->sym);
}

lenv *lenv_copy(lenv *e)
{
    lenv *res = malloc(sizeof(lenv));
    res->par = e->par;
    res->count = e->count;

    res->syms = malloc(sizeof(char*) * res->count);
    res->lvals = malloc(sizeof(lval*) * res->count);

    int i;
    for (i = 0; i < e->count; i++) {
	res->syms[i] = malloc(strlen(e->syms[i]) + 1);
	strcpy(res->syms[i], e->syms[i]);
	res->lvals[i] = lval_copy(e->lvals[i]);
    }
    return res;
}

void lenv_def(lenv *e, lval *k, lval *v)
{
    while (e->par) { e = e->par; }
    lenv_update(e, k, v);
}
