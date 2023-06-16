#include "user.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "clogger/clogger.h"

/* one idea also possible , if immutability to enforce */
/*static const user_t user_invalid = ....*/

const char* ANONYMOUS_NAME = "anonymous"; /* to avoid multiple definition at link stage */

/* ** helpers ** */
static user_status_t make_invalid_user(user_t *user);

user_t make_user(size_t id, const char * name, 
                     bool is_daltonien, bool is_admin, user_status_t *status)
{
    char *pbuffer;

    user_t new_user;

    pbuffer = (char*) malloc( strlen(name) + 1);
    if( !pbuffer ) {
        make_invalid_user(&new_user);
        *status = USER_INVALID;
        return new_user;
    }
    strncpy(pbuffer, name, MAX_SIZE_NAME_USER);
    new_user.pname = pbuffer;

    new_user.user_id = id;
    new_user.is_daltonien = is_daltonien;
    new_user.is_admin = is_admin;
    *status = USER_VALID;
    return new_user;
}

user_status_t user_init(user_t * user, size_t id, const char *name, 
                        bool is_daltonien, bool is_admin )
{
    char *pbuffer;

    CLOG_DEBUG("init with name %s\n", name);
    if( strlen( name ) > MAX_SIZE_NAME_USER - 1) {
        CLOG_ERR("NAME too long, max is %d \n", MAX_SIZE_NAME_USER);
        /*make_invalid_user(user);*/
        return make_invalid_user(user);
    }

    pbuffer = (char*) malloc( strlen(name) + 1);
    if( !pbuffer )
        return -1;

    strcpy( pbuffer, name);
    /* swap */
    user->pname = pbuffer;
    user->user_id = id;
    user->is_daltonien = is_daltonien;
    /* is admin return */
    user->is_admin = is_admin;
    /*return is_admin == true ? USER_ADMIN : USER_VALID;*/
    return USER_VALID;
}
/* user_status_t */
int user_default_init(user_t *user)
{   
    make_invalid_user(user);
    return USER_INVALID;
}
/* or make invalid state, pname NULL => invalid */
void user_delete(user_t *user)
{
    if( !user )
        return;
    if( user->pname)
        free( user->pname );
    user->user_id = -1;
    user->pname = NULL;
    user->is_daltonien = false;
    user->is_admin = false;
}

/* ******** Accessors **********  */
user_status_t user_status(const user_t *user) 
{
    return (user->user_id == 0 || user->pname == NULL) ? USER_INVALID
                                                        : USER_VALID;
    /* return user->user_id == -1 ? USER_INVALID
                                 : USER_VALID; */
    /*
                                 : user->is_admin == false ? USER_VALID
                                                             : USER_ADMIN;
    */
}

bool user_valid(const user_t *user)
{
    return ( user->user_id == 0 || user->pname == NULL) ? false
                                                         : true;
}

size_t user_id(const user_t *user)
{
    return user->user_id;
}

const char* user_name(const user_t *user)
{
    return user->pname;
}

bool user_daltonien(const user_t *user)
{
    return user->is_daltonien;
}

bool user_admin(const user_t *user)
{
    return user->is_admin;
}

void user_info(const user_t *user)
{
    printf("pseudo: %s\n", user->pname);
    printf("daltonien: %d\n", user->is_daltonien);
    printf("admin: %d\n", user->is_admin);
#ifdef JMMJ_DEBUG
    printf("id: %ld\n", user->user_id);
#endif
}

/* **** Helper ***** */
user_status_t make_invalid_user(user_t *user)
{
    user->user_id = 0;
    user->pname = NULL;
    user->is_daltonien = false;
    user->is_admin = false;
    return USER_INVALID;
}