//
//  StringForth/Cell.cpp
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.

#include "Cell.h"
#include "Misc.h"
#include <stdlib.h>

S4::Cell *S4::CellFromCell(S4::Cell *cell) {
    switch (cell->kind) {
        case Cell::code:
            return CellFromOffset(cell->data.coreOffset);
        case Cell::integer:
            return CellFromInteger(cell->data.integer);
        case Cell::null:
            return CellFromNull();
        case Cell::number:
            return CellFromNumber(cell->data.number);
        case Cell::text:
            return CellFromText(cell->data.text);
    }
    printf("panic:\t%s %s %d\n\tunknown cell type %d\n", __FILE__, __FUNCTION__, __LINE__, cell->kind);
    exit(2);

    // NOT REACHED
    return 0;
}

S4::Cell *S4::CellFromInteger(long integer_) {
    Cell *cell = new Cell();
    cell->kind = Cell::integer;
    cell->data.integer = integer_;
    return cell;
}

S4::Cell *S4::CellFromNull() {
    Cell *cell = new Cell();
    cell->kind = Cell::null;
    cell->data.integer = 0;
    return cell;
}

S4::Cell *S4::CellFromNumber(double number_) {
    Cell *cell = new Cell();
    cell->kind = Cell::number;
    cell->data.number = number_;
    return cell;
}

S4::Cell *S4::CellFromOffset(size_t offset_) {
    Cell *cell = new Cell();
    cell->kind = Cell::code;
    cell->data.coreOffset = offset_;
    return cell;
}

S4::Cell *S4::CellFromText(const char *text_) {
    Cell *cell = new Cell();
    cell->kind = Cell::text;
    cell->data.text = text_;
    return cell;
}

