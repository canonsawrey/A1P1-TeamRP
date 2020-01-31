#pragma once
#include "type.h"
#include "column.h"

/**
 * Child to Column class. Represents a Column of SOR data, with both the raw string and typed data.
 */
template <class T>
class TypeColumn : public Column {
    public:
        vector<T> values;
        vector<string> stringValues;

        /**
         * Constructor for TypeColumn
         */
        TypeColumn(Type type, vector<T> data, vector<string> stringData) : Column(type) {
            values = data;
            stringValues = stringData;
        }

        /**
         * Returns the data with correct type at the specified row
         * @param row index of the data (starting at 0)
         */
        T getValue(int row) {
            if (row > values.size() - 1) {
                throw "Index out of bound exception.";
            } else {
                return values[row];
            }
        }

        /**
         * Returns the data's string representation at the specified row
         * @param row index of the data (starting at 0)
         */
        string getStringValue(int row) {
            if (row > stringValues.size() - 1) {
                throw "Index out of bound exception.";
            } else {
                string data = stringValues[row];
                if (columnType == STRING) {
                    data = "\"" + data + "\"";
                }
                return data;
            }
        }

        /**
         * @return the column's Type
         */
        Type getType() {
            return columnType;
        }

        /**
         * @return The length of the column / number of rows
         */
        int length() {
            return values.size();
        }

        /**
         * Prints a string representation of the column. Useful for debugging
         * @param spacing The length of each value of the column. Ensures all rows line up when printing multiple rows
         */
        void print(int spacing) {
            for (string str: stringValues) {
                string printString;
                if (str.length() > spacing) {
                    printString = str.substr(0, spacing);
                } else {
                    printString = str;
                    while (printString.length() < spacing) {
                        printString.append(" ");
                    }
                }
                cout << printString << " ";
            }
        }

        ~TypeColumn() { }
};