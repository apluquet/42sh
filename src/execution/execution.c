#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "lexer.h"
#include "my_utils.h"
#include "my_xmalloc.h"

char **token_list_to_char_array(struct list *l)
{
    char **tab = my_xcalloc(NULL, l->size + 1, sizeof(char **));

    struct list_item *cpy = l->head;
    for (size_t i = 0; cpy && i < l->size; i++)
    {
        tab[i] = cpy->data;
        cpy = cpy->next;
    }
    return tab;
}

int run_command(char **args)
{
    int pid = fork();
    if (!pid)
    {
        execvp(args[0], args);
        exit(127);
    }
    int rvalue = 0;
    waitpid(pid, &rvalue, 0);
    rvalue = WEXITSTATUS(rvalue);
    if (rvalue == 127)
        fprintf(stderr, "Command not found zeubi : %s\n", args[0]);
    return rvalue;
}