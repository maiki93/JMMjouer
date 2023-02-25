#pragma once

/* NOT called anymore */
#include "irecord_private.h"

/* complete declaration of file_record_t */
typedef struct {
    irecord_t record_base; /*compose with the structure, allow polymorphism */
    char *filename; /* like irecord, cannot be const for free / swap style still good */
    FILE *fp;  /* really needed ? if want to keep the file opened between calls */
} file_record_t;
