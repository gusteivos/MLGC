
#ifndef _MLGC_MAIN_H_
#define _MLGC_MAIN_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>

    #include "ascii.h"
    #include "file_util.h"
    #include "lexer.h"
    #include "location.h"
    #include "util.h"


    #ifndef NAME
    #define NAME "mlgc"
    #endif

    #define MIN_ARG_C 2


    void shift_argc_argv(int *argc, char **argv[]);

 /* int main(int argc, char *argv[]); */

    void usage(char *name, bool show_help, int exit_status);

    #ifndef NDEBUG
   
    int debug_main(int argc, char *argv[]);
   
    #endif

#endif /* _MLGC_MAIN_H_ */
