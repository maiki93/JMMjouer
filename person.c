#include "person.h"

#include <stdio.h>
#include <string.h>
#include "clogger.h"

int person_init(person_t * person, const char * name, bool is_daltonien )
{
    CLOG_DEBUG("init with name %s", name);
    if( strlen( name ) > MAX_SIZE_NOM_PERSON - 1) {
        CLOG_ERR("NAME too long, max is %d \n", MAX_SIZE_NOM_PERSON);
        return PERSON_INVALID;
    }

    strcpy(person->nom, name);
    person->is_daltonien = is_daltonien;
    return PERSON_OK;
}

int person_default_init(person_t *person)
{   
    return person_init( person, "invalid", false);
}

bool person_is_valid(person_t *person) 
{
    return strcmp( person->nom, "invalid");
}

const char* person_name(person_t *person)
{
    return person->nom;
}

void person_info(person_t *person)
{
    printf("pseudo: %s\n", person->nom);
    printf("daltonien: %d\n", person->is_daltonien);
}