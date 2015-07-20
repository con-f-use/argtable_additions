/* encoding: UTF-8, linebreak: linux, indet-mode: spaces, tabsize: 4 */
/* Written by Christoph Bischko */

#ifndef ARGTAB2___ADDITIONS
#define ARGTAB2___ADDITIONS

#include <argtable2.h>

// This contains (amongst other things) a dirty flag hack to argtable2 that
//+introduces a way to discern argument types (str, dbl, int) just by the
//+void** argtable array containing the arguments.

/** Argument type identifiers. */
enum argtype {
    ARGAD_REM  = 1,
    ARGAD_LIT  = 2,
    ARGAD_INT  = 3,
    ARGAD_DBL  = 4,
    ARGAD_STR  = 5,
    ARGAD_REX  = 6,
    ARGAD_FILE = 7,
    ARGAD_DATE = 8,
    ARGAD_END  = 9
};

/** Returns an integer corresponding to the type of argument (double, string, filename etc.) that was passed. @see enum argtype  */
int argad_get_argument_type(
    void * arg
);

/** Prints all argtable arguments and their values to a given stream. */
void argad_fprint_arguments(
    FILE* fd,
    void** argtable
);
/** Prints all argtable arguments and their values to stdout. Convenience */
void argad_print_arguments(
    void** argtable
);

/** Prints all a timestamp as well as all parameters passed in argv to a given stream. */
void argad_fprint_shell_call(
    FILE* fd,
    int argc,
    char **argv
);
/** Prints a timestamp as well as all parameters passed in argv to stdout. Convenience */
void argad_print_shell_call(
    int argc,
    char **argv
);

/** Convenience function to get the maximal number of file arguments **/
int argad_get_file_args(
    const int argc,
    char** argv
);

/** Convenience function to check for argument errors. Legacy */
void argad_handle_arg_errors(
    const char *description,
    int argc,
    char **argv,
    void **argtable,
    struct arg_end* end
);

/** Prints error messages related to various errors in the user input */
void argad_handle_errors(
    const char *description,
    int argc,
    char **argv,
    void **argtable
);

// DEFINE NEW ARGUMENT CONSTRUCTORS (to hack argument types into the flag property)
struct arg_rem* argad_rem(const char* datatype, const char* glossary);

struct arg_lit* argad_lit0(const char* shortopts,
                         const char* longopts,
                         const char* glossary);
struct arg_lit* argad_lit1(const char* shortopts,
                         const char* longopts,
                         const char *glossary);
struct arg_lit* argad_litn(const char* shortopts,
                         const char* longopts,
                         int mincount,
                         int maxcount,
                         const char *glossary);

struct arg_key* argad_key0(const char* keyword,
                         int flags,
                         const char* glossary);
struct arg_key* argad_key1(const char* keyword,
                         int flags,
                         const char* glossary);
struct arg_key* argad_keyn(const char* keyword,
                         int flags,
                         int mincount,
                         int maxcount,
                         const char* glossary);

struct arg_int* argad_int0(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         const char* glossary);
struct arg_int* argad_int1(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         const char *glossary);
struct arg_int* argad_intn(const char* shortopts,
                         const char* longopts,
                         const char *datatype,
                         int mincount,
                         int maxcount,
                         const char *glossary);

struct arg_dbl* argad_dbl0(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         const char* glossary);
struct arg_dbl* argad_dbl1(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         const char *glossary);
struct arg_dbl* argad_dbln(const char* shortopts,
                         const char* longopts,
                         const char *datatype,
                         int mincount,
                         int maxcount,
                         const char *glossary);

struct arg_str* argad_str0(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         const char* glossary);
struct arg_str* argad_str1(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         const char *glossary);
struct arg_str* argad_strn(const char* shortopts,
                         const char* longopts,
                         const char* datatype,
                         int mincount,
                         int maxcount,
                         const char *glossary);

struct arg_rex* argad_rex0(const char* shortopts,
                         const char* longopts,
                         const char* pattern,
                         const char* datatype,
                         int flags,
                         const char* glossary);
struct arg_rex* argad_rex1(const char* shortopts,
                         const char* longopts,
                         const char* pattern,
                         const char* datatype,
                         int flags,
                         const char *glossary);
struct arg_rex* argad_rexn(const char* shortopts,
                         const char* longopts,
                         const char* pattern,
                         const char* datatype,
                         int mincount,
                         int maxcount,
                         int flags,
                         const char *glossary);

struct arg_file* argad_file0(const char* shortopts,
                           const char* longopts,
                           const char* datatype,
                           const char* glossary);
struct arg_file* argad_file1(const char* shortopts,
                           const char* longopts,
                           const char* datatype,
                           const char *glossary);
struct arg_file* argad_filen(const char* shortopts,
                           const char* longopts,
                           const char* datatype,
                           int mincount,
                           int maxcount,
                           const char *glossary);

struct arg_date* argad_date0(const char* shortopts,
                           const char* longopts,
                           const char* format,
                           const char* datatype,
                           const char* glossary);
struct arg_date* argad_date1(const char* shortopts,
                           const char* longopts,
                           const char* format,
                           const char* datatype,
                           const char *glossary);
struct arg_date* argad_daten(const char* shortopts,
                           const char* longopts,
                           const char* format,
                           const char* datatype,
                           int mincount,
                           int maxcount,
                           const char *glossary);

struct arg_end* argad_end(int maxerrors);

#endif
