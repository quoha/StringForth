//
//  StringForth/Dictionary.h
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
// ------------------------------------------------------------------------
// Dictionary holds executable words. This implementation allows the same
// name to be entered multiple times. Doing so "shadows" the first value
// so that it won't be found by later lookups.
//
// Can I implement this using std::unordered_map? I don't think so because
// I need to be able to shadow words and std::unorderd_map will replace
// the value for identical keys.
// ------------------------------------------------------------------------

#ifndef __StringForth__Dictionary__
#define __StringForth__Dictionary__

#include "StringForth.h"
#include "Word.h"
#include <stdio.h>

namespace S4 {

    class Dictionary {
        static const int numberOfBuckets = 1024;
        struct Entry {
            Entry        *left;
            Entry        *right;
            Word         *word;
            unsigned int  hashValue;
        } *bucket[numberOfBuckets];

        bool   Delete(Entry *bucket);
        Entry *Find(const char *name);
        Entry *Find(Word *word);

    public:
        Dictionary();
        ~Dictionary();

        void  Add(Word *word);
        bool  Delete(const char *name);
        bool  Delete(Word *word);
        void  Dump(FILE *fp);
        Word *Lookup(const char *name);
    }; // class Dictionary

} // namespace S4

#endif /* defined(__StringForth__Dictionary__) */
