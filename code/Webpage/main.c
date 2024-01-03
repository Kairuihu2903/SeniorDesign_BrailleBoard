#include "EnglishToBraille.h"
#include "BrailleToEnglish.h"

int main()
{
    // HARDCODED TEST (success)
    // // e2b test
    // E2B_initDict();
    // char *b_code = E2B_findValueByKey('h');
    // printf("%s\n", b_code);
    // // b2e test
    // B2E_initDict();
    // char eng_char = *B2E_findValueByKey("100000");
    // printf("%c\n", eng_char);

    // INTERACTIVE TEST (pending)
    E2B_initDict();
    printf("Enter a lower-case letter to convert to braille code: ");
    char ch = getchar();
    char *b_code = E2B_findValueByKey(ch);
    printf("Character (%c) is represented as %s in braille code.\n", ch, b_code);
}