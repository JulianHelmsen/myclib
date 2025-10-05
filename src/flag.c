#include <flag.h>
#include <da.h>
#include <errno.h>
#include <string.h>
#include <macros.h>
#include <sview.h>

enum opt_kind {
    INT,
    STRING,
    BOOL
};

union option_value {
    int ival;
    sview sval;
    bool bval;
};

struct option {
    enum opt_kind kind;
    const char* name;
    const char* description;
    union option_value val;
};

struct options {
    size_t capacity;
    size_t size;
    struct option* items;
};

struct flag {
    char name;
    bool present;
    const char* description;
};

static const char* prog_name = NULL;
static struct option opts[100];
static size_t opt_count = 0;
static sview arguments[100];
static size_t arg_count = 0;
static struct flag flags[100];
static size_t flag_count = 0;

int* flag_opt_int(const char* name, int def_value, const char* descr) {
    if(opt_count >= ARRAY_LEN(opts)) {
        ERR("Too many options defined. Max = %zu\n", ARRAY_LEN(opts));
        abort();
    }
    struct option* opt = &opts[opt_count++];
    opt->name = name;
    opt->kind = INT;
    opt->description = descr;
    opt->val.ival = def_value;
    return &opt->val.ival;
}

bool* flag_opt_bool(const char* name, bool def_value, const char* descr) {
    if(opt_count >= ARRAY_LEN(opts)) {
        ERR("Too many options defined. Max = %zu\n", ARRAY_LEN(opts));
        abort();
    }
    struct option* opt = &opts[opt_count++];
    opt->name = name;
    opt->kind = BOOL;
    opt->description = descr;
    opt->val.bval = def_value;
    return &opt->val.bval;

}
sview* flag_opt_string(const char* name, const char* def_value, const char* descr) {
    if(opt_count >= ARRAY_LEN(opts)) {
        ERR("Too many options defined. Max = %zu\n", ARRAY_LEN(opts));
        abort();
    }

    struct option* opt = &opts[opt_count++];
    opt->name = name;
    opt->kind = STRING;
    opt->description = descr;
    opt->val.sval = sview_create_null_terminated(def_value);

    return &opt->val.sval;
}

bool* flag_exists(char name, const char* description) {
    if(flag_count >= ARRAY_LEN(flags)) {
        ERR("Too many flags defined. Max = %zu\n", ARRAY_LEN(flags));
        abort();
    }
    struct flag* flag = &flags[flag_count++];
    flag->name = name;
    flag->description = description;
    flag->present = false;
    return &flag->present;

}

void flag_help(FILE* out) {
    UNUSED(out);
    fprintf(out, "Usage:\n");
    fprintf(out, "\t%s [flags...] [options...]\n", prog_name);


    fprintf(out, "\toptions: \n");
    for(size_t i = 0; i < opt_count; ++i) {
        const struct option* opt = &opts[i];
        fprintf(out, "\t--%s=", opt->name);
        switch(opt->kind) {
            case INT: fprintf(out, "<int>    %d", opt->val.ival); break;
            case BOOL: fprintf(out, "<bool>    %s", opt->val.bval ? "true" : "false"); break;
            case STRING: fprintf(out, "<string>    %.*s", SVIEW_FMT_ARGS(opt->val.sval)); break;
            default: UNREACHABLE();
        }
        fprintf(out, "     %s\n", opt->description);
    }

    fprintf(out, "\tflags: \n");
}

static struct option* find_option_by_name(sview name) {
    for(size_t i = 0; i < opt_count; ++i) {
        struct option* opt = &opts[i];
        if(sview_eq(name, sview_create_null_terminated(opt->name))) {
            return opt;
        }
    }
    return NULL;
}

static bool set_opt_value(struct option* option, sview val) {
    switch(option->kind) {
    case INT: {
        errno = 0;
        const int ival = sview_to_i(val);
        if(errno != 0) {
            ERR("Failed to parse integer option '%s'. Provided value '%.*s'\n", option->name, SVIEW_FMT_ARGS(val));
            return false;
        }
        option->val.ival = ival;
        break;
    }
    case BOOL: {
        if(sview_eq(val, sview_create_lit("true"))) {
            option->val.bval = true;
        }else if(sview_eq(val, sview_create_lit("false"))) {
            option->val.bval = false;
        }else{
            ERR("Failed to parse bool option '%s'. Provided value '%.*s'\n", option->name, SVIEW_FMT_ARGS(val));
            return false;
        }
        break;
    }
    case STRING: {
        option->val.sval = val;
    }
    }
    return true;
}

size_t flag_argument_count(void) {
    return arg_count;
}

sview flag_argument_at(size_t idx) {
    return arguments[idx];
}

bool flag_parse(int argc, const char** argv) {
    if(argc == 0) {
        ERR("Not arguments provided (argc=0)\n");
        return false;
    }
    prog_name = argv[0];
    for(size_t i = 1; i < (size_t) argc; ++i) {
        sview arg = sview_create_null_terminated(argv[i]);
        sview name;
        if(sview_strip_prefix(arg, sview_create_lit("--"), &name)) {
            // option
            sview value;
            if(!sview_split(name, sview_create_lit("="), &name, &value)) {
                ERR("Syntax error for option %.*s\n", SVIEW_FMT_ARGS(name));
                return false;
            }
            struct option* opt = find_option_by_name(name);
            if(opt == NULL) {
                ERR("Option '%.*s' does not exist\n", SVIEW_FMT_ARGS(name));
                return false;
            }
            if(!set_opt_value(opt, value)) {
                return false;
            }
        }else if(sview_strip_prefix(arg, sview_create_lit("-"), &name)) {
            // flag list
            while(name.len > 0) {
                const char c = name.data[0];
                name = sview_chop_left(name, 1);

                bool found = false;
                for(size_t i = 0; i < flag_count; ++i) {
                    if(flags[i].name == c) {
                        flags[i].present = true;
                        found = true;
                        break;
                    }
                }
                if(!found) {
                    ERR("Flag '-%c' does not exist\n", c);
                    return false;
                }
            }
        }else {
            // normal argument
            arguments[arg_count++] = sview_create_null_terminated(argv[i]);
        }
    }
    return true;
}

void flag_reset(void) {
    opt_count = 0;
    arg_count = 0;
    flag_count = 0;
}
