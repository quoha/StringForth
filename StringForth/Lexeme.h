//
//  StringForth/Lexeme.h
//
//  Created by Michael Henderson on 4/28/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#ifndef __StringForth__Lexeme__
#define __StringForth__Lexeme__

#include "StringForth.h"

namespace S4 {
    struct Lexeme {
        Lexeme() {}
        ~Lexeme() {}

        enum { endOfInput, integer, number, symbol, text } kind;

        union {
            int    integer;
            double number;
            char  *symbol;
            char  *text;
        } data;

        bool IsEndOfInput(void) { return kind == endOfInput; }
        bool IsInteger(void) { return kind == integer; }
        bool IsNumber(void) { return kind == number; }
        bool IsSymbol(void) { return kind == symbol; }
        bool IsText(void) { return kind == text; }
    }; // struct Lexeme

} // namespace S4

#endif /* defined(__StringForth__Lexeme__) */
