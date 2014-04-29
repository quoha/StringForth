//
//  StringForth/Interpreter.h
//
//  Created by Michael Henderson on 4/28/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#ifndef __StringForth__Interpreter__
#define __StringForth__Interpreter__

#include "StringForth.h"
#include "Cell.h"
#include "Lexeme.h"

#include <stack>

namespace S4 {
    class Interpreter {
        char  *programCounter;
        char  *core;
        char  *endOfCore;
        size_t sizeOfCore;
        bool   compileMode;

        Dictionary *dictionary;

        std::stack<Cell *> dataStack;
        std::stack<size_t> returnStack;

        bool    NextWord(Lexeme *lexeme);
        void    Realloc(size_t newCoreSize);

    public:
        Interpreter(const char *program);

        void   AddPrimitive(Word *primitive);
        void   AddWord(Word *word);
        void   CompileMode();
        bool   GoSub(size_t coreOffset);
        size_t Height() const;
        void   InterpretMode();
        bool   IsCompileMode() const;
        Cell  *Pop();
        size_t ProgramCounter() const;
        void   Push(Cell *cell);
        bool   Return();
        size_t RSHeight() const;
        size_t RSPop();
        void   RSPush(size_t coreOffset);
        bool   Step();
        Cell  *Top() const;
    }; // class Interpreter

} // namespace S4

#endif /* defined(__StringForth__Interpreter__) */
