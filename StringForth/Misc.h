//
//  StringForth/Misc.h
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#ifndef __StringForth__Misc__
#define __StringForth__Misc__

#include <cstddef>

// utility definitions
//
namespace S4 {
    unsigned int Hash(const char *text);
    unsigned int Hash_SDBM(const char *text);
    bool         IsInteger(const char *string);
    bool         IsNumber(const char *string);
    bool         IsText(const char *string);
    size_t       NextPowerOfTwo(size_t integer);
    char        *StrCat(const char *s1, const char *s2);
    char        *StrCat(const char *s1, const char *s2, const char *s3);
    char        *StrDup(const char *src);
    char        *StrNDup(const char *src, size_t length);
} // namespace S4

#endif /* defined(__StringForth__Misc__) */
