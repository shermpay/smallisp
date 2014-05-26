#include <stdio.h>
#include <stdlib.h>
#include <editline/history.h>
#include <editline/readline.h>

#include "mpc.h"

#include "ltypes.h"

int main(int argc, char *argv[])
{
    /* Create types */
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Symbol = mpc_new("symbol");
    mpc_parser_t *Sexp = mpc_new("sexp");
    mpc_parser_t *Qexp = mpc_new("qexp");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");
    
    /* Definintions */
    mpca_lang(MPC_LANG_DEFAULT,
	      "number   : /-?[0-9]+/ ;					"
	      "symbol   : /[a-zA-z0-9_+\\-*\\/\\\\=<>!&]+/;		"
	      "sexp     : '(' <expr>* ')';				"
	      "qexp	: '{' <expr>* '}';				"
	      "expr     : <number> | <symbol> | <sexp> | <qexp>;	"
	      "lispy : /^/ <expr>* /$/ ;				",
	      Number, Symbol, Sexp, Qexp, Expr, Lispy, NULL);

    puts("Lispy version 0.0.0.5\n");
    puts("Press Ctrl-C to exit\n");

    lenv *env = lenv_new();
    lenv_add_builtins(env);
    while(1) {
	char *input = readline("lispy> ");

	add_history(input);
	
	mpc_result_t r;
	if (mpc_parse("<stdin>", input, Lispy, &r)) {
	    /* On success print AST */
	    lval *result = lval_eval(env, lval_read(r.output));
	    
	    lval_println(result);
	    lval_del(result);
	    mpc_ast_delete(r.output);
	} else {
	    /* Print error */
	    mpc_err_print(r.error);
	    mpc_err_print(r.error);
	}

	free(input);
    }
    lenv_del(env);
	
    mpc_cleanup(6, Number, Symbol, Sexp, Qexp, Expr, Lispy);
    return 0;
}
