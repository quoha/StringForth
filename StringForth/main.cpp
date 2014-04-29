//
//  StringForth/main.cpp
//
//  Created by Michael Henderson on 4/23/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#include "StringForth.h"

// ------------------------------------------------------------------------
const char *program = "0 dup . . 1 2 3 stackHeight . + + . 'hello' { 'hello, world!' . } 'goodbye' { 'say goodnight' . } hello goodbye stackHeight .";

// ------------------------------------------------------------------------
int main(int argc, const char * argv[]) {
    S4::Interpreter *interpreter = new S4::Interpreter(program);

    while (interpreter->Step()) {
        ;
    }

    delete interpreter;;

    return 0;
}
