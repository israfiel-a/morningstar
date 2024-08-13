/**
 * @file Warning.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides an in-application @def errno functionality, for catching
 * warnings set by functions. This is incredibly useful for catching errors
 * before they arise.
 * @date 2024-08-13
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_WARNING_OUTPUT_SYSTEM_
#define _MSENG_WARNING_OUTPUT_SYSTEM_

#include <Master.h>

typedef enum
{
    no_warning,
    test_warning
} warning_code_t;

typedef struct
{
    const char* setter;
    warning_code_t code;
} warning_t;

void ReportWarning_(const char* function, warning_code_t code);

#define ReportWarning(code) ReportWarning_(__func__, code)

const warning_t* ReadWarning(void);

#endif // _MSENG_WARNING_OUTPUT_SYSTEM_
