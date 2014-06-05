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
    mpc_parser_t *Bool = mpc_new("bool");
    mpc_parser_t *Symbol = mpc_new("symbol");
    mpc_parser_t *Sexp = mpc_new("sexp");
    mpc_parser_t *Qexp = mpc_new("qexp");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Smallisp = mpc_new("Smallisp");
    
    /* Definintions */
    mpca_lang(MPC_LANG_DEFAULT,
	      "number   : /-?[0-9]+/ ;					"
	      "bool	: \"true\" | \"false\"  ;			"
	      "symbol   : /[a-zA-Z0-9_+\\-*\\/=<>!&]+/;		"
	      "sexp     : '(' <expr>* ')';				"
	      "qexp	: '{' <expr>* '}';				"
	      "expr     : <number> | <bool> | <char> | <symbol> | <sexp> | <qexp>;	"
	      "Smallisp : /^/ <sexp>* /$/ ;				",
	      Number, Bool, Symbol, Sexp, Qexp, Expr, Smallisp, NULL);

    puts("Lispy version 0.0.0.7\n");
    puts("Press Ctrl-C to exit\n");

    lenv *env = lenv_new();
    lenv_add_builtins(env);
    while(1) {
	char *input = readline("slisp> ");

	add_history(input);
	
	mpc_result_t r;
	if (mpc_parse("<stdin>", input, Smallisp, &r)) {
	    /* On success print AST */
	    lval *result = lval_eval(env, lval_read(r.output));
	    
	    lval_println(result);
	    lval_del(result);
	    mpc_ast_print(r.output);
	    mpc_ast_delete(r.output);
	} else {
	    /* Print error */
	    mpc_err_print(r.error);
	    mpc_err_print(r.error);
	}

	free(input);
    }
    lenv_del(env);
	
    mpc_cleanup(7, Number, Bool, Symbol, Sexp, Qexp, Expr, Smallisp);
    return 0;
}
