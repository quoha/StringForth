//
//  StringForth/Word.h
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
// ------------------------------------------------------------------------
// Words can either be primitives, meaning C++ functions, or extensions,
// meaning text that can be executed. Extensions must reside in the core
// memory of the interpreter.
// ------------------------------------------------------------------------

#ifndef __StringForth__Word__
#define __StringForth__Word__

#include "StringForth.h"

namespace S4 {
    // for an extension, the interpreter should execute the code by
    //  1. saving the current program counter
    //  2. call CoreOffset to get the new programCounter
    //  3. let the interpreter take over
    //  4. on return, pop the program counter
    //
    // this simulates a gosub call.
    //
    // for a primitive, the interpreter should call the Execute function
    //
    class Word {
        char    *name;
        size_t   coreOffset;
    public:
        Word(const char *name);
        Word(const char *name, size_t coreOffset);
        ~Word();

        virtual size_t       CoreOffset() const;
        virtual bool         Compile(Interpreter *interpreter);
        virtual bool         Execute(Interpreter *interpreter);
        virtual bool         IsPrimitive() const;
        virtual const char  *Name() const;
    }; // class Word

    namespace Primitive {
        class Add : public Word {
        public:
            Add();
            bool Execute(Interpreter *interpreter);
        };
        class CloseBrace : public Word {
        public:
            CloseBrace();
            bool Compile(Interpreter *interpreter);
            bool Execute(Interpreter *interpreter);
        };
        class Dot : public Word {
        public:
            Dot();
            bool Execute(Interpreter *interpreter);
        };
        class Dup : public Word {
        public:
            Dup();
            bool Execute(Interpreter *interpreter);
        };
        class OpenBrace : public Word {
        public:
            OpenBrace();
            bool Execute(Interpreter *interpreter);
        };
        class StackHeight : public Word {
        public:
            StackHeight();
            bool Execute(Interpreter *interpreter);
        };
    } // namespace Primitive
} // namespace S4

#endif /* defined(__StringForth__Word__) */
