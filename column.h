#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include "type.h"
#include "helper.h"

using namespace std;

/**
 * Class to be used as a parent to TypeColumn
 * C++ doesnt allow vector of generic class, so parent is required
 */
class Column {
    public:
        Type columnType;

        Column() { }

        Column(Type type) {
            columnType = type;
        }

        virtual Type getType() {
            return columnType;
        }

        virtual int length() { 
            throw "Can't return length of parent type `Column`.";
        }
        
        virtual void print(int spacing) { 
            throw "Can't print parent type `Column`.";
        }

        virtual string getStringValue(int row) { 
            throw "No string value for parent type `Column`.";
        }

        ~Column() { }
};