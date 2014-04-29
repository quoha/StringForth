//
//  StringForth/StringForth.h
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#ifndef StringForth_StringForth_h
#define StringForth_StringForth_h

#include <cstddef>

// forward class declarations
//
namespace S4 {
    class Cell;
    class Dictionary;
    class Interpreter;
    class Lexeme;
    class Lexer;
    class Stack;
    class Variant;
    class Word;
} // namespace S4

// utility definitions
//
namespace S4 {
    unsigned int Hash(const char *text);
    bool         IsInteger(const char *string);
    bool         IsNumber(const char *string);
    bool         IsText(const char *string);
    char        *StrCat(const char *s1, const char *s2);
    char        *StrDup(const char *src);
    char        *StrNDup(const char *src, size_t length);
} // namespace S4

#include "Cell.h"
#include "Dictionary.h"
#include "Interpreter.h"
#include "Lexeme.h"
#include "Misc.h"

#endif /* defined(__StringForth__StringForth__) */
