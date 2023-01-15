#ifndef CLIST_GENERIC_H
#define CLIST_GENERIC_H

#include "stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLIST_OK 0
#define CLIST_NOTFOUND -1
#define CLIST_OUTOFRANGE -2
#define CLIST_EMPTY -3
#define CLIST_ALLOC_ERR -4

/* value_t play the role of a generic structure, 
    can store any serializable type or data structure */
typedef struct {
    char *data;
    size_t len;
} value_t;

typedef struct clist_type clist_gen_t;

/* to provide by user of the class, used in destructor */
typedef void (*deleter_t) (value_t* value);
/*  to serailize value */

/* @brief Return a value_t from data inputs.
    value_t takes ownership of the data which should not be modified after theis call */
value_t make_value(char *data, size_t len);
/* @brief Create a duplicate. 
    \return a value_t independent of the inputs */
value_t copy_value(const char *data, size_t len);
/* @brief Deallocate memory */
void free_value(value_t* value);

/*** clist_generic public api : 
      constructor, destructor and behavior ***/
clist_gen_t* clist_gen_new();
void clist_gen_init(clist_gen_t* clist);
void clist_gen_del(clist_gen_t *clist, deleter_t deleter);
/* could be private, maybe not the same logic as well 
   the goal is to call deleter(&value)*/
void clist_gen_clear(clist_gen_t *clist, deleter_t deleter);
/* @brief Return the number of value_t elements in the list 
    Actual, store a len data member, may change later.
    Total size of the message (sum of len) may be usefull fir serialization (to think)*/
size_t clist_gen_size(const clist_gen_t *clist);

/* @brief Make a copy the input value at the first elementin the list*/
int clist_gen_push_front(clist_gen_t *clist, value_t value);
/* @brief Make a copy of the input value at the end of the list*/
int clist_gen_push_back(clist_gen_t *clist, const value_t *value_in);

/* interface and usage not very convenient ** if want to modify only, not safe as well 
   but no copy needed */
int clist_gen_get_value_ref( const clist_gen_t *clist, size_t elem_nb, value_t **value_out);
/* get a copy of the content , better adapted for use ? */
int clist_gen_get_value_copy( const clist_gen_t *clist, size_t elem_nb, value_t *value_out);

/* @brief Retrieve a COPY of the first element where the buffer match the internal value_t::data.

   TODO equivalent with reference. improve interface with usage.
   Most important function  for reuse and specialization, very generic at this level 
   It checks the first n bytes given in buffer (later comp function ?)
   \return EROR code */
int clist_gen_find( const clist_gen_t* clist, value_t* value_out, 
                    const char* buffer, size_t buffer_len);

#ifdef __cplusplus
}
#endif

#endif /* CLIST_GENERIC_H */