#ifndef PINYIN_H
#define PINYIN_H


// 1 str1 > str2
// 0 str1 = str2
// -1 str1 < str2
int pinyin_strncmp_utf8(const void * str1, const void * str2, int n);

int pinyin_cmp_utf8(void * NotUsed,
        int nKey1, const void * pKey1,
        int nKey2, const void * pKey2);


#endif
