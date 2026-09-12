#pragma once

#ifndef SHU_HEADER
#ifdef SHU
#include SHU
#else
#include "../shu/shu.h"
#endif
#endif

// todo unrecognized argument warnings, lists, default behaviour for args without keyes (like gcc accepting src files as plain args)

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

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `args` MACRO INSTEAD !!!
bool SHU_InitializeArgs(int argc, char **argv);

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `args` MACRO INSTEAD !!!
void SHU_TerminateArgs(void);

/// @brief Creates a block to parse arguments. Automatically initializes and terminates the library. Use it as args(/*Your args*/){/*Your Code*/}
/// @param argc Argument count got from main function.
/// @param argv Argument string vector got from main function.
#define args(argc, argv)                                 \
    for (bool __passed = SHU_InitializeArgs(argc, argv); \
         !__passed;                                      \
         SHU_TerminateArgs(), __passed = true)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argString` MACRO INSTEAD !!!
SHUSliceView SHU_ArgList(const char *key);

/// @brief Tries getting a string list with given key. Use it only inside args block.
/// @param key Key to find the desired argument (eg: myKey -searched in args> '-myKey value')
/// @param onFail Code section to call if desired argument cannot be found. Does nothing if this section is empty. Do not forget semicolons.
/// @return Slice view of the list, data being pointer to the first argument, size being argument count. Increment the data pointer to get to next list element.
#define argList(key, ...) \
    SHU_ArgList(key);     \
    SHUM_ARGS_CHECK(key, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argString` MACRO INSTEAD !!!
SHUSliceView SHU_ArgString(const char *key);

/// @brief Tries getting the string value with given key. Use it only inside args block.
/// @param key Key to find the desired argument (eg: myKey -searched in args> '-myKey value')
/// @param onFail Code section to call if desired argument cannot be found. Does nothing if this section is empty. Do not forget semicolons.
/// @return
#define argString(key, ...) \
    SHU_ArgString(key);     \
    SHUM_ARGS_CHECK(key, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argInteger` MACRO INSTEAD !!!
SHUC_ARGS_INTEGER_TYPE SHU_ArgInteger(const char *key);

/// @brief Tries getting the integer number with given key. Use it only inside args block.
/// @param key Key to find the desired argument (eg: myKey -searched in args> '-myKey value')
/// @param onFail Code section to call if desired argument cannot be found. Does nothing if this section is empty. Do not forget semicolons.
/// @return
#define argInteger(key, ...) \
    SHU_ArgInteger(key);     \
    SHUM_ARGS_CHECK(key, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argDecimal` MACRO INSTEAD !!!
SHUC_ARGS_DECIMAL_TYPE SHU_ArgDecimal(const char *key);

/// @brief Tries getting the decimal number with given key. Use it only inside args block.
/// @param key Key to find the desired argument (eg: myKey -searched in args> '-myKey value')
/// @param onFail Code section to call if desired argument cannot be found. Does nothing if this section is empty. Do not forget semicolons.
/// @return
#define argDecimal(key, ...) \
    SHU_ArgDecimal(key);     \
    SHUM_ARGS_CHECK(key, ##__VA_ARGS__)

/// !!! DO NOT USE THIS FUNCTION ALONE, USE `argBoolean` MACRO INSTEAD !!!
SHUC_ARGS_BOOLEAN_TYPE SHU_ArgBoolean(const char *key);

/// @brief Tries getting the boolean flag with given key. Use it only inside args block.
/// @param key Key to find the desired argument (eg: myKey -searched in args> '-myKey value')
/// @param onFail Code section to call if desired argument cannot be found. Does nothing if this section is empty. Do not forget semicolons.
/// @return
#define argBoolean(flag, ...) \
    SHU_ArgBoolean(flag);     \
    SHUM_ARGS_CHECK(flag, ##__VA_ARGS__)

#pragma endregion Declarations

#pragma region Definitions

#ifdef SHU_IMPLEMENTATION

#pragma region Internals

static struct
{
    SHUResult lastResult;
    usz argCount;   // argc
    char **argData; // got from argv, immutable
    bool *argsUsed; // malloced with argCount
} SHUARG = {0};

static bool SHUI_StringsAreSame(const char *strA, const char *strB)
{
    for (usz i = 0;; i++)
    {
        char chrA = strA[i];
        char chrB = strB[i];

        if (chrA != chrB)
        {
            return false;
        }

        if (chrA == '\0')
        {
            break;
        }
    }

    return true;
}

#pragma endregion Internals

bool SHU_InitializeArgs(int argc, char **argv)
{
    SHUARG.argCount = (usz)argc;
    SHUARG.argData = argv;
    SHUARG.argsUsed = malloc(sizeof(bool) * SHUARG.argCount);
    SHU_Assert(SHUARG.argsUsed, "Internal: Memory allocation failed for size '%zu'.", SHUARG.argCount);
    memset(SHUARG.argsUsed, 0x00, SHUARG.argCount);

    return false;
}

void SHU_TerminateArgs(void)
{
    for (usz i = 0; i < SHUARG.argCount; i++)
    {
        bool *currentArgIsUsed = SHUARG.argsUsed + i;
        char *currentArgStr = SHUARG.argData + i;

        if (*currentArgIsUsed)
        {
            SHU_LogWarning("Parameter %.*s is not recognized.", currentArgStr);
        }
    }

    memset(SHUARG.argsUsed, 0x00, SHUARG.argCount);
    free(SHUARG.argData);
}

SHUSliceView SHU_ArgString(const char *key)
{
    for (usz i = 0; i < SHUARG.argCount; i++)
    {
        bool *currentArgIsUsed = SHUARG.argsUsed + i;
        char *currentArgStr = SHUARG.argData + i;

        if (*currentArgIsUsed)
        {
            continue;
        }

        if (strcmp(key, currentArgStr) == 0)
        {
            *currentArgIsUsed = true;
            // parse
            // return
        }
    }
}

SHUC_ARGS_INTEGER_TYPE SHU_ArgInteger(const char *key)
{
}

SHUC_ARGS_DECIMAL_TYPE SHU_ArgDecimal(const char *key)
{
}

SHUC_ARGS_BOOLEAN_TYPE SHU_ArgBoolean(const char *key)
{
}

#endif

#pragma endregion Definitions
