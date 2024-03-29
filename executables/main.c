#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

/** @file
 *  \defgroup executable_grp executable in text console
 * 
 *  Main executable for running JMMjouer in a text console.
 *   
 * This executable initializes the different modules
 * and call arcade main loop
*/

/* available system-wide only with gnu / glibc, found one implementation for windows */
#if defined(_WIN32) && defined(_MSC_VER)
    #include "executables/getopt.h"
#else /* LINUX or gcc mingw */
    #include <getopt.h>
#endif

#include "jmmjouer/arcade_params.h"
#include "jmmjouer/arcade.h"
/* storage of available games + in charge of loading dll (composition)
   injected in arcade (plugin manager is singleton) */
#include "jmmjouer/game_loader/game_loader.h"
#include "jmmjouer/game_loader/plugin_manager.h"

#include "record/irecord.h"
#include "record/file_record.h"

#include "clogger/clogger.h"
#include "jmmjouer/utils.h"

/** @{ @ingroup executable_grp */

/** \name for use with getopt.h */
/** \{ */
/* https://opensource.com/article/19/5/how-write-good-c-main-function */
/** list of options */
#define OPTSTR "vahd:"
/** usage line */ 
#define USAGE_FMT  "%s [-v] [-a anonym] [-d plugin_directory] [-h]"
/** default progname */
#define DEFAULT_PROGNAME JMMjouer
/** default directory for plugins */
static const char* const DEFAULT_PLUGINS_DIR = "plugins";
/* extern int errno;*/ /*dllimport attribute needed (windows spacific ?)*/
/** store arguments */
extern char *optarg;
/** current parsed indice */
extern int  optind;
 /** not used here, for error */
extern int opterr;

/** Structure build from the command line argument.
 * Passed as arugument to arcade */
typedef struct {
  /** mode verbose */
  bool           verbose;
  /** run games as  anonymous player, skip identification */
  int            flags_anomymous;
  /** to override default directory search path for plugins */
  char           *plg_dir;
} options_t;
/** \} */


/** set_terminate equivalent, to make better */
void clear_ressources_and_exit();
/** read command-line arguments */
int treat_options(options_t *options, arcade_params_t *arcade_opt);
/** print info usage */
void usage(char *progname, bool opt_verbose);
/** same, but more verbose */
void print_verbose(const options_t* opt_cmd, const arcade_params_t* opt_arcade);
/** Print preprocessor directives for achitecture.
 *  Usefull for debug preprocessor stage */
void log_directives();

/** Main executable. */
int main(int argc, char *argv[])
{
    /**********
     * Declaration of local variables 
     * *********/
    game_loader_t *game_loader; /* loading game (incl. plugins)*/
    file_record_t *record;      /* storage of joueur */
    /* options of the game to be passed to arcade */
    arcade_params_t arcade_options = { false };
    int retour;

    /* optarg() variables */
    int opt;
    /*int opterr = 0;*/
    options_t options = { false, 0, NULL };

    /* initialize the logger */
    init_clogger("jmmjouer.log");

    /* default value, cannot be in initilization, not compiled time */
    options.plg_dir = (char *)DEFAULT_PLUGINS_DIR;

    while ((opt = getopt(argc, argv, OPTSTR)) != EOF) {

       switch(opt) {
           case 'a':
            options.flags_anomymous = true;
            break;
           case 'v': 
            options.verbose = 1;
            break;
           case 'd':
            options.plg_dir = (char *) malloc( strlen(optarg) + 1);
            strcpy(options.plg_dir, optarg );
            break;
           case 'h':
           default : 
            usage("JMMjouer", options.verbose);
       }
    }

    /* further validation, fill structure for arcade / session */
    if( treat_options(&options, &arcade_options) != EXIT_SUCCESS) {
        /*perror(ERR_INVALID_OPTIONS);*/
        exit(EXIT_FAILURE);
    }

    /* would be nice to be able to choose console / log file */
    if( options.verbose )
        print_verbose( &options, &arcade_options);
    
    printf("\n\nJMMjouer vous accueille a son salon d'arcade\n");
    /* warning: universal character names are only valid in C++ and C99 */
    /* printf("Un choix de jeux un peu ..\U00001F47E.. de réflexion \U0001F9D0 \n\n"); */

    /* initalize pour random on peut l'appeler d'ici pour tous les jeux */
    /* for testing, full control random will be usefull */
    srand( (unsigned int) time(NULL)); /* cast for msvc */
    
    /***
     * Initilaization of the system of game laoding with compiled and plugins dll 
     ***/
    /* singleton plugin manager, set the direcotry */
    plugin_manager_set_directory( plugin_manager_get_instance(), options.plg_dir);
    game_loader = game_loader_new();
    game_loader_init( game_loader );
    retour = game_loader_load_all( game_loader );
    if( retour) {
        printf("Error in loading games\n");
        exit(1);
    }

    /* clear memory before continuing, to be sure it is copied inside plugin manager 
     clear options in fact 
     No clear only variables for plugin_manger initialisation */
    if( options.plg_dir != DEFAULT_PLUGINS_DIR ) {
        printf("deallocate plugin directory allocated on heap\n");
        free(options.plg_dir);
        options.plg_dir = NULL;
    } else {
        printf("No deallocation of default plugin directory\n");
    }
    
    /* allocate a concrete record implementation */
    record = file_record_new();
    file_record_init(record, "record_file.txt");

    /* base interface storage_description/info: filename/txt or db/port...*/
    printf("First interface usage: %s", record_get_storage_info( (irecord_t*)record ));

    /* *********
    *  Run Arcade 
    * **********/
    /* init core logic with inejection : game_loader and irecord */
    retour = arcade_init( game_loader, (irecord_t*)record);
    /* error, normal ending .. for return value */
    retour = arcade_run( &arcade_options );


    /* ********
    *  Clear ressources
    * *********/
    /* not a bad idea, but do nothing actually */
    /*arcade_clear() / */

    /* to pass to clear_ressources */
    file_record_delete( record );
    free( record ); /* need delete / free, or only free with interface/Inversion */
    record = NULL;
    
    game_loader_free( game_loader );
    game_loader = NULL;

    /* unload games and deallocation of memory on heap */
    clear_ressources_and_exit(); /* historique );*/
    return 0;
}

/*  see set_terminate() / signals */
void clear_ressources_and_exit() 
{
    int status;
    /* clear / delete singleton instances */
    plugin_manager_free();
    status = close_clogger();
    /*exit(0); failure should be much generic, error at different level */
    if( !status )
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

/* further validation of entries : check status file,...
   could fill structures/va */
int treat_options(options_t *options, arcade_params_t *arcade_opt)
{
    arcade_opt->play_anonymous = options->flags_anomymous ? true : false;
    return EXIT_SUCCESS;
}

/*
int serie_3_game(Joueur joueur, Historique* historique)
{
    start_game_pendu(joueur, historique);
    start_game_mastermind(joueur, historique);
    lancer_morpion(joueur, historique);
    return 0;
}
*/

void print_verbose(const options_t* opt_cmd, const arcade_params_t* opt_arcade) 
{        
        printf("\n---------------------------------\n");
        log_directives();
        printf("\n---Cmd Line options --------------\n");
        printf("mode verbose: %d\n", opt_cmd->verbose);
        printf("anonymous: %d\n", opt_cmd->flags_anomymous);
        printf("plugins directory : %s\n", opt_cmd->plg_dir);
        
        printf("\n --- options du jeu ---------------\n");
        printf("play anonymous: %s\n", opt_arcade->play_anonymous ? "vrai" : "faux");
}

void log_directives()
{
    printf("LOG directives\n");

/* only if windows.h is incuded */
#ifdef _WINDOWS_
	printf("compile on _WINDOWS_ \n");
#endif
/* defined by compiler, safer. WIN32 and UNIX to test also (see stackoverflow, not sure)
   It is about the platform, not the compiler, gcc on mingw64 is 1 */
#ifdef _WIN32
	printf("compile pour OS Windows\n");
    #ifdef _WIN64
        printf("Architecture 64 Bits\n");
    #else
        printf("Architecture 32 bits\n");
    #endif
#endif
/* VER of MSVC see _MSS_VER_LONG..
   MSVC compiler
   _DEBUG Defined as 1 when the /LDd, /MDd, or /MTd compiler option is set. Otherwise, undefined.
   show release mode otherwise, even with /DEBUG option of cl.exe / link.exe */
#ifdef _MSC_VER
	printf("compile avec MSVC compiler\n");
    #if defined(_DEBUG)
        printf("MSVC in debug mode\n");
    #else 
        printf("MSVC in release mode\n");
    #endif
#elif defined(__GNUC__) || defined(__GNUG__)
    printf("compile with GCC compiler\n");
    #ifdef NDEBUG
        printf("GCC in release mode\n");
    #else
        printf("GCC in debug mode\n");
    #endif
#endif

#ifdef JMMJ_DEBUG
    printf("DIRECTIVE JMMJ_DEBUG defined\n");
#endif
}

void usage(char *progname, bool opt_verbose)
{
    printf("Usage: opt %d\n", opt_verbose);
    printf(USAGE_FMT, progname);
    printf("\n");

    if( opt_verbose) {
        printf("\nMore details in verbose ...\n");
    }
    exit(EXIT_FAILURE);
}

/** @} */ /* end executable grp */
