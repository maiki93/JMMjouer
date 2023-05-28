#include "person.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "clogger/clogger.h"

/* one idea also possible , if immutability to enforce */
/*static const person_t person_invalid = ....*/

/* ** helpers ** */
static person_status_t make_invalid_person(person_t *person);

person_status_t person_init(person_t * person, const char * name, 
                bool is_daltonien, bool is_admin )
{
    char *pbuffer;

    CLOG_DEBUG("init with name %s", name);
    if( strlen( name ) > MAX_SIZE_NOM_PERSON - 1) {
        CLOG_ERR("NAME too long, max is %d \n", MAX_SIZE_NOM_PERSON);
        /*make_invalid_person(person);*/
        return make_invalid_person(person);
    }

    pbuffer = (char*) malloc( strlen(name) + 1);
    if( !pbuffer )
        return -1;

    strcpy( pbuffer, name);
    /* swap */
    person->pname = pbuffer;
    person->is_daltonien = is_daltonien;
    /* is admin return */
    person->is_admin = is_admin;
    return is_admin == true ? PERSON_ADMIN : PERSON_VALID;
}
/* person_status_t */
int person_default_init(person_t *person)
{   
    make_invalid_person(person);
    return PERSON_INVALID;
}
/* or make invalid state, pname NULL => invalid */
void person_delete(person_t *person)
{
    if( !person )
        return;
    if( person->pname)
        free( person->pname );
    person->pname = NULL;
    person->is_daltonien = false;
    person->is_admin = false;
}

/* ******** Accessors **********  */
person_status_t person_status(const person_t *person) 
{
    return person->pname == NULL ? PERSON_INVALID 
                                 : person->is_admin == false ? PERSON_VALID
                                                             : PERSON_ADMIN;
}

const char* person_name(const person_t *person)
{
    return person->pname;
}

bool person_daltonien(const person_t *person)
{
    return person->is_daltonien;
}

bool person_admin(const person_t *person)
{
    return person->is_admin;
}

void person_info(const person_t *person)
{
    printf("pseudo: %s\n", person->pname);
    printf("daltonien: %d\n", person->is_daltonien);
    printf("admin: %d\n", person->is_admin);
}

/*
void person_set_admin(person_t *person)
{
    person->is_admin = true;
}
*/

/* **** Helper ***** */
person_status_t make_invalid_person(person_t *person)
{
    person->pname = NULL;
    person->is_daltonien = false;
    person->is_admin = false;
    return PERSON_INVALID;
}