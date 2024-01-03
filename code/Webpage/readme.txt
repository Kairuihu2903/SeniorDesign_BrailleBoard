This is a program that translates between english text and braille code.

(Example of braille code numbering referenced here: https://www.nbp.org/alphcard.pdf)

It consists of two modules to facilitate the translations:

- EnglishToBraille
- BrailleToEnglish

The modules each store a one-to-one character translation between the two langauges in a hashmap for optimal lookup speed.

The hashmap implementation was done through uthash.h (https://www.cs.bu.edu/~jappavoo/Resources/psml/apps/hashy/uthash/doc/).

It is possible that uthash.h will not work on the ESP device.

If that is the case, then I will use C++ because it comes with a hashmap.
Or maybe I just just lots of if statements.