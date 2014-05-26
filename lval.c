#include <stdio.h>
#include <stdlib.h>

#include "lval.h"

#define LASSERT(args, cond, fmt, ...)			  \
    if ((!(cond))) {					  \
	lval *err = lval_err(fmt, ##__VA_ARGS__);	  \
	lval_del(args);					  \
	return err;					  \
    }

/* Creates a new lval type of Number */
lval *lval_num(long x)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_NUM;
    v->num = x;
    return v;
}

/* Creates a new lval type of Error */
lval *lval_err(char *fmt, ...)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_ERR;
    v->err = malloc(512);

    va_list va;
    va_start(va, fmt);
    vsnprintf(v->err, 511, fmt, va);

    v->err = realloc(v->err, strlen(v->err) + 1);

    va_end(va);
    return v;
}

/* Creates a new lval type of Symbol */
lval *lval_sym(char *s)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_SYM;
    v->sym = malloc(strlen(s) + 1);
    strcpy(v->sym, s);
    return v;
}

/* Creates a new lval type of Sexp */
lval *lval_sexp(void)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_SEXP;
    v->count = 0;
    v->cell = NULL;
    return v;
}

lval *lval_qexp(void)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_QEXP;
    v->count = 0;
    v->cell = NULL;
    return v;
}

/* Creates a new lval type of Fun */
lval *lval_fun(lbuiltin func)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_FUN;
    v->builtin = func;
    return v;
}

lval *lval_lambda(lval *formals, lval *body)
{
    lval *v = malloc(sizeof(lval));
    v->type = LVAL_FUN;
    v->builtin = NULL;

    v->env = lenv_new();
    v->formals = formals;
    v->body = body;
    return v;
}

/* Delete lval, free up memory of lval and its references */
void lval_del(lval *v)
{
    switch (v->type) {
	case LVAL_NUM: break;

	case LVAL_FUN:
	    if (!v->builtin) {
		lenv_del(v->env);
		lval_del(v->formals);
		lval_del(v->body);
	    }
	    break;
	case LVAL_ERR: free(v->err); break;
	case LVAL_SYM: free(v->sym); break;
	case LVAL_QEXP:
	case LVAL_SEXP: 
	    for (int i = 0; i < v->count; i++) {
		lval_del(v->cell[i]);
	    }
	    free(v->cell);
	    break;
    }

    /* Free the struct itself */
    free(v);
}

/* add an lval to a cell */
lval *lval_add(lval *v, lval *x)
{
    v->count++;
    v->cell = realloc(v->cell, v->count * sizeof(lval*));
    v->cell[v->count - 1] = x;
    return v;
}

lval *lval_pop(lval *v, int i)
{
    lval *result = v->cell[i];
    memmove(&v->cell[i], &v->cell[i + 1], sizeof(lval*) * (v->count - i - 1));
    v->count--;

    v->cell = realloc(v->cell, sizeof(lval*) * v->count);
    return result;
}

lval *lval_take(lval *v, int i)
{
    lval *result = lval_pop(v, i);
    lval_del(v);
    return result;
}

lval *lval_copy(lval *v)
{
    lval *result = malloc(sizeof(lval));
    result->type = v->type;

    switch (v->type) {
	case LVAL_NUM: result->num = v->num; break;
	case LVAL_FUN:
	    if (v->builtin) {
		result->builtin = v->builtin;
	    } else {
		result->builtin = NULL;
		result->env = lenv_copy(v->env);
		result->formals = lval_copy(v->formals);
		result->body = lval_copy(v->body);
	    }
	    break;
	case LVAL_ERR:
	    result->err = malloc(strlen(v->err) + 1);
	    strcpy(result->err, v->err);
	    break;
	case LVAL_SYM:
	    result->sym = malloc(strlen(v->sym) + 1);
	    strcpy(result->sym, v->sym);
	    break;
	case LVAL_SEXP:
	case LVAL_QEXP:
	    result->count = v->count;
	    result->cell = malloc(sizeof(lval*) * result->count);
	    int i;
	    for (i = 0; i < result->count; i++) {
		result->cell[i] = lval_copy(v->cell[i]);
	    }
	    break;
    }
    return result;
}

char *ltype_name(int t)
{
    switch(t) {
	case LVAL_FUN: return "<function>";
	case LVAL_NUM: return "<number>";
	case LVAL_ERR: return "<error>";
	case LVAL_SYM: return "<symbol>";
	case LVAL_SEXP: return "<sexp>";
	case LVAL_QEXP: return "<qexp>";
	default: return "<undefined>";
    }
}

lval *lval_read_num(mpc_ast_t *t)
{
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    lval *result =  errno != ERANGE ? lval_num(x) : lval_err("invalid number");
    return result;
}

lval *lval_read(mpc_ast_t *t)
{
    if (strstr(t->tag, "number")) { return lval_read_num(t); }
    if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

    /* if root(>) or sexp than create empty list */
    lval *lst = NULL;
    if (strstr(t->tag, "qexp")) { lst = lval_qexp(); }
    if (strcmp(t->tag, ">") == 0) { lst = lval_sexp(); }
    if (strstr(t->tag, "sexp")) { lst = lval_sexp(); }

    /* Fill the list */
    int i;
    for (i = 0; i < t->children_num; i++) {
	/* check for parens */
	if (strcmp(t->children[i]->contents, "(") == 0 ||
	    strcmp(t->children[i]->contents, ")") == 0) {
	    continue;
	}
	if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
	if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
	if (strcmp(t->children[i]->tag, "regex") == 0) { continue; }
	lst = lval_add(lst, lval_read(t->children[i]));
    }
    return lst;
}

/* Printing expr */
void lval_expr_print(lval *v, char open, char close)
{
    putchar(open);
    int i;
    for (i = 0; i < v->count; i++) {
	lval_print(v->cell[i]);
	if (i != (v->count - 1)) {
	    putchar(' ');
	}
    }
    putchar(close);
}

/* Print lval */
void lval_print(lval *v)
{
    switch (v->type){
	case LVAL_NUM: printf("%li", v->num); break;
	case LVAL_ERR: printf("Error: %s", v->err); break;
	case LVAL_SYM: printf("%s", v->sym); break;
	case LVAL_FUN:
	    if (v->builtin) {
		printf("<builtin>");
	    } else {
		printf("(lambda: ");
		lval_print(v->formals);
		putchar(' ');
		lval_print(v->body);
		putchar(')');
	    }
	    break;
	case LVAL_QEXP: lval_expr_print(v, '{', '}'); break;
	case LVAL_SEXP: lval_expr_print(v, '(', ')'); break;
    }
}

/* Prints lval with a new line */
void lval_println(lval *v) { lval_print(v); putchar('\n'); }

lval *lval_call(lenv *e, lval *func, lval *arg_vals)
{
    if (func->builtin) { return func->builtin(e, arg_vals); }

    int given = arg_vals->count;
    int total = func->formals->count;

    while (arg_vals->count) {
	if (func->formals->count == 0) {
	    lval_del(arg_vals);
	    return lval_err("Function passed too many args. Got %i. Expected %i.",
			    given, total);
	}

	lval *sym = lval_pop(func->formals, 0);
	if (strcmp(sym->sym, "&") == 0) { /* For 'rest' params */
	    if (func->formals->count != 1) {
		lval_del(arg_vals);
		return lval_err("<function> args invalid. Symbol should follow after '&'.");
	    }
	    lval *nsym = lval_pop(func->formals, 0);
	    lenv_update(e, nsym, builtin_list(e, a));
	    lval_del(sym); lval_del(nsym);
	    break;
	}
	lval *val = lval_pop(arg_vals, 0);
	/* Assign each formal the value passed in through arg_vals */
	lenv_update(func->env, sym, val);
	lval_del(sym); lval_del(val);
    }

    lval_del(arg_vals);

    /* if '&' remains, bound to empty list */
    if (func->formals->count == 0 &&
	(strcmp(func->formals->cell[0]->sym, "&") == 0)) {
	if (f->formals->count != 2) {
	    lval_err("<function> requires symbol after '&'");
	}
	lval_del(lval_pop(func->formals, 0));

	lval *sym = lval_pop(func->formals, 0);
	lval *val = lval_qexp();

	lenv_update(func->e, sym, val);
	lval_del(sym); lval_del(val);
    }
    if (func->formals->count == 0) {
	func->env->par = e; 		/* Assign parent */
	return builtin_eval(func->env, lval_add(lval_sexp(), lval_copy(func->body)));
    } else {
	return lval_copy(func);
    }
}

/* Builtin Functions */
lval *builtin_op(lenv *e, lval *v, char *op)
{
    int i;
    for (i = 0; i < v->count; i++) {
	if (v->cell[i]->type != LVAL_NUM) {
	    lval_del(v);
	    return lval_err("Cannot operate on non-number");
	}
    }

    lval *x = (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) ? lval_num(0) : lval_num(1);
    if (strcmp(op, "/") == 0 && v->count > 1) {
	x = lval_pop(v, 0);
    }

    while (v->count > 0) {
	lval *y = lval_pop(v, 0);

	if (strcmp(op, "+") == 0) { x->num += y->num; }
	if (strcmp(op, "-") == 0) { x->num -= y->num; }
	if (strcmp(op, "*") == 0) { x->num *= y->num; }
	if (strcmp(op, "/") == 0) {
	    if (y->num == 0) {
		lval_del(x); lval_del(y);
		return lval_err("Division by zero");
	    }
	    x->num /= y->num;
	}
	lval_del(y);
    }

    lval_del(v);
    return x;
}

lval *builtin_add(lenv *e, lval *v) { return builtin_op(e, v, "+"); }
lval *builtin_sub(lenv *e, lval *v) { return builtin_op(e, v, "-"); }
lval *builtin_mul(lenv *e, lval *v) { return builtin_op(e, v, "*"); }
lval *builtin_div(lenv *e, lval *v) { return builtin_op(e, v, "/"); }

lval *builtin_lambda(lenv *e, lval *v)
{
    LASSERT(v, (v->count == 2), "<lambda> defn invalid. Requires arg list and body");
    LASSERT(v, v->cell[0]->type == LVAL_QEXP,
	    "<lambda>, arg list invalid type, requires <qexp>, found %s", v->cell[0]);
    int i;
    for (i = 0; i < v->cell[0]->count; i++) {
	LASSERT(v, v->cell[0]->cell[i]->type == LVAL_SYM,
		"Cannot define non-symbol. Got %s. Expected %s",
		ltype_name(v->cell[0]->cell[i]->type), ltype_name(LVAL_SYM));
    }
    lval *args = lval_pop(v, 0);
    lval *body = lval_pop(v, 0);
    lval_del(v);
    return lval_lambda(args, body);
}

lval *builtin_head(lenv *e, lval *v)
{
    LASSERT(v, (v->count == 1),
	    "'head': Invalid arg count. Passed incorrect arg count. Got %i. Expected %i",
	    v->count, 1);
    LASSERT(v, (v->cell[0]->type == LVAL_QEXP),
	    "'head': Incorrect type for arg 1. Got %s. Expected %s",
	    ltype_name(v->cell[0]->type), ltype_name(LVAL_QEXP));
    LASSERT(v, (v->cell[0]->count != 0), "Passed in empty list"); 

    lval *result = lval_take(v, 0);
    while (result->count > 1) {
	lval_del(lval_pop(result, 1));
    }
    return result->cell[0];
}

lval *builtin_tail(lenv *e, lval *v)
{
    LASSERT(v, v->count == 1, "'tail': Passed incorrect arg count. Got %i. Expected %i",
	    v->count, 1);
    LASSERT(v, v->cell[0]->type == LVAL_QEXP,
	    "'tail': Incorrect type for arg 1. Got %s. Expected %s",
	    ltype_name(v->cell[0]->type), ltype_name(LVAL_QEXP));
    LASSERT(v, v->cell[0]->count != 0, "Passed in empty list");

    lval *result = lval_take(v, 0);
    lval_del(lval_pop(result, 0));
    return result;
}

lval *builtin_list(lenv *e, lval *v)
{
    v->type = LVAL_QEXP;
    return v;
}

lval *builtin_eval(lenv *e, lval *v)
{
    LASSERT(v, (v->count == 1), "Function 'eval' passed too many arguments!");
    LASSERT(v, (v->cell[0]->type == LVAL_QEXP), "Function 'eval' passed incorrect type!");

    lval* x = lval_take(v, 0);
    x->type = LVAL_SEXP;
    return lval_eval(e, x);
}

lval *builtin_var(lenv *e, lval *v, char *func)
{
    LASSERT(v, v->cell[0]->type == LVAL_QEXP,
	    "'%s': Passed incorrect type for arg 1. Got %s. Got Expected %s",
	    func, ltype_name(v->cell[0]->type), ltype_name(LVAL_QEXP));

    lval *syms = v->cell[0];
    int i;
    for (i = 0; i < syms->count; i++) {
	LASSERT(v, (syms->cell[i]->type == LVAL_SYM),
		"'%s': Cannot define non-symbol", func);
    }

    LASSERT(v, (syms->count == v->count - 1),
	    "'%s': Requires 1 expression to 1 symbol", func);

    for (i = 0; i < syms->count; i++) {
	if (strcmp(func, "let"))  {
	    lenv_update(e, syms->cell[i], v->cell[i + 1]);
	}

	if (strcmp(func, "def")) {
	    lenv_def(e, syms->cell[i], v->cell[i + 1]);
	}
    }

    lval_del(v);
    return lval_sexp();
}

lval *builtin_def(lenv *e, lval *v) { return builtin_var(e, v, "def"); }
lval *builtin_let(lenv *e, lval *v) { return builtin_var(e, v, "let"); }

lval *builtin_todo(lenv *e, lval *v)
{
    puts("1. Basic Interpreter \t Done");
    puts("2. Basic Evaluation \t Done");
    puts("3. Basic environment \t Done");
    puts("4. Basic builtins \t Done");
    puts("5. Basic variables \t Done");
    puts("6. Error reporting \t Done");
    puts("7. Defining functions \t TODO");
    return v;
}

/* Evaluates Sexps */
lval *lval_eval_sexp(lenv *e, lval *v)
{
    int i;
    /* Evaluate children */
    for (i = 0; i < v->count; i++) {
	v->cell[i] = lval_eval(e, v->cell[i]);
    }

    /* Error checking */
    for (i = 0; i < v->count; i++) {
	if (v->cell[i]->type == LVAL_ERR) {
	    return lval_take(v, i);
	}
    }

    if (v->count == 0) { return v; }
    if (v->count == 1) { return lval_take(v, 0); }

    /* Checks first element is Symbol */
    lval *f = lval_pop(v, 0);
    if (f->type != LVAL_FUN) {
	lval *err =
	    lval_err("S-expressions starts with incorrect type. Got %s, Expected %s.",
		     ltype_name(f->type), ltype_name(LVAL_FUN));
	lval_del(f); lval_del(v);
	return err;
    }

    /* lookup symbol */
    lval *result = lval_call(e, f, v);
    lval_del(f);
    return result;
}

lval *lval_eval(lenv *e, lval *v)
{
    if (v->type == LVAL_SYM) {
	lval *x = lenv_lkup(e, v);
	lval_del(v);
	return x;
    }
    if (v->type == LVAL_SEXP) {
	return lval_eval_sexp(e, v);
    }
    return v;
}

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

void lenv_add_builtin(lenv *e, char *name, lbuiltin func)
{
    lval *f = lval_fun(func);
    lval *k = lval_sym(name);
    lenv_update(e, k, f);
    lval_del(k); lval_del(f);
}

void lenv_add_builtins(lenv *e)
{
    /* Arithmetic operators */
    lenv_add_builtin(e, "+", builtin_add); lenv_add_builtin(e, "-", builtin_sub);
    lenv_add_builtin(e, "*", builtin_mul); lenv_add_builtin(e, "/", builtin_div);

    /* List functions */
    lenv_add_builtin(e, "head", builtin_head);
    lenv_add_builtin(e, "tail", builtin_tail);
    lenv_add_builtin(e, "list", builtin_list);

    /* Core functions */
    lenv_add_builtin(e, "lambda", builtin_lambda);
    lenv_add_builtin(e, "def", builtin_def);
    lenv_add_builtin(e, "let", builtin_let);
    lenv_add_builtin(e, "eval", builtin_eval);

    /* Dev functions */
}
