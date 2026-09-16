#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/conditional.h"

#define MAX_MACROS 100
#define MAX_NAME 50
#define MAX_NESTING 50

static char defined_macros[MAX_MACROS][MAX_NAME];
static int macro_count = 0;

static int is_identifier_char(char ch)
{
    return isalnum((unsigned char)ch) || ch == '_';
}

static int is_macro_defined(const char *name)
{
    int i;

    for (i = 0; i < macro_count; i++)
    {
        if (strcmp(defined_macros[i], name) == 0)
        {
            return 1;
        }
    }

    return 0;
}

static void add_macro(const char *name)
{
    if (macro_count >= MAX_MACROS)
    {
        printf("Error: too many macros\n");
        exit(EXIT_FAILURE);
    }

    if (!is_macro_defined(name))
    {
        strcpy(defined_macros[macro_count], name);
        macro_count++;
    }
}

char *process_conditionals(char *buf)
{
    char *read = buf;
    char *write = buf;

    int active = 1;

    int parent_active[MAX_NESTING];
    int condition_true[MAX_NESTING];
    int else_seen[MAX_NESTING];

    int depth = 0;

    while (*read != '\0')
    {
        char *line_start = read;
        char *line_end;
        char *p;

        char directive[20];
        char name[MAX_NAME];

        int has_directive = 0;

        /* Find end of current line */
        line_end = strchr(read, '\n');

        if (line_end == NULL)
        {
            line_end = read + strlen(read);
        }

        /*
         * Find first non-space character.
         */
        p = read;

        while (p < line_end && (*p == ' ' || *p == '\t'))
        {
            p++;
        }

        /*
         * Check whether this is a preprocessor directive.
         */
        if (p < line_end && *p == '#')
        {
            p++;

            while (p < line_end && (*p == ' ' || *p == '\t'))
            {
                p++;
            }

            if (strncmp(p, "ifdef", 5) == 0 &&
                !is_identifier_char(p[5]))
            {
                strcpy(directive, "ifdef");
                has_directive = 1;
                p += 5;
            }
            else if (strncmp(p, "ifndef", 6) == 0 &&
                     !is_identifier_char(p[6]))
            {
                strcpy(directive, "ifndef");
                has_directive = 1;
                p += 6;
            }
            else if (strncmp(p, "else", 4) == 0 &&
                     !is_identifier_char(p[4]))
            {
                strcpy(directive, "else");
                has_directive = 1;
                p += 4;
            }
            else if (strncmp(p, "endif", 5) == 0 &&
                     !is_identifier_char(p[5]))
            {
                strcpy(directive, "endif");
                has_directive = 1;
                p += 5;
            }
        }

        /*
         * #ifdef / #ifndef
         */
        if (has_directive &&
            (strcmp(directive, "ifdef") == 0 ||
             strcmp(directive, "ifndef") == 0))
        {
            if (depth >= MAX_NESTING)
            {
                printf("Error: conditional nesting too deep\n");
                exit(EXIT_FAILURE);
            }

            while (p < line_end && (*p == ' ' || *p == '\t'))
            {
                p++;
            }

            int i = 0;

            while (p < line_end && is_identifier_char(*p))
            {
                if (i < MAX_NAME - 1)
                {
                    name[i++] = *p;
                }

                p++;
            }

            name[i] = '\0';

            if (i == 0)
            {
                printf("Error: invalid conditional directive\n");
                exit(EXIT_FAILURE);
            }

            parent_active[depth] = active;

            if (strcmp(directive, "ifdef") == 0)
            {
                condition_true[depth] = is_macro_defined(name);
            }
            else
            {
                condition_true[depth] = !is_macro_defined(name);
            }

            else_seen[depth] = 0;

            active = parent_active[depth] &&
                     condition_true[depth];

            depth++;
        }

        /*
         * #else
         */
        else if (has_directive &&
                 strcmp(directive, "else") == 0)
        {
            if (depth == 0)
            {
                printf("Error: unexpected #else\n");
                exit(EXIT_FAILURE);
            }

            if (else_seen[depth - 1])
            {
                printf("Error: multiple #else\n");
                exit(EXIT_FAILURE);
            }

            else_seen[depth - 1] = 1;

            active = parent_active[depth - 1] &&
                     !condition_true[depth - 1];
        }

        /*
         * #endif
         */
        else if (has_directive &&
                 strcmp(directive, "endif") == 0)
        {
            if (depth == 0)
            {
                printf("Error: unexpected #endif\n");
                exit(EXIT_FAILURE);
            }

            depth--;

            active = parent_active[depth];
        }

        /*
         * #define
         *
         * We don't remove it here.
         * process_defines() will handle it later.
         */
        else if (active &&
                 p < line_end &&
                 strncmp(p, "define", 6) == 0 &&
                 !is_identifier_char(p[6]))
        {
            p += 6;

            while (p < line_end && (*p == ' ' || *p == '\t'))
            {
                p++;
            }

            int i = 0;

            while (p < line_end && is_identifier_char(*p))
            {
                if (i < MAX_NAME - 1)
                {
                    name[i++] = *p;
                }

                p++;
            }

            name[i] = '\0';

            if (i > 0)
            {
                add_macro(name);
            }

            /*
             * Copy the #define line so that
             * process_defines() can process it later.
             */
            while (read < line_end)
            {
                *write++ = *read++;
            }

            if (*read == '\n')
            {
                *write++ = *read++;
            }

            continue;
        }

        /*
         * Normal line.
         *
         * Copy only when currently active.
         */
        if (active && !has_directive)
        {
            while (read < line_end)
            {
                *write++ = *read++;
            }

            if (*read == '\n')
            {
                *write++ = *read++;
            }
        }
        else
        {
            /*
             * Remove the directive or inactive line.
             */
            read = line_end;

            if (*read == '\n')
            {
                read++;
            }
        }
    }

    if (depth != 0)
    {
        printf("Error: missing #endif\n");
        exit(EXIT_FAILURE);
    }

    *write = '\0';

    return buf;
}