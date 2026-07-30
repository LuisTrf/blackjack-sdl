#pragma once

#define STRING(s) #s
#define XSTRING(s) STRING(s)
#define DOLLAR_STRING(s) "$" XSTRING(s)