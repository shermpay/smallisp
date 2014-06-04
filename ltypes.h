#include "mpc.h"
enum { LVAL_NUM, LVAL_BOOL, LVAL_CHAR, LVAL_ERR, LVAL_SYM, LVAL_FUN, LVAL_QEXP, LVAL_SEXP };

typedef int bool_t;
#define FALSE 0
#define TRUE 1

struct lval;
typedef struct lval lval;
struct lenv;
typedef struct lenv lenv;
typedef lval* (*lbuiltin)(lenv*, lval*);
struct lval {
    lval** cell;
    lbuiltin builtin;

    lenv *env;
    lval *formals;
    lval *body;

    char* err;
    char* sym;

    long num;

    int type;
    int count;
};

struct lenv {
    char **syms;
    lval **lvals;

    lenv *par;

    int count;
};


#define LASSERT(args, cond, fmt, ...)			  \
    if ((!(cond))) {					  \
	lval *err = lval_err(fmt, ##__VA_ARGS__);	  \
	lval_del(args);					  \
	return err;					  \
    }

#define LASSERT_TYPE(func, args, index, extype)			\
    LASSERT(args, ((args->cell[index]->type) == (extype)),	\
	    "TypeError. In '%s': Expected %s; Got %s", func,	\
	    ltype_name(extype), ltype_name(args->cell[index]->type))

/* Contstructors */
lval *lval_num(long x);
lval *lval_bool(bool_t p);
lval *lval_err(char *fmt, ...);
lval *lval_sym(char *s);
lval *lval_sexp(void);
lval *lval_qexp(void);
lval *lval_fun(lbuiltin builtin);
lval *lval_lambda(lval *formals, lval *body);
/* Manipulation functions */
void lval_del(lval *v);
lval *lval_add(lval *v, lval *x);
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
lval *lval_copy(lval *v);
lval *lval_equal(lval *x, lval *y, lval *arg, int arg_idx);
/* Reading functions */
lval *lval_read_num(mpc_ast_t *t);
lval *lval_read(mpc_ast_t *t);
/* Printing functions */
void lval_print(lval *v);
void lval_println(lval *v);
void lval_expr_print(lval *v, char open, char close);
/* Evaluation functions */
lval *lval_eval_sexp(lenv *e, lval *v);
lval *lval_eval(lenv *e, lval *v);
lval *lval_call(lenv *e, lval *func, lval *arg_vals);

/* Environment functions */
lenv *lenv_new(void);
void lenv_del(lenv *e);
lval *lenv_lkup(lenv *e, lval *v);
void lenv_update(lenv *e, lval *k, lval *v);
lenv *lenv_copy(lenv *e);
void lenv_def(lenv*, lval*, lval*);

/* Builtin functions */
lval *builtin_op(lenv *e, lval *v, char *op);
lval *builtin_add(lenv *e, lval *v);
lval *builtin_sub(lenv *e, lval *v);
lval *builtin_mul(lenv *e, lval *v);
lval *builtin_div(lenv *e, lval *v);
lval *builtin_lambda(lenv *e, lval *v);
lval *builtin_head(lenv *e, lval *v);
lval *builtin_tail(lenv *e, lval *v);
lval *builtin_list(lenv *e, lval *v);
lval *builtin_eval(lenv *e, lval *v);
lval *builtin_var(lenv *e, lval *v, char *func);
lval *builtin_def(lenv *e, lval *v);
lval *builtin_let(lenv *e, lval *v);
lval *builtin_if(lenv *e, lval *v);

void lenv_add_builtin(lenv *e, char *name, lbuiltin builtin);
void lenv_add_builtins(lenv *e);

/* Other functions */
char *ltype_name(int t);

