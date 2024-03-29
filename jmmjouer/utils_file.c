
#include "utils_file.h"

#if defined(_WIN32)
    #include <windows.h>
#else /* posix compliant */
    #include <dirent.h>
#endif

#include <stdio.h>
#include <string.h>

#include "clogger/clogger.h"

/* need windows API header 
    https://learn.microsoft.com/en-us/windows/win32/fileio/listing-the-files-in-a-directory
    https://stackoverflow.com/questions/41404711/how-to-list-files-in-a-directory-using-the-windows-api
*/

/* case windows */
#if defined(_WIN32)
int for_files_with_extension( const char* directory, const char* extension, 
                              int (*p_fct)(clist_gen_t *, const char* ), clist_gen_t *clist_to_fill )
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    /*TCHAR szDir[MAX_PATH];*/ /* windows define MAX_CHAR 260 */
    LARGE_INTEGER filesize;
    char * dir_path;
    int retour;
    bool found_file;

    dir_path = malloc( strlen(directory) + 4);
    strcpy( dir_path, directory);
    strcat(dir_path,"\\*");

    hFind = FindFirstFile(dir_path, &ffd);
    if( hFind == INVALID_HANDLE_VALUE ) {
        CLOG_ERR("Directory not found or inaccessible %s / %s ",dir_path, extension);
        return 1;
    }

    do {
        filesize.LowPart = ffd.nFileSizeLow;
        filesize.HighPart = ffd.nFileSizeHigh;
        /* printf("  %s   %ld bytes\n", ffd.cFileName, (long)filesize.QuadPart); */
        CLOG_DEBUG("Load file: %s, size: %ld bytes\n", ffd.cFileName, (long)filesize.QuadPart);

        /* check extension, then load it with ptr_f on load_game_dll */
        found_file = check_extension( ffd.cFileName, extension );
        if( found_file ) {
           CLOG_DEBUG("found library %s\n", ffd.cFileName);
           retour = p_fct( clist_to_fill, ffd.cFileName );
           if( retour ) {
                CLOG_ERR("Error in calling pointer function %d", retour);
           }
        } 

    } while ( FindNextFile(hFind, &ffd) != 0 );

    /* deallocation local variable */
    free(dir_path);

    return 0;
}
/* case linux */
#else
/* alternative scandir allow filtering, sorting
   https://lloydrochester.com/post/c/list-directory/#:~:text=Use%20the%20glibc%20scandir%20function,of%20files%20in%20the%20directory. */
int for_files_with_extension( const char* directory, const char* extension, 
                              int (*p_fct)(clist_gen_t *, const char* ), clist_gen_t *clist_to_fill )
{
    DIR *d;
    struct dirent *dir;

    char * dir_path;
    int retour;
    bool found_file;

    dir_path = malloc( strlen(directory) + 1);
    strcpy( dir_path, directory);

    d = opendir(dir_path);
    if( !d ) {
        CLOG_ERR("Cannot open directory %s\n", dir_path); /* return -1 */
    }
   
   while( (dir = readdir(d)) != NULL ) {
        /*printf("Load file: %s\n", dir->d_name);*/
        /* check it is a file (no symlink, directory...)
        if (dir->d_type == DT_REG)
        {
            printf("%s\n", dir->d_name);
        }
        */
        CLOG_DEBUG("Load file: %s\n", dir->d_name);
        /*CLOG_DEBUG("Load file: %s, size: %ld bytes\n", ffd.cFileName, (long)filesize.QuadPart);*/

        /* check extension, then load it with ptr_f on load_game_dll */
        found_file = check_extension( dir->d_name, extension );
        if( found_file ) {
           CLOG_DEBUG("found library %s\n", dir->d_name);
           retour = p_fct( clist_to_fill, dir->d_name );
           if( retour ) {
                CLOG_ERR("Error in calling pointer function %d", retour);
           }
        }
    }
    closedir(d);
    /* deallocation local variable*/
    free(dir_path);
    return 0;
}
#endif

/* keep as general callback exemple, not used */
/*
int for_files_with_extension_callback( const char* directory, const char* extension, int (*p_fct)(const char* ) )
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    //TCHAR szDir[MAX_PATH]; windows define MAX_CHAR 260
    LARGE_INTEGER filesize;
    char * dir_path;
    int retour;
    bool found_file;

    dir_path = malloc( strlen(directory) + 4);
    strcpy( dir_path, directory);
    strcat(dir_path,"\\*");

    hFind = FindFirstFile(dir_path, &ffd);
    if( hFind == INVALID_HANDLE_VALUE ) {
        CLOG_ERR("Directory not found or inaccessible %s / %s ",dir_path, extension);
        return 0;
    }

    do {
        filesize.LowPart = ffd.nFileSizeLow;
        filesize.HighPart = ffd.nFileSizeHigh;
        printf("  %s   %ld bytes\n", ffd.cFileName, (long)filesize.QuadPart);

        // check extension, then load it with ptr_f on load_game_dll
        found_file = check_extension( ffd.cFileName, extension );
        printf("found_file %d\n", found_file);
        if( found_file ) {
           retour = p_fct( ffd.cFileName );
           printf("retour from funct. call %d\n", retour);
        } 

    } while ( FindNextFile(hFind, &ffd) != 0 );

    return 0;
}
*/

bool check_extension( const char* name, const char* extension )
{
    /* strrchr find last occurence, cstspn also possible */
    const char *pdot = strrchr( name, '.');
    if( !pdot )
        return false;
        
    if ( !strcmp( pdot+1, extension) )
        return true;

    return false;
}