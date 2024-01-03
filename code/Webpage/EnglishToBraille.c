#include "EnglishToBraille.h"

struct E2B_KeyValue *EngToBrailleDict = NULL;

void E2B_addKeyValue(char key, const char *value)
{
    struct E2B_KeyValue *item = (struct E2B_KeyValue *)malloc(sizeof(struct E2B_KeyValue));
    item->key = key;
    strncpy(item->value, value, sizeof(item->value));
    item->value[sizeof(item->value) - 1] = '\0'; // Ensure null-terminated
    HASH_ADD(hh, EngToBrailleDict, key, sizeof(char), item);
}

char *E2B_findValueByKey(char key)
{
    struct E2B_KeyValue *item;
    HASH_FIND(hh, EngToBrailleDict, &key, sizeof(char), item);
    return item->value;
}

void E2B_initDict()
{
    E2B_addKeyValue('a', "100000");
    E2B_addKeyValue('b', "110000");
    E2B_addKeyValue('c', "100100");
    E2B_addKeyValue('d', "100110");
    E2B_addKeyValue('e', "100010");
    E2B_addKeyValue('f', "110100");
    E2B_addKeyValue('g', "110110");
    E2B_addKeyValue('h', "110010");
    E2B_addKeyValue('i', "010100");
    E2B_addKeyValue('j', "100100");
    E2B_addKeyValue('k', "101000");
    E2B_addKeyValue('l', "111000");
    E2B_addKeyValue('m', "101100");
    E2B_addKeyValue('n', "101110");
    E2B_addKeyValue('o', "101010");
    E2B_addKeyValue('p', "111100");
    E2B_addKeyValue('q', "111110");
    E2B_addKeyValue('r', "111010");
    E2B_addKeyValue('s', "011100");
    E2B_addKeyValue('t', "011110");
    E2B_addKeyValue('u', "101001");
    E2B_addKeyValue('v', "111001");
    E2B_addKeyValue('w', "010111");
    E2B_addKeyValue('x', "101101");
    E2B_addKeyValue('y', "101111");
    E2B_addKeyValue('z', "101011");
}
