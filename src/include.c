#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/include.h"
#include "../include/comment.h"

#define SYSTEM_INCLUDE_PATH "/usr/include/"

static char *read_header(const char *filename)
{
    FILE *fp;
    long int size;
    char *buf;
    fp=fopen(filename, "rb");
    if(fp==NULL)
    {
        printf("Error: cannot open header %s\n", filename);
        return NULL;
    }
    fseek(fp,0,SEEK_END);
    size=ftell(fp);
    rewind(fp);
    buf=malloc(size+1);
    if(buf==NULL)
    {
        printf("Memory allocation failed\n");
        fclose(fp);
        return NULL;
    }
    if(size>0)
    {
        if(fread(buf,size,1,fp)!=1)
        {
            printf("Failed to read header %s\n", filename);
            free(buf);
            fclose(fp);
            return NULL;
        }
    }
    buf[size]='\0';
    fclose(fp);
    remove_comments(buf);
    return buf;
}

static void get_directory(const char *filename, char *directory)
{
    const char *last_slash1;
    const char *last_slash2;
    const char *last_slash;

    last_slash1=strrchr(filename, '/');
    last_slash2=strrchr(filename, '\\');

    if(last_slash1>last_slash2)
    {
        last_slash=last_slash1;
    }
    else
    {
        last_slash=last_slash2;
    }

    if(last_slash!=NULL)
    {
        int length=last_slash-filename;
        strncpy(directory,filename,length);
        directory[length]='\0';
    }
    else
    {
        strcpy(directory,".");
    }
}

static char *replace_include(char *buf,char *line_start,char *line_end,char *header_buf)
{
    size_t old_size;
    size_t include_size;
    size_t header_size;
    size_t new_size;
    size_t position;

    char *temp;
    old_size=strlen(buf);
    if(*line_end=='\n')
    {
        include_size=line_end-line_start+1;
    }
    else
    {
        include_size=line_end-line_start;
    }
    header_size=strlen(header_buf);
    new_size=old_size-include_size+header_size;
    position=line_start-buf;
    temp=realloc(buf,new_size+1);
    if(temp==NULL)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    buf=temp;
    
    /* Move text after #include line. */
    memmove(buf+position+header_size,buf+position+include_size,old_size-position-include_size+1);
    
    /* Insert header contents. */
    memcpy(buf+position,header_buf,header_size);
    return buf;
}

char *process_includes(char *buf, const char *filename)
{
    char *p;
    char *line_start;
    char *line_end;
    char *q;
    char *header_buf;

    char header[50];
    char directory[200];
    char path[300];

    int i;
    p = buf;
    while((p=strstr(p,"#include"))!=NULL)
    {
        /* Find beginning of line.*/
        line_start=p;
        while(line_start>buf && *(line_start-1)!='\n')
        {
            line_start--;
        }
        q = line_start;
        while(*q==' ' || *q=='\t')
        {
            q++;
        }
        if(q!=p)
        {
            p+=8;
            continue;
        }
        p += 8;
        while(*p==' ' || *p=='\t')
        {
            p++;
        }
        if(*p=='"')
        {
            p++;
            i=0;
            while(*p!='"' && *p!='\0')
            {
                if(i<sizeof(header)-1)
                {
                    header[i++]=*p;
                }
                p++;
            }
            header[i]='\0';
            if (*p!='"')
            {
                printf("Error: invalid #include\n");
                return buf;
            }
            p++;
            get_directory(filename, directory);
            snprintf(path,sizeof(path),"%s/%s",directory,header);
            header_buf=read_header(path);
            if(header_buf==NULL)
            {
                return buf;
            }
        }
        else if(*p=='<')
        {
            p++;
            i=0;
            while(*p!='>' && *p!='\0')
            {
                if(i<sizeof(header)-1)
                {
                    header[i++]=*p;
                }
                p++;
            }
            header[i]='\0';
            if(*p!='>')
            {
                printf("Error: invalid #include\n");
                return buf;
            }
            p++;
            snprintf(path,sizeof(path),"%s%s",SYSTEM_INCLUDE_PATH,header);
            header_buf=read_header(path);
            if(header_buf==NULL)
            {
                return buf;
            }
        }
        else
        {
            printf("Invalid #include\n");
            return buf;
        }
        line_end=strchr(p,'\n');
        if(line_end==NULL)
        {
            line_end=p+strlen(p);
        }
        buf=replace_include(buf,line_start,line_end,header_buf);
        free(header_buf);
        p = buf;
    }
    return buf;
}

