#include "ltypes.h"

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
    return lval_take(result, 0);
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
    LASSERT_TYPE("eval", v, 0, LVAL_QEXP);

    lval* x = lval_take(v, 0);
    x->type = LVAL_SEXP;
    return lval_eval(e, x);
}

lval *builtin_var(lenv *e, lval *v, char *func)
{
    LASSERT_TYPE(func, v, 0, LVAL_QEXP);

    lval *syms = v->cell[0];
    int i;
    for (i = 0; i < syms->count; i++) {
    	LASSERT_TYPE(func, syms, i, LVAL_SYM);
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

lval *builtin_if(lenv *e, lval *v)
{
    LASSERT_TYPE("if", v, 0, LVAL_BOOL);
    lval *result = malloc(sizeof(lval));

    if (v->cell[0]->num == 1) {
	result = lval_take(v, 1);
    } else {
	result = lval_take(v, 2);
    }

    return result;
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
    lenv_add_builtin(e, "if", builtin_if);

    /* Dev functions */
}
