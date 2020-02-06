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
        Type type;
        vector<string*> values;

        Column() { 
            type = BOOL;
        }

        Column(Type type) {
            type = type;
        }

        /**
         * @return the column's Type
         */
        Type getType() {
            return type;
        }

        void addValue(string* currentField, int row) {
            //cout << "Adding value: " << *currentField << " At row: " << row << endl;
            if (row == values.size()) {
                values.push_back(currentField);
            } else if (row > values.size()) {
                vector<string*> replacement = vector<string*>(row + 1);
                for(int i = 0; i < values.size(); i++) {
                    replacement[i] = values[i];
                }
                replacement.at(row) = currentField;
                values = replacement;
            }
        }
        
        /**
         * @return the column's length
         */
        int length() { 
            return values.size();
        }

        /**
         * Returns the data with correct type at the specified row
         * @param row index of the data (starting at 0)
         */
        string* getValue(int row) { 
            //cout << "in getvalue with row:" << row << "\n";
            if (row > values.size() - 1) {
                throw "Index out of bound exception.";
            } else {
                string* ptr = values[row];
                return values[row];
            }
        }
        
        /**
         * Prints a string representation of the column. Useful for debugging
         * @param spacing The length of each value of the column. Ensures all rows line up when printing multiple rows
         */
        void print(int spacing) {
            cout << "in print";
            for (string* str: values) {
                string printString;
                if (str->length() > spacing) {
                    printString = str->substr(0, spacing);
                } else {
                    printString = *str;
                    while (printString.length() < spacing) {
                        printString.append(" ");
                    }
                }
                cout << printString << " |";
            }
        }
        
        ~Column() {
            for (size_t i = 0; i < values.size(); i++) {
                delete values[i];
            }
        }
};