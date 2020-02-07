#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include "type.h"
#include "helper.h"

using namespace std;

/**
 * Represents a single Column of data
 */
class Column {
    public:
        Type type;
        vector<string*> values;

        Column() { 
            // initialize type to BOOL as it is the most restrictive type
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

        /**
         * Adds a given value to the given row in this Column
         * @param currentField the data to be added to the Column
         * @param row the row to insert the data at
         */
        void addValue(string* currentField, int row) {
            // only care about updating the schema when we are within the first 500 lines
            if (row <= 500) {
                Type fieldType = getFieldType(*currentField);
                //update column type if needed
                updateColumnType(fieldType);
            }
            trimQuotes(currentField);
            if (row == values.size()) {
                trimQuotes(currentField);
                values.push_back(currentField);
            } else if (row > values.size()) {
                values.resize(row + 1);
                values.at(row) = currentField;
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
            if (row > values.size() - 1) {
                fail("Index out of bound exception. Program terminated.");
            } else {
                string* ptr = values[row];
                return values[row];
            }
        }

        /**
         * Updates the type of this Column if the incomingType is less restrictive than the currentType
         * I.e. STRING replaces FLOAT replaces INT replaces BOOL
         * @param incomingType The type that may replace this column's current Type
         */
        void updateColumnType(Type incomingType) {
            // check if the old column Type should be updated
            if (shouldChangeType(type, incomingType)) {
                type = incomingType;
            }
        }
        
        /**
         * Prints a string representation of the column. Useful for debugging
         * @param spacing The length of each value of the column. Ensures all rows line up when printing multiple rows
         */
        void print(int spacing) {
            cout << padString(typeStr(type), 10);
            for (string* str: values) {
                string printString;
                if (str == nullptr) {
                    printString = "[EMPTY]";
                } else {
                    printString = *str;
                }
                cout << "| " << padString(printString, spacing) << " |";
            }
        }
        
        ~Column() {
            for (size_t i = 0; i < values.size(); i++) {
                delete values[i];
            }
        }
};