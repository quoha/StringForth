//
//  StringForth/Word.cpp
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#include "Word.h"
#include <stdio.h>
#include <stdlib.h>

// ------------------------------------------------------------------------
S4::Word::Word(const char *name_) {
    name = StrDup(name_);
    coreOffset = 0;
}

// ------------------------------------------------------------------------
S4::Word::Word(const char *name_, size_t coreOffset_) {
    name = StrDup(name_);
    coreOffset = coreOffset_;
}

// ------------------------------------------------------------------------
S4::Word::~Word() {
    delete [] name;
}

// ------------------------------------------------------------------------
size_t S4::Word::CoreOffset() const {
    return coreOffset;
}

// ------------------------------------------------------------------------
// default compile behaviour is to ignore the word
//
bool S4::Word::Compile(S4::Interpreter *interpreter) {
    return true;
}

// ------------------------------------------------------------------------
// execute the code by saving the current program counter, setting it to
// the code pointer, and then letting the interpreter take over.
//
// this simulates a gosub call.
//
bool S4::Word::Execute(S4::Interpreter *interpreter) {
    return interpreter->GoSub(coreOffset);
}

// ------------------------------------------------------------------------
bool S4::Word::IsPrimitive() const {
    return coreOffset ? false : true;
}

// ------------------------------------------------------------------------
const char *S4::Word::Name() const {
    return name ? name : "";
}

// ------------------------------------------------------------------------
S4::Primitive::Add::Add(void) : Word("+") {
}

// ------------------------------------------------------------------------
// x2 x1 -- x3 { x3 <- x1 + x2 }
//
bool S4::Primitive::Add::Execute(S4::Interpreter *interpreter) {
    int minimumHeight = 2;
    if (interpreter->Height() < minimumHeight) {
        printf("error:\t%s requires minimum stack height of %d\n", Name(), minimumHeight);
        return false;
    }

    // will reuse x1 to store the result
    //
    Cell *x1 = interpreter->Pop();
    Cell *x2 = interpreter->Pop();
    const char *tmp;

    switch (x1->kind) {
        case Cell::code:
            x1->kind         = Cell::null;
            x1->data.integer = 0;
            break;
        case Cell::integer:
            switch (x2->kind) {
                case Cell::code:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::integer:
                    x1->data.integer += x2->data.integer;
                    break;
                case Cell::null:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::number:
                    x1->kind          = Cell::number;
                    x1->data.number   = x1->data.integer + x2->data.number;
                    break;
                case Cell::text:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
            }
            break;
        case Cell::null:
            break;
        case Cell::number:
            switch (x2->kind) {
                case Cell::code:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::integer:
                    x1->data.number  += x2->data.integer;
                    break;
                case Cell::null:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::number:
                    x1->data.number  += x2->data.number;
                    break;
                case Cell::text:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
            }
            break;
        case Cell::text:
            switch (x2->kind) {
                case Cell::code:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::integer:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::null:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::number:
                    x1->kind          = Cell::null;
                    x1->data.integer  = 0;
                    break;
                case Cell::text:
                    tmp = x1->data.text;
                    x1->data.text = StrCat(x2->data.text, x1->data.text);
                    delete [] tmp;
                    break;
            }
            break;
    }

    delete x2;

    interpreter->Push(x1);

    return true;
}

// ------------------------------------------------------------------------
S4::Primitive::CloseBrace::CloseBrace(void) : Word("}", 0) {
}

// ------------------------------------------------------------------------
//
bool S4::Primitive::CloseBrace::Compile(S4::Interpreter *interpreter) {
    // turn off interpretation mode until we encounter the end of the definition
    //
    interpreter->InterpretMode();

    return true;
}

// ------------------------------------------------------------------------
//
bool S4::Primitive::CloseBrace::Execute(S4::Interpreter *interpreter) {
    int minimumHeight = 1;
    if (interpreter->RSHeight() < minimumHeight) {
        printf("error:\t%s requires minimum return stack height of %d\n", Name(), minimumHeight);
        return false;
    }

    return interpreter->Return();
}

// ------------------------------------------------------------------------
S4::Primitive::Dot::Dot(void) : Word(".", 0) {
}

// ------------------------------------------------------------------------
// x1 -- { pops top of stack and displays value }
//
bool S4::Primitive::Dot::Execute(S4::Interpreter *interpreter) {
    int minimumHeight = 1;
    if (interpreter->Height() < minimumHeight) {
        printf("error:\t%s requires minimum stack height of %d\n", Name(), minimumHeight);
        return false;
    }

    Cell *x1 = interpreter->Pop();

    switch (x1->kind) {
        case Cell::code:
            printf(".disp:\tds code\n");
            delete x1;
            return true;
        case Cell::integer:
            printf(".disp:\tds integer %ld\n", x1->data.integer);
            delete x1;
            return true;
        case Cell::null:
            printf(".disp:\tds null\n");
            delete x1;
            return true;
        case Cell::number:
            printf(".disp:\tds number  %g\n", x1->data.number);
            delete x1;
            return true;
        case Cell::text:
            printf(".disp:\tds text    %s\n", x1->data.text);
            delete x1;
            return true;
    }

    printf("panic:\t%s %s %d\n\tstack top unknown type %d\n", __FILE__, __FUNCTION__, __LINE__, x1->kind);

    delete x1;
    return false;
}

// ------------------------------------------------------------------------
S4::Primitive::Dup::Dup(void) : Word("dup", 0) {
}

// ------------------------------------------------------------------------
// x -- x x { duplicates top of stack }
//
bool S4::Primitive::Dup::Execute(S4::Interpreter *interpreter) {
    int minimumHeight = 1;
    if (interpreter->Height() < minimumHeight) {
        printf("error:\t%s requires minimum stack height of %d\n", Name(), minimumHeight);
        return false;
    }

    interpreter->Push(CellFromCell(interpreter->Top()));

    return true;
}

// ------------------------------------------------------------------------
S4::Primitive::OpenBrace::OpenBrace(void) : Word("{", 0) {
}

// ------------------------------------------------------------------------
//
bool S4::Primitive::OpenBrace::Execute(S4::Interpreter *interpreter) {
    int minimumHeight = 1;
    if (interpreter->Height() < minimumHeight) {
        printf("error:\t%s requires minimum stack height of %d\n", Name(), minimumHeight);
        return false;
    }

    Cell *nameOfWord = interpreter->Pop();
    if (nameOfWord->kind != Cell::text) {
        printf("error:\t%s requires one text argument for the name of the word being defined\n", Name());
        return false;
    }

    interpreter->AddWord(new Word(nameOfWord->data.text, interpreter->ProgramCounter()));

    // turn off interpretation mode until we encounter the end of the definition
    //
    interpreter->CompileMode();

    return true;
}

// ------------------------------------------------------------------------
S4::Primitive::StackHeight::StackHeight(void) : Word("stackHeight", 0) {
}

// ------------------------------------------------------------------------
// -- n { pushes height of stack onto top of stack }
//
bool S4::Primitive::StackHeight::Execute(S4::Interpreter *interpreter) {
    interpreter->Push(CellFromInteger(interpreter->Height()));
    return true;
}


