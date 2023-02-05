#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "irecord_private.h"
#include "clogger.h"

#include "cmap_game_victories.h"

/* complete declaration of file_record_t */
typedef struct {
    irecord_t record_base; /*compose with the structure, allow polymorphism */
    char *filename; /* like irecord, cannot be const for free / swap style still good */
    FILE *fp;  /* really needed ? if want to keep the file opened between calls */
} file_record_t;

enum  { MAX_LINE_SIZE = 50 };

/* warning: useless storage class specifier in empty declaration */
/*static*/ enum { RECORD_MAX_LINE_SIZE = 50 };

/** private implementation to override base class methods **/
joueur_t __get_joueur_from_name( void *this, const char* name);
/* to make ovrride of this metod, public API */
size_t count_records(file_record_t *this);

/** private functions used only internally */
static int open_file( file_record_t *this, bool mode_write );
static void close_file( file_record_t *this );
static bool search_record_joueur_by_name(file_record_t *this, char *line, const char* name);
/* from char* to joueur & victories */
static int extract_name_joueur(const char *line, char *name_joueur);
static int extract_one_record(file_record_t *this, char *line, joueur_t *joueur);
int extract_map_victories( file_record_t *this, char *line, cmap_game_victories_t *map);

/** utilities function **/
static bool next_record(file_record_t *this, char *line);
static bool match_name( const char* line, const char *name);
/* test with fp style */
static int increment( void* input, void* output );
static int for_each_record( file_record_t *this, int(*pf)(void*, void*), void* input, void* output );

file_record_t* file_record_new()
{
    file_record_t *this = (file_record_t*) malloc( sizeof(file_record_t) );
    return this;
}

void file_record_init( file_record_t *this, const char *file_txt_name) 
{
    char tmp_description[100];  /* irecord will make a copy */
    char *tmp_filename;
    int byte;

    CLOG_DEBUG("Constructor filename %s\n", file_txt_name);
    /*create a specific description */
    byte = sprintf( tmp_description, 
                    "Record: a simple text file\nfilename %s\n", 
                    file_txt_name);
    assert( byte <= 100 );
    /* base class init */
    irecord_init( (irecord_t *)this, tmp_description );
    /* data member init */
    tmp_filename = (char*)malloc( strlen(file_txt_name) + 1);
    strcpy( tmp_filename, file_txt_name );
    this->filename = tmp_filename;
    this->fp = NULL;
    /* v-table equivalent */
    this->record_base.get_joueur_from_name = __get_joueur_from_name;
}

void file_record_delete( file_record_t *this)
{
    CLOG_DEBUG("Destructor filename %s\n", this->filename);
    if( this->filename )
        free( this->filename );
    this->filename = NULL;
    /* assert fp is closed, or check and call fclose from here also */
    this->fp = NULL;
    /* more "polymorphic" */
    irecord_delete( (irecord_t*) this);
    /* free this here ? */
}

joueur_t __get_joueur_from_name( void *this, const char* name)
{
    /* create buffer for reading line, 
        passed to all methods to avoid reallocation */
    char buffer_line[MAX_LINE_SIZE];
    file_record_t *file_record = (file_record_t *)this;
    joueur_t joueur;
    bool found_joueur;

    assert(this!= NULL);
    assert(file_record != NULL);

    joueur_default_init( &joueur );
    
    if( open_file( file_record, false ) ) {
        /* fclose( file_record->fp ); */
        return joueur;
    }
    /* advance file pointer unitil a record match the name 
        may use predicate here ... or just empty name for all ? 
        may store fiile position also with fsetpos */
    found_joueur = search_record_joueur_by_name( file_record, buffer_line, name);

    if( !found_joueur ) {
        CLOG_DEBUG("did not find joueur in record %s\n", name);
    }
    else {
        CLOG_DEBUG("found joueur in record: %s\n", name);
        extract_one_record( this, buffer_line, &joueur );
    }
    /* return an int, so can check also */
    close_file( file_record );
    /*
    CLOG_DEBUG("Found joueur return: name %s\n", joueur.person.nom);
    */
    return joueur;
}

int open_file( file_record_t *this, bool mode_write )
{
    if( mode_write)
        this->fp = fopen( this->filename, "r+");
    else
        this->fp = fopen( this->filename, "r");

    if( this->fp == NULL ) {
        CLOG_ERR("Impossible d'ouvrir le fichier %s\n", this->filename );
        printf("Impossible d'ouvrir le fichier %s\n", this->filename );
        CLOG_ERR("Etat de fp %p\n", (void *)this->fp);
        return -1; /*RETURN_KO / FILE_ERROR */
    }
    return 0;
}

void close_file( file_record_t *this )
{
    /* errno set with this function */
    if( fclose( this->fp) != 0 ) {
        CLOG_ERR("Closing file: %s\n", this->filename);
    }
}

/* @brief Parcourt le fichier to find the record starting with name.
    return fpos_t in the file at the start of the record s*/
bool search_record_joueur_by_name(file_record_t *this, char *line, const char* name)
{
    char name_joueur[MAX_SIZE_NOM_PERSON];
    /*char name_joueur[110];*/
    /*char *name_joueur = (char*)calloc(MAX_SIZE_NOM_PERSON, sizeof(char));*/
    /* loop until line contains 'name: is_daltonien' */
    while( next_record(this, line) ) 
    {
        /* test line content, this->fp is fixed */
        if ( extract_name_joueur(line, name_joueur)
            && match_name( name_joueur, name) )
        {
            return true;
        }
    }

    return false;
}

int extract_name_joueur(const char *line, char *name_joueur)
{
    return sscanf( line, "%s", name_joueur );
    /* solve scanf dr_memroy problem 
       google reported as problem with chkstk with mingw64, gcc
       use suppress_scanf until more tests*/
    /*return 0;*/
    /*int dummy;
    return sscanf( line, "%s %d", name_joueur, &dummy );
    */
}

int extract_one_record(file_record_t *this, char *line, joueur_t *joueur)
{
    char name_record[MAX_SIZE_NOM_PERSON]; /*NAME_MAXSIZE];*/  /* contains name read in file */
    int is_daltonien = false;
    int retour;

    /*sscanf( line, "\n%s %d\n", &joueur->person.nom, &joueur->person.is_daltonien );*/
    retour = sscanf( line, "\n%s %d\n", name_record, &is_daltonien );
    /* return errno or nb correct assignement == 2 */
    strncpy_s( joueur->person.nom, MAX_SIZE_NOM_PERSON, name_record, MAX_SIZE_NOM_PERSON);
    joueur->person.is_daltonien = is_daltonien;


    /* test if already existing / allocated map_victories */
    if( joueur->map_victories.clist != NULL) {
        printf("deallocate the map before extraction of one");

    }
    /* read game victories */
    extract_map_victories( this, line, &joueur->map_victories );
    return retour;
}

int extract_map_victories( file_record_t *this, char *line, cmap_game_victories_t *map)
{
    /* format victories :
    mastermind: 2 2 */
    struct pair_game_victory_t pair_victory;
    char *delimiter = ":";
    size_t pos_delimiter;

    fgets(line, MAX_LINE_SIZE,  this->fp);

    while( line[0] != '+' )
    {
        /* works if mastermind : x x", space between name and semi-column */
        /* https://stackoverflow.com/questions/45832469/get-the-beginning-of-a-string-until-a-given-char */
        /*
        sscanf( line, "%s : %d %d", pair_victory.game_name
                                   , & pair_victory.victories.nb_win
                                   , & pair_victory.victories.nb_lost );
        */
       pos_delimiter = strcspn( line, delimiter);
       strncpy(pair_victory.game_name, line, pos_delimiter);
       pair_victory.game_name[pos_delimiter] = '\0';
       sscanf( line+pos_delimiter+1, "%d %d", & pair_victory.victories.nb_win
                                            , & pair_victory.victories.nb_lost);
        
        game_victories_insert( map, pair_victory );
        fgets(line, MAX_LINE_SIZE,  this->fp);
    }
    return 0;
}

/* place file pointer at the start of a record */
bool next_record(file_record_t *this, char *line) 
{
    bool found_record = false;

    while( fgets( line, MAX_LINE_SIZE, this->fp ) != NULL 
            && found_record == false ) 
    {
        if( line[0] == '+')
            /* one more readline befor exit to get the name */
            found_record = true;
    }
    return found_record;
}       

/* basic block, reusable */
bool match_name( const char* name_record, const char *name)
{
    /* string name is the first entry, can compare strng directly by theirs pointers */
    if ( (strncmp( name_record, name, strlen(name) ) == 0)
        && ( strlen(name_record) ==  strlen(name) ) ) 
    {
        return true;
    }
    /* no match */
    return false;
}


int for_each_record( file_record_t *this, int(*pf)(void*, void*), void* input, void* output )
{
    char line[MAX_LINE_SIZE];
    bool found = false;

    /* must return start of the file */
    while( fgets( line, MAX_LINE_SIZE, this->fp ) != NULL 
            && found == false )
    {
        /* reach a new joueur record on character '+', start on next line */
        if( line[0] == '+' ) {
            found = pf(input, output);
        }

    }
    return (int)found;
}
/* function test for reuse */
int increment( void* input, void* output )
{
    size_t *int_in = (size_t*)input;
    size_t *int_out = (size_t*)output;
    assert( int_out != NULL);
    /* *int_in = (*int_in)+1; */
    (*int_in)++;
    return 0;
}

size_t count_records(file_record_t *this)
{
    size_t counter = 0;
    size_t counter2 = 0;

    open_file( this, false );
    for_each_record( this, &increment, (void*) &counter, (void*) &counter2);
    fclose(this->fp);

    return counter;
}