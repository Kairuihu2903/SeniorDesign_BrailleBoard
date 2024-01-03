#include "BrailleToEnglish.h"

struct B2E_KeyValue *BrailleToEngDict = NULL;

void B2E_addKeyValue(const char *key, char value)
{
    struct B2E_KeyValue *item = (struct B2E_KeyValue *)malloc(sizeof(struct B2E_KeyValue));
    strncpy(item->key, key, sizeof(item->key));
    item->key[sizeof(item->key) - 1] = '\0'; // Ensure null-terminated
    item->value = value;
    HASH_ADD_STR(BrailleToEngDict, key, item);
}

char *B2E_findValueByKey(const char *key)
{
    struct B2E_KeyValue *item;
    HASH_FIND_STR(BrailleToEngDict, key, item);
    return &item->value;
}

void B2E_initDict()
{
    B2E_addKeyValue("100000", 'a');
    B2E_addKeyValue("110000", 'b');
    B2E_addKeyValue("100100", 'c');
    B2E_addKeyValue("100110", 'd');
    B2E_addKeyValue("100010", 'e');
    B2E_addKeyValue("110100", 'f');
    B2E_addKeyValue("110110", 'g');
    B2E_addKeyValue("110010", 'h');
    B2E_addKeyValue("010100", 'i');
    B2E_addKeyValue("100100", 'j');
    B2E_addKeyValue("101000", 'k');
    B2E_addKeyValue("111000", 'l');
    B2E_addKeyValue("101100", 'm');
    B2E_addKeyValue("101110", 'n');
    B2E_addKeyValue("101010", 'o');
    B2E_addKeyValue("111100", 'p');
    B2E_addKeyValue("111110", 'q');
    B2E_addKeyValue("111010", 'r');
    B2E_addKeyValue("011100", 's');
    B2E_addKeyValue("011110", 't');
    B2E_addKeyValue("101001", 'u');
    B2E_addKeyValue("111001", 'v');
    B2E_addKeyValue("010111", 'w');
    B2E_addKeyValue("101101", 'x');
    B2E_addKeyValue("101111", 'y');
    B2E_addKeyValue("101011", 'z');
}