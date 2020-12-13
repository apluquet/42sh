#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#include "source.h"

#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "b_utils.h"

static char *iter_dir(char *base_path, char *dest_path);

static char *get_rec_path(char *base_path, char *dir_name, char *dest_path)
{
    char *rec_path = strdup(base_path);
    rec_path = realloc(
        rec_path, sizeof(char) * (strlen(rec_path) + strlen(dir_name) + 2));

    strcpy(rec_path, base_path);
    strcat(rec_path, "/");
    strcat(rec_path, dir_name);

    rec_path = iter_dir(rec_path, dest_path);
    return rec_path;
}

static char *iter_dir(char *base_path, char *dest_path)
{
    struct dirent *dp;
    DIR *dir = opendir(base_path);

    if (!dir)
        return NULL;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if (dp->d_type == DT_DIR)
            {
                char *rec_path = get_rec_path(base_path, dp->d_name, dest_path);
                if (rec_path)
                {
                    closedir(dir);
                    return rec_path;
                }
            }

            else if (!strcmp(dp->d_name, dest_path))
            {
                char *ret = strdup(base_path);
                ret = realloc(
                    ret, sizeof(char) * (strlen(ret) + strlen(dp->d_name) + 2));
                if (strcat(ret, "/") && strcat(ret, dp->d_name))
                    closedir(dir);
                return ret;
            }
        }
    }
    closedir(dir);
    return NULL;
}

static char *get_path(char *str)
{
    if (strstr(str, "/") != NULL)
        return str;

    char *PATH = getenv("PATH");
    char *index = PATH;

    char *found = iter_dir(".", str);
    if (found)
        return found;

    while (index)
    {
        char *next_path = strstr(index, ":");

        char *actual_path = strndup(index, next_path - index);
        char *found = iter_dir(actual_path, str);
        if (found)
        {
            free(actual_path);
            return found;
        }

        index = strstr(index, ":");

        if (index)
            index++;

        free(actual_path);
        actual_path = NULL;
    }

    return NULL;
}

char **b_source(char *argv[])
{
    int argc = argv_len(argv);

    if (argc < 2)
    {
        warnx("filename required");
        return NULL;
    }

    char *path = get_path(argv[1]);
    argv[1] = path;
    return argv + 1;
}