#include "uthash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BRAILLE_TO_ENGLISH
#define BRAILLE_TO_ENGLISH

struct B2E_KeyValue
{
    char key[7];
    char value;
    UT_hash_handle hh;
};

extern struct B2E_KeyValue *BrailleToEngDict;

extern void B2E_addKeyValue(const char *key, char value);

extern char *B2E_findValueByKey(const char *key);

extern void B2E_initDict();

#endif
