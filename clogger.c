

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h> /* for va_args */

#include "clogger.h"

/* static const char* filename;  to keep a copy ? */
static FILE *fp = NULL;

/* to avoid global intitialization, not sure static possible here in c89 */
FILE* getFp() 
{
    static FILE *fp;
    return fp;
}

int init_clogger(const char *filename) 
{
    char *local_filename = NULL;
    /* make a default */
    if( !filename ) {
        local_filename = "clogger.log";
    } else {
        local_filename = (char*) malloc( strlen(filename) );
        strcpy(local_filename, filename);
    }
    /* cannot log yet ! */
    fp = fopen( local_filename, "w");

    

    if (fp == NULL) {
        printf("Error in creating log file %s\n", local_filename);
        free( local_filename );
        return 1;
    }

    free( local_filename );
    return 0;
}

int close_clogger() 
{
    int retour;
    if (fp != NULL)
        retour = fclose(fp);

    if (retour) {
        printf("Error in closing filename");
        return 1;
    }
    return 0;
}

/** for dev to move in own implementation later */
void log_debug(const char *filename, long line, const char* format, ...) 
{
    va_list args;
    /*printf("%s:%ld :: ", filename, line);*/
    fprintf(fp, "%s:%ld :: ", filename, line);
    /* loop over the argument */
    va_start(args, format);
    /*vprintf(format, args);*/
    vfprintf(fp, format, args);
    va_end(args);
}