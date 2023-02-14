#ifndef CLIST_CSTRING_H
#define CLIST_CSTRING_H

#include "clist_generic.h"

#ifdef __cplusplus
extern "C" {
#endif

/* @brief List of C-string with a list-vector interface.
    Implemented with an internal clist_generic, a forward-list.
    Make interface convenient for C-strings

    Common error messages are the same than defined in clist_generic.h
*/

typedef struct {
    /* like cmap, clist_gen_t clist; no access generic type in .h 
     may contain the size at this level, rather than clist_generic (cmap also..) */
    clist_gen_t *clist;
} clist_cstring_t;

/* @brief A unique simple interface to help deletion of 
        array of structure return by get_array methods.
    Otherwise typedef / struct overall complex and increase dependencies */
void carray_cstring_delete( char **carray, size_t len);

/** Constructor/ Destructor */
clist_cstring_t* clist_cstring_new();
void clist_cstring_init(clist_cstring_t* list);
/* a default deleter would be enough in this case */
void clist_cstring_clear(clist_cstring_t* list);
void clist_cstring_delete(clist_cstring_t* list);

/****** Public methods ******/
size_t clist_cstring_size(clist_cstring_t *list);

/* @brief Insert a (copy of the) cstring at the end of the list.
    The null terminaison caracter is present in value_t::data 
        to allow the retrieval by reference */
int clist_cstring_push_back(clist_cstring_t *list, const char *to_insert);

/* @brief Get a copy of elem_nb-th element.
    Copy allocated on heap, must be freed by caller.
    \param[in] element number of the collection
    \param[inout] adress of the destination string, will contain the terminaison caracter on exit
    \return CLIST_OK or error from clist_genric.h */
int clist_cstring_get_copy( clist_cstring_t *list, size_t elem_nb, char **string_out);

/* boring to use, for printf for instance, it is a ref so return by pointer is enought */
/* @brief Get a reference to one element of the list.
    May be very tricky to modify the original throught the reference (size problems) !!
    \param[in] element number of the collection
    \param[inout] adress of the destination string, will contain the terminaison caracter on exit
    \return CLIST_OK or error from clist_genric.h */
int clist_cstring_get_ref( clist_cstring_t *list, size_t elem_nb, char **string_out);

/* @brief Return a array of string, deep copies of the internal elements.
    \param[out] heap allocated array_out with length array_len. must be deleted by caller
        (see carray_cstring_delete)
    \return CLIST_OK or error from clist_genric.h */
int clist_cstring_get_array( clist_cstring_t *list, char ***array_out, size_t *array_len );

/* An array of reference may be faster ? */

#ifdef __cplusplus
}
#endif

#endif /* CLIST_CSTRING_H */