/* encoding: UTF-8, linebreak: linux, indet-mode: spaces, tabsize: 4 */
/* Written by Christoph Bischko */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argtable2.h>
#ifndef SC_NOTIME
    #include <time.h>
#endif
#if !defined(SC_NOHOST) && defined(HAVE_UNISTD_H)
    #include <unistd.h>
#endif

#include "argtab2_additions.h"

#define ARGAD_FLAG_LENGTH 3 /* Bits already used in the flag (these can't store the type information) */

#define STRINGIFY(x) #x
#define STR(x)  STRINGIFY(x)

int argad_get_argument_type(void * arg){
    struct arg_hdr * header = (struct arg_hdr*) arg;
    return header->flag >> ARGAD_FLAG_LENGTH;
}


/* print user arguments */
void argad_fprint_arguments(FILE* fd, void** argtable) {
    int tabindex, i;                                /* iterators */
    int argad_type;                                 /* type of the current argument */
    const char *shortopts, *longopts, *datatype;    /* members describing the current argument */
    void *parent;                                   /* pointer the current argument (member of it's the argument header) */
    struct arg_hdr **table = (struct arg_hdr**)argtable; /* table containing the arguments */
    fprintf(fd, "# Command line arguments: \n");

    // LOOP OVER ARGUMENTS
    for(tabindex=0; !(table[tabindex]->flag&ARG_TERMINATOR); tabindex++) {
        // GET RELEVANT PROPERTIES
        argad_type= table[tabindex]->flag >> ARGAD_FLAG_LENGTH;
        shortopts = table[tabindex]->shortopts;
        longopts  = table[tabindex]->longopts;
        datatype  = table[tabindex]->datatype;
        parent    = table[tabindex]->parent;
        if (longopts!=NULL) longopts=strndup(longopts, strcspn(longopts,",") ); /* string up to first the comma (if not present whole string) */
        //if (datatype==NULL) continue; /* skip if argument is fishy (ToDo: maybe not needed anymore) */

        // DETECT TYPE AND DO THE APPROPRIATE THING
        if (        argad_type==ARGAD_LIT) {
            struct arg_lit *arg = (struct arg_lit*)parent;
            fprintf(fd,
                "# \t %-20s[%d] = %s (%d)\n",
                longopts, 0, shortopts, arg->count
            );
        } else if ( argad_type==ARGAD_INT) {
            struct arg_int *arg = (struct arg_int*)parent;
            fprintf(fd,  "# \t %-20s[%d] = %+e = %d (%d)\n",
                    longopts, 0, 1.0*arg->ival[0], arg->ival[0], arg->count );
            for(i=1; i<table[tabindex]->maxcount; ++i) {
                fprintf(fd,
                    "# \t %-20s[%d] = %+e = %d\n",
                    longopts, i, 1.0*arg->ival[i], arg->ival[i]
                );
            }
        } else if ( argad_type==ARGAD_DBL) {
            struct arg_dbl *arg = (struct arg_dbl*)parent;
            fprintf(fd,  "# \t %-20s[%d] = %+e (%d)\n",
                    longopts, 0, arg->dval[0], arg->count );
            for(i=1; i<table[tabindex]->maxcount; ++i) {
                fprintf(fd,
                    "# \t %-20s[%d] = %+e\n",
                    longopts, i, arg->dval[i]
                );
            }
        } else if ( argad_type==ARGAD_STR) {
            struct arg_str *arg = (struct arg_str*)parent;
            fprintf(fd,  "# \t %-20s[%d] = %s (%d)\n",
                    longopts, 0, arg->sval[0], arg->count );
            for(i=1; i<table[tabindex]->maxcount; ++i) {
                fprintf(fd,
                    "# \t %-20s[%d] = %s\n",
                    longopts, i, arg->sval[i]
                );
            }
        } else if ( argad_type==ARGAD_REX) {
            struct arg_rex *arg = (struct arg_rex*)parent;
            fprintf(fd,  "# \t %-20s[%d] = %s (%d)\n",
                    longopts, 0, arg->sval[0], arg->count );
            for(i=1; i<table[tabindex]->maxcount; ++i) {
                fprintf(fd,
                    "# \t %-20s[%d] = %s\n",
                    longopts, i, arg->sval[i]
                );
            }
        } else if ( argad_type==ARGAD_FILE) {
            struct arg_file *arg = (struct arg_file*)parent;
            fprintf(fd,  "# \t %-20s[%d] = %s (%d)\n",
                    datatype, 0, arg->filename[0], arg->count );
            for(i=1; i<table[tabindex]->maxcount; ++i) {
                fprintf(fd,
                    "# \t %-20s[%d] = %s\n",
                    datatype, i, arg->filename[i]
                );
            }
        } else if ( argad_type==ARGAD_DATE) { /* ToDo: Implement print date */
            //struct arg_date *arg = (struct arg_date*)parent;
            fprintf(fd, "Note: arg_date found but printing not jet implemented.\n");
        } else if (
                    argad_type==ARGAD_END ||
                    argad_type==ARGAD_REM
        ) {
            ; /* skip arg_end and arg_rem */
        }
/* start legacy for compatability with older versions. It's basically the same thing as before just using the (unsave) datatype to detect the argument type. ToDo: Function pointer instead (good idea?) */
        else {
            if( !strcmp(datatype,"<double>") ) {
                struct arg_dbl *arg = (struct arg_dbl*)parent;
                for(i=0; i<table[tabindex]->maxcount; ++i) {
                    fprintf(fd,
                        "# \t %-20s[%d] = %+e\n",
                        longopts, i, arg->dval[i]
                    );
                }
            } else if( !strcmp(datatype,"<int>") ) {
                struct arg_int *arg = (struct arg_int*)parent;
                for(i=0; i<table[tabindex]->maxcount; ++i) {
                    fprintf(fd,
                        "# \t %-20s[%d] = %+e\n",
                        longopts, i, 1.0*arg->ival[i]
                    );
                }
            } else if ( !strcmp(datatype,"INPUT-FILE") ) {
                struct arg_file *arg = (struct arg_file*)parent;
                for(i=0; i<table[tabindex]->maxcount; ++i) {
                    fprintf(fd,
                        "# \t %-20s[%d] = %s\n",
                        datatype, i, arg->filename[i]
                    );
                }
            } else {
                //struct arg_lit *arg = (struct arg_lit*)parent;
                for(i=0; i<table[tabindex]->maxcount; ++i) {
                    fprintf(fd,
                        "# \t %-20s[%d] = %s\n",
                        longopts, i, shortopts
                    );
                }
            }
        }
/* end legacy */
    }
}

void argad_fprint_shell_call(FILE* fd, int argc, char **argv) {
    // INFO ON THE CALL
#if defined REVISION || !defined SC_NOUSER || !defined SC_NOHOST || !defined SC_NOTIME
    fprintf(fd, "# ");
#endif
#ifdef REVISION
    fprintf(fd, "Revision: %s built " __DATE__ " " __TIME__ " | ", STR(REVISION));
#endif
#ifndef SC_NOUSER
    char *username = getenv("USER");
    if ( username != NULL ) {
        fprintf(fd, "%s", username);
    }
#endif
#if !defined(SC_NOHOST) && defined(HAVE_UNISTD_H)
    // Also getenv("HOSTNAME") could work
    char hostname[201]; hostname[200] = '\0';
    gethostname(hostname,200);
    fprintf(fd, "@%s", hostname);
#endif
#if !defined SC_NOUSER || !defined SC_NOHOST
    fprintf(fd," | ");
#endif
#ifndef SC_NOTIME
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(fd, "System time: %s", asctime(timeinfo) );
#endif
    fprintf(fd, "#");

    // PRINT THE CALL
    int i;
    for(i=0; i<argc; ++i) {
        fprintf(fd, "%s ",argv[i]);
    }
    fprintf(fd, "\n");
}

int argad_get_file_args(const int argc, char** argv) {
    int i;
    int argad_max_fileargs = argc-1; // first argument is the program name
    for(i=0; i<argc; ++i)
        if( argv[i][0] == '-' ) argad_max_fileargs--; // arguments starting with a '-' are options
    return argad_max_fileargs; // the rest could be a filename
}

void argad_handle_errors(const char *description, int argc, char **argv,
void **argtable) {
    int tabindex;   /* iterator */
    int argad_type; /* stores the current argument type */
    struct arg_hdr **table = (struct arg_hdr**)argtable; /* table containing the arguments */

    // SEARCH FOR AN ARG_END CONSTRUCT
    for(tabindex=0; 1; tabindex++) {
        argad_type = (table[tabindex]->flag) >> ARGAD_FLAG_LENGTH;
        if( argad_type == ARGAD_END ) { /* if found run the real function */
            argad_handle_arg_errors( description, argc, argv, argtable,
                (struct arg_end*)(table[tabindex]->parent)             );
            return;
        }
    }
    fprintf(stderr, "Error: No arg_end construct found in array of arguments."
        " Did you use the argad_end() constructor function?\n");
    exit(EXIT_FAILURE);
}

void argad_handle_arg_errors(const char* description, int argc,
char** argv, void** argtable, struct arg_end* end) {
    int tabindex;                   /* table iterator */
    struct arg_hdr **table;         /* table containing the arguments */
    const char *shortopts;          /* short options string for the current argument */
    struct arg_lit *argadHelp=NULL; /* stores the help argument */

    if ( arg_nullcheck(argtable) != 0 ) {
        /* NULL entries were detected, some allocations must have failed */
        fprintf(stderr,
            "%s: Insufficient memory. arg_nullcheck() failed.\n", argv[0]
        );
        exit(EXIT_FAILURE);
    }

    if ( arg_parse(argc, argv, argtable) > 0 ) {
        table = (struct arg_hdr**)argtable;

        // SEARCH FOR THE HELP ARGUMENT
        for(tabindex=0; !(table[tabindex]->flag&ARG_TERMINATOR); tabindex++) {
            shortopts = table[tabindex]->shortopts;
            if (shortopts==NULL) continue; /* skip if argument is fubar */
            if( !strcmp(shortopts,"h") ) {
                argadHelp = (struct arg_lit*)(table[tabindex]->parent);
                break;
            }
        }
        shortopts = table[tabindex]->shortopts;

        // PRINT HELP MESSAGE AND EXIT (if user specified the help argument)
        if ( argadHelp->count > 0 ) { /* --help takes precedence over error reporting */
            printf("Usage: %s ", argv[0]);
            printf("%s\n\nOPTIONS are:\n",description);
            arg_print_glossary_gnu(stdout,argtable);
            exit(EXIT_SUCCESS);
        }

        // PRINT PARSER ERRORS
        arg_print_errors(stderr, end, argv[0]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

// CONVENIANCE FUNCTIONS TO PRINT TO STDOUT
void argad_print_arguments(void** argtable) {
    argad_fprint_arguments(stdout, argtable);
}

void argad_print_shell_call(int argc, char **argv) {
    argad_fprint_shell_call(stdout, argc, argv);
}

// FROM HERE ON IT'S THE FLAG HACK
/* This utilizes the unused bits of the flag property in each argument's header. The constructor functions are redefined to encode the arguments' type in this flag. */


struct arg_end* argad_end(int maxerrors) {
    struct arg_end* ret;
    ret = arg_end(maxerrors);
    ret->hdr.flag = ret->hdr.flag | (ARGAD_END<<ARGAD_FLAG_LENGTH);
    return ret;
}

struct arg_rem* argad_rem(const char* datatype, const char* glossary) {
    struct arg_rem* ret;
    ret = arg_rem(datatype, glossary);
    ret->hdr.flag = ret->hdr.flag | (ARGAD_REM<<ARGAD_FLAG_LENGTH);
    return ret;
}

struct arg_rex* argad_rex0(const char* shortopts, const char* longopts,
const char* pattern, const char* datatype, int flags, const char* glossary) {
    return argad_rexn(shortopts, longopts, pattern, datatype, 0, 1, flags,
        glossary);
}

struct arg_rex* argad_rex1(const char* shortopts, const char* longopts,
const char* pattern, const char* datatype, int flags, const char *glossary) {
    return argad_rexn(shortopts, longopts, pattern, datatype, 1, 1, flags,
        glossary);
}

struct arg_rex* argad_rexn(const char* shortopts, const char* longopts,
const char* pattern, const char* datatype, int mincount, int maxcount,
int flags, const char *glossary) {
    struct arg_rex* ret;
    ret = arg_rexn(shortopts, longopts, pattern, datatype, mincount, maxcount,
        flags, glossary);
    ret->hdr.flag = ret->hdr.flag | (ARGAD_REX<<ARGAD_FLAG_LENGTH);
    return ret;
}

// marcro to repeat the above with slight variation
#define COMMA , // hack to pass an actual comma in preprocessor arguments
#define ARGAD_DECLARE(NAME,ARGS,SHORTARGS,FLAG) \
    struct arg_##NAME * argad_##NAME##0(const char* shortopts, const char* longopts, ARGS \
    const char* glossary) { \
        return argad_##NAME##n(shortopts, longopts, SHORTARGS 0, 1, glossary); \
    } \
    \
    struct arg_##NAME * argad_##NAME##1(const char* shortopts, const char* longopts, ARGS \
    const char *glossary) { \
        return argad_##NAME##n(shortopts, longopts, SHORTARGS 1, 1, glossary); \
    } \
    \
    struct arg_##NAME * argad_##NAME##n(const char* shortopts, const char* longopts, ARGS \
    int mincount, int maxcount, const char *glossary) { \
        struct arg_##NAME * ret; \
        ret = arg_##NAME##n(shortopts, longopts, SHORTARGS mincount, maxcount, glossary); \
        ret->hdr.flag = ret->hdr.flag | ((FLAG)<<ARGAD_FLAG_LENGTH); \
        return ret; \
    }


ARGAD_DECLARE(lit , , , ARGAD_LIT)
ARGAD_DECLARE(int , const char * datatype COMMA, datatype COMMA, ARGAD_INT)
ARGAD_DECLARE(dbl , const char * datatype COMMA, datatype COMMA, ARGAD_DBL)
ARGAD_DECLARE(str , const char * datatype COMMA, datatype COMMA, ARGAD_STR)
ARGAD_DECLARE(file, const char * datatype COMMA, datatype COMMA, ARGAD_FILE)
ARGAD_DECLARE(date, const char * format COMMA const char * datatype COMMA, format COMMA datatype COMMA, ARGAD_DATE)
