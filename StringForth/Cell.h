//
//  StringForth/Cell.h
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
// ------------------------------------------------------------------------
// Cell is the basic unit of memory (BUM) for the StringForth interpreter.
// You could also consider them the primitive datatypes for this
// interpreter.
// ------------------------------------------------------------------------

#ifndef __StringForth__Cell__
#define __StringForth__Cell__

#include "StringForth.h"

namespace S4 {
    struct Cell {
        enum { code, integer, null, number, text } kind;
        union {
            size_t      coreOffset;
            long        integer;
            double      number;
            const char *text;
        } data;
    }; // struct Cell

    Cell *CellFromCell(Cell *cell);
    Cell *CellFromInteger(long integer);
    Cell *CellFromNull();
    Cell *CellFromNumber(double number);
    Cell *CellFromOffset(size_t offset);
    Cell *CellFromText(const char *text);
} // namespace S4

#endif /* defined(__StringForth__Cell__) */
