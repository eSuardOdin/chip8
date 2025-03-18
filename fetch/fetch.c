/*
 * Just trying to fetch instructions and display them
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    unsigned char buffer[2];
    FILE *fp;
    
    fp = fopen(argv[1], "rb");
    if(fp == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read
    for(int i = 0; i < 8; i++)
    {
        fread(buffer, sizeof(buffer), 1, fp);
        printf("%02X%02X ", buffer[1], buffer[0]);
    }
    printf("\n");
    exit(EXIT_SUCCESS);
}
