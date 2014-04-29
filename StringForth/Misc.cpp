//
//  StringForth/Misc.cpp
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#include "Misc.h"
#include <cstring>
#include <ctype.h>

// ------------------------------------------------------------------------
// djb2 hash
//
unsigned int S4::Hash(const char *text) {
    unsigned int hash;

    hash = 5381;
    while (*text) {
        hash = ((hash << 5) + hash) + *(text++);
    }

    return hash;
}

// ------------------------------------------------------------------------
unsigned int S4::Hash_SDBM(const char *text) {
    unsigned int hash = *text;

    while (*text) {
        hash = *(text++) + (hash << 5) + (hash << 16);
    }

    return hash;
}

// ------------------------------------------------------------------------
bool S4::IsInteger(const char *string) {
    if (isdigit(string[0]) || ((string[0] == '-' || string[1] == '+') && isdigit(string[1]))) {
        do {
            string++;
        } while (isdigit(string[0]));
        return *string ? false : true;
    }
    return false;
}

// ------------------------------------------------------------------------
bool S4::IsNumber(const char *string) {
    if (isdigit(string[0]) || ((string[0] == '-' || string[1] == '+') && isdigit(string[1]))) {
        do {
            string++;
        } while (isdigit(string[0]));
        if (string[0] == '.' && isdigit(string[1])) {
            do {
                string++;
            } while (isdigit(string[0]));
        }
        return *string ? false : true;
    }
    return false;
}

// ------------------------------------------------------------------------
bool S4::IsText(const char *string) {
    return *string == '"' || *string == '\'';
}

// ------------------------------------------------------------------------
size_t S4::NextPowerOfTwo(size_t integer) {
    if (integer == 0) {
        return 2;
    } else if (!(integer & (integer - 1))) {
        // already is a power of two
        //
        return integer * 2;
    }
    while (integer & (integer - 1)) {
        integer &= (integer - 1);
    }
    return integer << 1;
}

// ------------------------------------------------------------------------
char *S4::StrCat(const char *s1, const char *s2) {
    s1 = s1 ? s1 : "";
    s2 = s2 ? s2 : "";
    ssize_t len1 = std::strlen(s1);
    ssize_t len2 = std::strlen(s2);
    char *dst = new char[len1 + len2 + 1];
    strcpy(dst, s1);
    strcpy(dst + len1, s2);
    return dst;
}

// ------------------------------------------------------------------------
char *S4::StrCat(const char *s1, const char *s2, const char *s3) {
    s1 = s1 ? s1 : "";
    s2 = s2 ? s2 : "";
    s3 = s3 ? s3 : "";
    ssize_t len1 = std::strlen(s1);
    ssize_t len2 = std::strlen(s2);
    ssize_t len3 = std::strlen(s3);
    char *dst = new char[len1 + len2 + len3 + 1];
    strcpy(dst, s1);
    strcpy(dst + len1, s2);
    strcpy(dst + len1 + len2, s3);
    return dst;
}

// ------------------------------------------------------------------------
char *S4::StrDup(const char *src) {
    return std::strcpy(new char[std::strlen(src ? src : "") + 1], src ? src : "");
}

// ------------------------------------------------------------------------
char *S4::StrNDup(const char *src, size_t length) {
    char *dst = new char[length + 1];
    memcpy(dst, src, length);
    dst[length] = 0;
    return dst;
}
