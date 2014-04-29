//
//  StringForth/Interpreter.cpp
//
//  Created by Michael Henderson on 4/28/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#include "Interpreter.h"
#include <stdlib.h>
#include <ctype.h>

// ------------------------------------------------------------------------
S4::Interpreter::Interpreter(const char *program) {
    static const char *coreWords = "42 .";

    dictionary = new Dictionary();

    program = program ? program : "";

    sizeOfCore = NextPowerOfTwo(std::strlen(coreWords) + 1 + std::strlen(program));
    core = new char[sizeOfCore];
    endOfCore = core + sizeOfCore;
    *(endOfCore - 1) = 0;

    // load primitives
    AddPrimitive(new S4::Primitive::Add());
    AddPrimitive(new S4::Primitive::CloseBrace());
    AddPrimitive(new S4::Primitive::Dot());
    AddPrimitive(new S4::Primitive::Dup());
    AddPrimitive(new S4::Primitive::OpenBrace());
    AddPrimitive(new S4::Primitive::StackHeight());

    // put core words at the beginning of memory and the program after it
    core = StrCat(coreWords, " ", program);

    printf(".core:\t%s\n", core);
    // and initialize the program counter
    programCounter = core;
}

// ------------------------------------------------------------------------
//
void S4::Interpreter::AddPrimitive(S4::Word *primitive) {
    dictionary->Add(primitive);
}

// ------------------------------------------------------------------------
//
void S4::Interpreter::AddWord(S4::Word *word) {
    dictionary->Add(word);
}

// ------------------------------------------------------------------------
//
void S4::Interpreter::CompileMode() {
    compileMode = true;
}

// ------------------------------------------------------------------------
//
bool S4::Interpreter::GoSub(size_t coreOffset) {
    if (sizeOfCore < coreOffset) {
        printf("panic:\t%s %s %d\n\tGoSub address is invalid (%ld/%ld)\n", __FILE__, __FUNCTION__, __LINE__, coreOffset, sizeOfCore);
        exit(2);
    }

    RSPush(programCounter - core);

    programCounter = coreOffset + core;

    return true;
}

// ------------------------------------------------------------------------
//
size_t S4::Interpreter::Height() const {
    return dataStack.size();
}

// ------------------------------------------------------------------------
//
void S4::Interpreter::InterpretMode() {
    compileMode = false;
}

// ------------------------------------------------------------------------
//
bool S4::Interpreter::IsCompileMode() const {
    return compileMode;
}

// ------------------------------------------------------------------------
bool S4::Interpreter::NextWord(S4::Lexeme *lexeme) {
    lexeme->kind = Lexeme::endOfInput;

    while (isspace(*programCounter)) {
        programCounter++;
    }

    if (*programCounter) {
        const char *startOfLexeme = programCounter;

        if (*programCounter == '\'' || *programCounter == '"' || *programCounter == '`') {
            char quote         = *(programCounter++);
            int  escapedQuotes = 0;
            int  escapedChars  = 0;
            while (*programCounter) {
                if (programCounter[0] == quote) {
                    if (programCounter[1] == quote) {
                        escapedQuotes   = 1;
                        programCounter += 2;
                        continue;
                    } else if (!programCounter[1] || isspace(programCounter[1])) {
                        // end of text
                        break;
                    }
                } else if (programCounter[0] == '\\') {
                    escapedChars = 1;
                }
                programCounter++;
            }
            lexeme->kind = Lexeme::text;
            // don't include the quotes in the copy
            //
            if (*programCounter == quote) {
                // advance past the closing quote before the copy
                programCounter++;
                lexeme->data.text = StrNDup(startOfLexeme + 1, (programCounter - 1) - (startOfLexeme + 1));
            } else {
                // unterminated string, sigh
                lexeme->data.text = StrNDup(startOfLexeme + 1, programCounter - (startOfLexeme + 1));
            }
            // TODO: combine quotes and backslash logic
            //
            if (escapedQuotes) {
                char *src = lexeme->data.text;
                while (*src && !(src[0] == quote && src[1] == quote)) {
                    src++;
                }
                char *dst = src;
                while (*src) {
                    if (src[0] == quote && src[1] == quote) {
                        src++;
                        *(dst++) = *(src++);
                    } else {
                        *(dst++) = *(src++);
                    }
                }
                *dst = 0;
            }
            if (escapedChars) {
                char *src = lexeme->data.text;
                while (*src && *src != '\\') {
                    src++;
                }
                char *dst = src;
                while (*src) {
                    if (*src != '\\') {
                        *(dst++) = *(src++);
                    } else {
                        switch (*(++src)) { // inspired by C strings
                            case 0:
                                // never escape null bytes (or quotes for that matter)
                                break;
                            case '0': // \0 is a null byte in the string
                                *(dst++) = 0;
                                src++;
                                break;
                            case 'n': // \n is a newline
                                *(dst++) = '\n';
                                src++;
                                break;
                            case 'r': // \r is a carriage return
                                *(dst++) = '\r';
                                src++;
                                break;
                            case 't': // \t is a tab
                                *(dst++) = '\t';
                                src++;
                                break;
                            default:
                                *(dst++) = *(src++);
                                break;
                        }
                    }
                }
                *dst = 0;
            }
            return lexeme;
        }

        if (isdigit(programCounter[0]) || ((programCounter[0] == '-' || programCounter[1] == '+') && isdigit(programCounter[1]))) {
            do {
                programCounter++;
            } while (isdigit(programCounter[0]));
            if (isspace(*programCounter) || !*programCounter) {
                lexeme->kind = Lexeme::integer;
                lexeme->data.integer = atoi(startOfLexeme);
                return lexeme;
            }
            if (programCounter[0] == '.' && isdigit(programCounter[1])) {
                do {
                    programCounter++;
                } while (isdigit(programCounter[0]));
                if (isspace(*programCounter) || !*programCounter) {
                    lexeme->kind = Lexeme::number;
                    lexeme->data.number = strtod(startOfLexeme, 0);
                    return lexeme;
                }
            }
        }

        while (*programCounter && !isspace(*programCounter)) {
            programCounter++;
        }
        lexeme->kind = Lexeme::symbol;
        lexeme->data.symbol = StrNDup(startOfLexeme, programCounter - startOfLexeme);
        return lexeme;
    }

    lexeme->kind = Lexeme::endOfInput;

    return false;
}

// ------------------------------------------------------------------------
// x -- { pop and return top element of stack }
//
S4::Cell *S4::Interpreter::Pop() {
    if (!dataStack.size()) {
        printf("panic:\t%s %s %d\n\tpopping empty stack\n", __FILE__, __FUNCTION__, __LINE__);
        exit(2);
    }
    Cell *x = dataStack.top();
    dataStack.pop();
    return x;
}

// ------------------------------------------------------------------------
//
size_t S4::Interpreter::ProgramCounter() const {
    return programCounter - core;
}

// ------------------------------------------------------------------------
// -- x { push x on to stack }
//
void S4::Interpreter::Push(S4::Cell *cell) {
    dataStack.push(cell);
}

// ------------------------------------------------------------------------
void S4::Interpreter::Realloc(size_t newCoreSize) {
    newCoreSize = NextPowerOfTwo(newCoreSize);
    if (newCoreSize > sizeOfCore) {
        char *oldCore = core;

        sizeOfCore = newCoreSize;
        core = new char[sizeOfCore];
        endOfCore = core + sizeOfCore;
        *(endOfCore - 1) = 0;

        std::strcpy(core, oldCore);

        delete [] oldCore;
    }
}

// ------------------------------------------------------------------------
//
bool S4::Interpreter::Return() {
    int minimumHeight = 1;
    if (RSHeight() < minimumHeight) {
        printf("panic:\t%s %s %d\n\treturn stack is empty\n", __FILE__, __FUNCTION__, __LINE__);
        exit(2);
    }

    size_t returnOffset = RSPop();
    if (sizeOfCore < returnOffset) {
        printf("panic:\t%s %s %d\n\treturn stack is invalid (%ld/%ld)\n", __FILE__, __FUNCTION__, __LINE__, returnOffset, sizeOfCore);
        exit(2);
    }

    programCounter = core + returnOffset;

    return true;
}

// ------------------------------------------------------------------------
//
size_t S4::Interpreter::RSHeight() const {
    return returnStack.size();
}

// ------------------------------------------------------------------------
// RS: x -- { pop and return top element of stack }
//
size_t S4::Interpreter::RSPop() {
    if (!returnStack.size()) {
        printf("panic:\t%s %s %d\n\tpopping empty returnStack\n", __FILE__, __FUNCTION__, __LINE__);
        exit(2);
    }
    size_t coreOffset = returnStack.top();
    returnStack.pop();
    return coreOffset;
}

// ------------------------------------------------------------------------
// RS: -- x { push x on to stack }
//
void S4::Interpreter::RSPush(size_t coreOffset) {
    returnStack.push(coreOffset);
}

// ------------------------------------------------------------------------
bool S4::Interpreter::Step() {
    Lexeme l;
    if (!NextWord(&l)) {
        printf(".step:\tthinking it is end of input\n\t-->%s\n", programCounter);
        return false;
    }

    Word *word;

    switch (l.kind) {
        case Lexeme::endOfInput:
            printf(".step:\tendOfInput\n");
            return false;
        case Lexeme::integer:
            if (IsCompileMode()) {
                return true;
            }
            printf(".push:\tinteger %d\n", l.data.integer);
            dataStack.push(CellFromInteger(l.data.integer));
            return true;
        case Lexeme::number:
            if (IsCompileMode()) {
                return true;
            }
            printf(".push:\tnumber  %g\n", l.data.number);
            dataStack.push(CellFromNumber(l.data.number));
            return true;
        case Lexeme::symbol:
            printf(".eval:\tsymbol  %s\n", l.data.symbol);
            word = dictionary->Lookup(l.data.symbol);
            if (!word) {
                printf("error:\tundefined word '%s'\n", l.data.symbol);
                return false;
            }
            if (IsCompileMode()) {
                return word->Compile(this);
            }
            return word->Execute(this);
        case Lexeme::text:
            if (IsCompileMode()) {
                return true;
            }
            printf(".push:\ttext    %s\n", l.data.text);
            dataStack.push(CellFromText(l.data.text));
            return true;
    }

    printf("panic:\tunknown lexeme %d\n", l.kind);
    exit(2);

    // NOT REACHED
    return false;
}

// ------------------------------------------------------------------------
// x -- x { return top element of stack }
//
S4::Cell *S4::Interpreter::Top() const {
    if (!dataStack.size()) {
        printf("panic:\t%s %s %d\n\tpopping empty stack\n", __FILE__, __FUNCTION__, __LINE__);
        exit(2);
    }
    return dataStack.top();
}

