//macro.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/macro.h"

static int is_identifier_char(char ch)
{
    return isalnum((unsigned char)ch) || ch=='_';
}

static void replace_macro(char **buf, char *name, char *value)
{
    char *p=*buf;

    while(*p!='\0')
    {
        /* Skip string literal */
        if(*p=='"')
        {
            p++;
            while(*p!='\0')
            {
                if(*p=='\\')
                {
                    p++;
                    if(*p!='\0')
                    {
                        p++;
                    }
                }
                else if(*p=='"')
                {
                    p++;
                    break;
                }
                else
                {
                    p++;
                }
            }
            continue;
        }

        /* Skip character literal */
        if(*p=='\'')
        {
            p++;
            while(*p!='\0')
            {
                if(*p=='\\')
                {
                    p++;
                    if(*p!='\0')
                    {
                        p++;
                    }
                }
                else if(*p=='\'')
                {
                    p++;
                    break;
                }
                else
                {
                    p++;
                }
            }
            continue;
        }

        /* Check for macro */
        if(strncmp(p,name,strlen(name))==0)
        {
            size_t name_len=strlen(name);
            size_t value_len=strlen(value);

            /* Check identifier boundaries */
            if((p==*buf || !is_identifier_char(*(p - 1))) &&!is_identifier_char(*(p + name_len)))
            {
                size_t old_size=strlen(*buf);
                size_t new_size=old_size-name_len+value_len;
                size_t position=p-*buf;

                char *temp=realloc(*buf,new_size+1);
                if(temp==NULL)
                {
                    printf("Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
                *buf=temp;
                p=*buf+position;
                memmove(p+value_len,p+name_len,old_size-position-name_len+1);
                memcpy(p,value,value_len);
                p+=value_len;
                continue;
            }
        }
        p++;
    }
}

char *process_defines(char *buf)
{
    char *p;
    while((p = strstr(buf, "#define"))!=NULL)
    {
        /* #define must start a line, allowing spaces/tabs */
        if(p!=buf && *(p-1)!='\n')
        {
            p+=7;
            continue;
        }
        char *line_start=p;

        /* Move past #define */
        p+=7;

        /* Skip spaces */
        while(*p==' ' || *p=='\t')
        {
            p++;
        }

        /* Read macro name */
        char name[50];
        int i=0;

        while(is_identifier_char(*p))
        {
            if(i<sizeof(name)-1)
            {
                name[i++]=*p;
            }
            p++;
        }
        name[i]='\0';
        if(i==0)
        {
            printf("Error: invalid #define\n");
            exit(EXIT_FAILURE);
        }

        /* Skip spaces before value */
        while(*p==' ' || *p=='\t')
        {
            p++;
        }

        /* Find end of line */
        char *line_end=strchr(p,'\n');
        if(line_end==NULL)
        {
            line_end=p+strlen(p);
        }

        /* Read macro value */
        char value[200];
        int value_len=line_end-p;

        if(value_len>0 && p[value_len-1]=='\r')
        {
            value_len--;
        }

        if(value_len>=sizeof(value))
        {
            printf("Error: macro value too long\n");
            exit(EXIT_FAILURE);
        }

        strncpy(value,p,value_len);
        value[value_len]='\0';

        /* Remove #define line */
        if(*line_end=='\n')
        {
            memmove(line_start,line_end+1,strlen(line_end+1)+1);
        }
        else
        {
            *line_start='\0';
        }
        replace_macro(&buf,name,value);
    }
    return buf;
}