//main.c 
#include<stdio.h> 
#include"../include/preprocessor.h"
int main(int argc,char *argv[]) 
{     
    if(argc!=2)     
    {         
        printf("Invalid inputs\n");         
        printf("Usage: mini_cpp <source_file>\n");         
        return 1;     
    }  
    preprocess_file(argv[1]);   
}
