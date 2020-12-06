#include "ast.h"
#include "major.h"
#include "parser.h"

struct ast *parser_operator(struct major *mj, struct ast *ast, struct token *tk)
{
    struct ast *newast = create_ast(mj, tk);
    newast->left = ast;
    newast->right = take_action(mj, newast->right, get_next_token(mj));
    return newast;
}