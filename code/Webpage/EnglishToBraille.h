#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ENGLISH_TO_BRAILLE
#define ENGLISH_TO_BRAILLE

struct E2B_KeyValue
{
    char key;
    char value[7];
    UT_hash_handle hh;
};

extern struct E2B_KeyValue *EngToBrailleDict;

extern void E2B_addKeyValue(char key, const char *value);

extern char *E2B_findValueByKey(char key);

extern void E2B_initDict();

#endif
