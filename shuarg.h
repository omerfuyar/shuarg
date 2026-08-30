#pragma once

#ifndef SHU_HEADER
#ifdef SHU
#include SHU
#else
#include "../shu/shu.h"
#endif
#endif

// todo unrecognized argument warnings, lists, default behaviour for args without prefixes (like gcc accepting src files as plain args)

#pragma region Macros

#ifndef SHUC_ARGS_DECIMAL_TYPE
#define SHUC_ARGS_DECIMAL_TYPE f64
#endif

#ifndef SHUC_ARGS_INTEGER_TYPE
#define SHUC_ARGS_INTEGER_TYPE i64
#endif

#ifndef SHUC_ARGS_BOOLEAN_TYPE
#define SHUC_ARGS_BOOLEAN_TYPE bool
#endif

#ifndef SHUC_ARGS_NUMBER_BUFFER
#define SHUC_ARGS_NUMBER_BUFFER 64
#endif

#define SHUM_ARGS_CHECK(arg, ...)                      \
    if (SHU_ArgsGetLastResult() && (0 __VA_OPT__(+1))) \
    {                                                  \
        __VA_ARGS__                                    \
    }

#pragma endregion Macros

#pragma region Declarations

void SHU_InitializeArgs(int argc, char **argv);

void SHU_TerminateArgs(void);

#define args(argc, argv)            \
    SHU_InitializeArgs(argc, argv); \
    for (bool __passed = false;     \
         !__passed;                 \
         SHU_TerminateArgs(), __passed = true)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argsString` MACRO INSTEAD !!!
SHUSliceView SHU_ArgsString(const char *prefix);

#define argsString(prefix, ...) \
    SHU_ArgsString(prefix);     \
    SHUM_ARGS_CHECK(prefix, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argsInteger` MACRO INSTEAD !!!
SHUC_ARGS_INTEGER_TYPE SHU_ArgsInteger(const char *prefix);

#define argsInteger(prefix, ...) \
    SHU_ArgsInteger(prefix);     \
    SHUM_ARGS_CHECK(prefix, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argsDecimal` MACRO INSTEAD !!!
SHUC_ARGS_DECIMAL_TYPE SHU_ArgsDecimal(const char *prefix);

#define argsDecimal(prefix, ...) \
    SHU_ArgsDecimal(prefix);     \
    SHUM_ARGS_CHECK(prefix, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argsBoolean` MACRO INSTEAD !!!
SHUC_ARGS_BOOLEAN_TYPE SHU_ArgsBoolean(const char *prefix);

#define argsBoolean(flag, ...) \
    SHU_ArgsBoolean(flag);     \
    SHUM_ARGS_CHECK(flag, ##__VA_ARGS__)

#pragma endregion Declarations

#pragma region Definitions

#ifdef SHU_IMPLEMENTATION

#pragma region Internals

static struct
{
    int argc;
    char **argv;
} SHUARG = {0};

#pragma endregion Internals

void SHU_InitializeArgs(int argc, char **argv)
{
    SHUARG.argc = argc;
    SHUARG.argv = argv;
}

void SHU_TerminateArgs(void)
{
}

SHUSliceView SHU_ArgsString(const char *prefix)
{
}

SHUC_ARGS_INTEGER_TYPE SHU_ArgsInteger(const char *prefix)
{
}

SHUC_ARGS_DECIMAL_TYPE SHU_ArgsDecimal(const char *prefix)
{
}

SHUC_ARGS_BOOLEAN_TYPE SHU_ArgsBoolean(const char *prefix)
{
}

#endif

#pragma endregion Definitions
