#include "Warning.h"
#include "Messages.h"
#include <stddef.h>

warning_t warningno = {NULL, no_warning};

void ReportWarning_(const char* function, warning_code_t code)
{
    warningno = (warning_t){function, code};
    ReportMessage("function %s raised warning %d", function, code);
}

const warning_t* ReadWarning(void) { return &warningno; }
