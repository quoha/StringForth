//
//  StringForth/Dictionary.cpp
//
//  Created by Michael Henderson on 4/27/14.
//  Copyright (c) 2014 Maloquacious Productions, LLC. All rights reserved.
//

#include "Dictionary.h"

// ------------------------------------------------------------------------
S4::Dictionary::Dictionary() {
    for (int idx = 0; idx < numberOfBuckets; idx++) {
        bucket[idx] = 0;
    }
}

// ------------------------------------------------------------------------
S4::Dictionary::~Dictionary() {
}

// ------------------------------------------------------------------------
void S4::Dictionary::Add(S4::Word *word) {
    Entry *entry           = new Entry;
    entry->hashValue       = Hash(word->Name());
    entry->word            = word;
    entry->left            = 0;
    entry->right           = 0;

    int bucketNumber       = entry->hashValue % numberOfBuckets;

    // link the new entry to the start of the chain
    //
    entry->right           = bucket[bucketNumber];
    if (entry->right) {
        entry->right->left = entry;
    }
    bucket[bucketNumber]   = entry;
}

// ------------------------------------------------------------------------
// delete entry from dictionary. unlink it from the chain that it is in.
// if it is the root of the chain, then reset the root to the next bucket
// in the chain.
//
bool S4::Dictionary::Delete(S4::Dictionary::Entry *entry) {
    if (entry) {
        int bucketNumber         = entry->hashValue % numberOfBuckets;

        // unlink the entry from the chain. if the entry is the first
        // in the chain, reset the bucket pointer, too
        //
        if (!entry->left) {
            // first entry in the chain
            bucket[bucketNumber] = entry->right;
        } else {
            entry->left->right   = entry->right;
        }

        entry->right->left       = entry->left;

        delete entry;
        
        return true;
    }

    return false;
}

// ------------------------------------------------------------------------
// look for first matching name in the dictionary. if found, delete the
// entry for it from the dictionary and return true. otherwise, return
// false.
//
bool S4::Dictionary::Delete(const char *name) {
    return Delete(Find(name));
}

// ------------------------------------------------------------------------
// delete word from dictionary. return true if word was found, false if it
// was not in the dictionary.
//
bool S4::Dictionary::Delete(S4::Word *word) {
    return Delete(Find(word));
}

// ------------------------------------------------------------------------
void S4::Dictionary::Dump(FILE *fp) {
    int minimumChain  = 0;
    int maximumChain  = 0;
    int numberUsed    = 0;
    int numberStored  = 0;
    int numberChained = 0;

    for (int idx = 0; idx < numberOfBuckets; idx++) {
        int chainLength = 0;

        Entry *entry = bucket[idx];

        if (entry) {
            numberUsed++;

            while (entry) {
                chainLength++;
                numberChained++;
                numberStored++;
                entry = entry->right;
            }

            if (idx == 0 || (chainLength && chainLength < minimumChain)) {
                minimumChain = chainLength;
            }
            if (chainLength > maximumChain) {
                maximumChain = chainLength;
            }
        }
    }
    fprintf(fp, "// ------------------------------------------------------------------------\n");
    fprintf(fp, ".dict: %-18s == %8d\n", "numberOfBuckets", numberOfBuckets);
    fprintf(fp, ".....: %-18s == %8d\n", "numberUsed"     , numberUsed     );
    fprintf(fp, ".....: %-18s == %8d\n", "numberStored"   , numberStored   );
    fprintf(fp, ".....: %-18s == %8d\n", "numberChained"  , numberChained  );
    fprintf(fp, ".....: %-18s == %8d\n", "minimumChain"   , minimumChain   );
    fprintf(fp, ".....: %-18s == %8d\n", "maximumChain"   , maximumChain   );

    if (numberUsed) {
        fprintf(fp, ".....: %-18s == %g\n", "averageChaining", (double)(numberChained)/(double)(numberUsed));
    }
}

// ------------------------------------------------------------------------
S4::Dictionary::Entry *S4::Dictionary::Find(const char *name) {
    unsigned int hashValue = Hash(name);
    int bucketNumber       = hashValue % numberOfBuckets;

    for (Entry *entry = bucket[bucketNumber]; entry; entry = entry->right) {
        if (entry->hashValue == hashValue && !strcmp(name, entry->word->Name())) {
            return entry;
        }
    }

    return 0;
}

// ------------------------------------------------------------------------
S4::Dictionary::Entry *S4::Dictionary::Find(S4::Word *word) {
    unsigned int hashValue = Hash(word->Name());
    int bucketNumber       = hashValue % numberOfBuckets;

    for (Entry *entry = bucket[bucketNumber]; entry; entry = entry->right) {
        if (entry->hashValue == hashValue && word == entry->word) {
            return entry;
        }
    }

    return 0;
}

// ------------------------------------------------------------------------
S4::Word *S4::Dictionary::Lookup(const char *name) {
    Entry *entry = Find(name);
    return entry ? entry->word : 0;
}
