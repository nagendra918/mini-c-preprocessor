//preprocessor.c
#include<stdio.h>
#include<stdlib.h>
#include"../include/preprocessor.h"
#include"../include/comment.h"
#include"../include/macro.h"
#include"../include/include.h"
#include "../include/conditional.h"

void preprocess_file(const char *filename)
{
    char *buf;
    buf = read_from_file(filename);
    remove_comments(buf);
    buf=process_includes(buf, filename);
    buf=process_conditionals(buf);
    buf=process_defines(buf);
    printf("%s\n", buf);
    free(buf);
}
char *read_from_file(const char *filename)
{
    FILE *fp;
    long int size;
    char *buf;
    fp=fopen(filename,"rb");  
    if(fp==NULL)     
    {         
        printf("Error: cannot open %s\n",filename);  
        exit(EXIT_FAILURE);  
    } 
    fseek(fp,0,SEEK_END);
    size=ftell(fp);
    rewind(fp);
    buf=malloc(size+1);
    if(buf==NULL)
    {
        printf("Memory allocation failed\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    if(size>0)
    {
        if(fread(buf,size,1,fp)!=1)
        {
            printf("Failed to read from %s\n",filename);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    buf[size]='\0';
    fclose(fp);
    return buf;
}
