//comment.c
#include <stdio.h>
#include <stdlib.h>
#include "../include/comment.h"

void remove_comments(char *buf)
{
    char *read = buf;
    char *write = buf;
    int closed;
    while (*read != '\0')
    {
        /* String literal */
        if (*read == '"')
        {
            closed=0;
            *write++ = *read++;
            while (*read != '\0')
            {
                /* Escaped character inside string */
                if (*read == '\\')
                {
                    *write++ = *read++;
                    if (*read != '\0')
                    {
                        *write++ = *read++;
                    }
                }
                /* Closing quote */
                else if (*read == '"')
                {
                    *write++ = *read++;
                    closed=1;
                    break;
                }
                else
                {
                    *write++ = *read++;
                }
            }
            if (!closed)
            {
                printf("Error: unterminated string literal\n");
                exit(EXIT_FAILURE);
            }
        }

        /* Character literal */
        else if (*read == '\'')
        {
            *write++ = *read++;
            while (*read != '\0')
            {
                /* Escaped character inside character literal */
                if (*read == '\\')
                {
                    *write++ = *read++;
                    if (*read != '\0')
                    {
                        *write++ = *read++;
                    }
                }
                /* Closing quote */
                else if (*read == '\'')
                {
                    *write++ = *read++;
                    break;
                }
                else
                {
                    *write++ = *read++;
                }
            }
        }

        /* Single-line comment */
        else if (*read == '/' && *(read + 1) == '/')
        {
            read += 2;

            while (*read != '\n' && *read != '\0')
            {
                read++;
            }
        }

        /* Multi-line comment */
        else if (*read == '/' && *(read + 1) == '*')
        {
            read += 2;
            while (*read != '\0' && !(*read == '*' && *(read + 1) == '/'))
            {
                read++;
            }
            if (*read == '\0')
            {
                printf("Error: unterminated block comment\n");
                exit(EXIT_FAILURE);
            }
            read += 2;
        }
        
        /* Normal character */
        else
        {
            *write++ = *read++;
        }
    }
    *write = '\0';
}