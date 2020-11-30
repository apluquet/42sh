#include <stdio.h>

#include "lexer.h"
#include "tokens.h"

static void red()
{
    printf("\033[1;31m");
}

static void yellow()
{
    printf("\033[0;36m");
}

static void reset()
{
    printf("\033[0m");
}

static void print_token_redir(struct token *tk)
{
    if (tk->redirection->std_in)
        printf(" < %s", tk->redirection->std_in);
    if (tk->redirection->std_out)
        printf(" 1%s %s",
               tk->redirection->std_out_append_mode == REDIR_TRUNK ? ">" : ">>",
               tk->redirection->std_out);
    if (tk->redirection->std_err)
        printf(" 2%s %s",
               tk->redirection->std_err_append_mode == REDIR_TRUNK ? ">" : ">>",
               tk->redirection->std_err);
}

void print_token(struct token *tk)
{
    red();
    printf("%s", token2string(tk));

    if (tk->word == WORD_COMMAND)
    {
        struct list_item *li = tk->data->head;
        for (size_t i = 0; i < tk->data->size; i++)
        {
            yellow();
            printf(" %s", li->data);
            li = li->next;
        }
    }
    else if (tk->word == WORD_REDIR)
    {
        print_token_redir(tk);
    }
    reset();
    printf("\n");
}

void lexer_printer(struct lexer *lex)
{
    if (!lex)
    {
        printf("lexer_printer: Nothing to print\n");
        return;
    }

    struct token_list *tmp = lex->head;

    while (tmp)
    {
        struct token *tk = tmp->tk;
        print_token(tk);
        tmp = tmp->next;
    }
}