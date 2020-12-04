#include "ast.h"
#include "parser.h"
#include "tokens.h"

/**
 * @brief Predicate used by parser_while() to know if this is still a compound
 * list.
 *
 * @param w
 * @return int
 */
static int should_loop(enum words w)
{
    return w != WORD_DONE;
}

/**
 * @brief Handles the parsing of an "while" condition
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_for(struct major *mj, struct ast *ast, struct token *tk)
{
    if (ast && ast->data->word == WORD_COMMAND)
    {
        struct ast *newast = add_single_command(mj, ast, NULL);
        ast = newast;
    }
    struct token *in_list = get_next_token(mj);
    struct token *t_do = NULL;
    if (in_list->word != WORD_IN)
        t_do = in_list;
    else
        t_do = get_next_token(mj);
    if (t_do->word != WORD_DO)
    {
        token_free(t_do);
        my_err(2, mj, "parser_for: syntax error: unexpected EOF");
    }

    struct ast *newast = create_ast(mj, tk);
    struct token *expr = NULL;
    parser_cpdlist(mj, &expr, newast, should_loop);
    newast->left = create_ast(mj, in_list);
    token_free(t_do);
    token_free(expr);
    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}