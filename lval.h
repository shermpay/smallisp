#include "mpc.h"
enum { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_FUN, LVAL_QEXP, LVAL_SEXP };

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
/* Contstructors */
lval *lval_num(long x);
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

void lenv_add_builtin(lenv *e, char *name, lbuiltin builtin);
void lenv_add_builtins(lenv *e);
