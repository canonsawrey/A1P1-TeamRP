#pragma once

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <regex>
#include "type.h"
#include "column.h"
#include "type_column.h"

using namespace std;

/**
 * Object representation of a .sor file
 * Parsing of .sor is done in the constructor of the class
 */
class Sor {
    
    /**
     * TODO_CANON TODO_TREVOR
     * We can probably eliminate all of these fields except columns
     */
    int totalColumns;
    vector<Type> columnTypes;
    vector<vector<string>> baseData;
    vector<Column*> columns;

public:

    /**
     * Constructor for Sor class
     */
    Sor(
        unsigned int from,
        unsigned int length,
        string filename
    ) {
        interpretSchema(filename, from, length);
        createColumns();
    }

    /**
      * Skips the fstream ahead from the start to "from" bytes in,
      * and then skips to the end of the line to eliminate partial start line.
      * @param fin The file stream that will be read in
      * @param from The number of bytes to skip forward
      */
    void skipTo(fstream &fin, unsigned int from) {
        unsigned int bytesRead = 0;
        char ch;
        while (bytesRead < from && fin >> noskipws >> ch) {
            bytesRead++;
        }
        while ((int)ch != 10 && fin >> noskipws >> ch) { }
    }

    /**
     * Parses the .sor file at filename, loading data in to 2D string vector `baseData`
     * @param filename .sor filename to be processed
     * @param from the number of bytes to skip forward
     * @param length Number of bytes to be read
     */
    void interpretSchema(string filename, unsigned int from, unsigned int length) {
        //Various fields to help parsing the file
        char ch;
        bool inField = false;
        bool inQuotes = false;
        unsigned int bytesRead = 0;
        int lineCount = 1;
        int ascii;
        int currentWidth = 0;
        int maxWidth = 0;
        string currentField = "";
        //Used to collect all the strings in a row. Once a newline is hit, the staging vector will be pushed to all of the columns
        vector<string> stagingVector;

        //create the filestream and skip to the proper location in the file
        fstream fin(filename, fstream::in);
        if (from != 0) {
            skipTo(fin, from);
        }

        // the linecount <= 500 check is said in the assignment "scan the whole file or first 500 lines"
        while (fin >> noskipws >> ch && lineCount <= 500 && bytesRead < length) {
            bytesRead++;
            ascii = (int)ch;
            switch (ascii) {
                //New line
                case 10:
                    if (!inQuotes) {
                        addRow(stagingVector, lineCount);
                        stagingVector.clear();
                        lineCount++;
                        inField = false;
                        if (currentWidth > maxWidth) {
                            maxWidth = currentWidth;
                        }
                        currentWidth = 0;
                    } else {
                        currentField = currentField + ch;
                    }
                    break;

                // < Character
                case 60:
                    if (!inQuotes) {
                        if (!inField) {
                            inField = true;
                            break;
                        }
                    } else {
                        currentField = currentField + ch;
                    }
                    break;

                // > Character
                case 62:
                    if (!inQuotes) {
                        if (inField) {
                            currentWidth++;
                            inField = false;
                            stagingVector.push_back(currentField);
                            currentField = "";
                        }
                    } else {
                        currentField = currentField + ch;
                    }
                    break;
                // " character
                case 34:
                    if (inField) {
                        if (!inQuotes) {
                            inQuotes = true;
                        } else {
                            inQuotes = false;
                        }
                        currentField = currentField + ch;
                    }
                    break;

                // Space character
                case 32:
                    if (inQuotes) {
                        currentField = currentField + ch;
                    }
                    break;

                default:
                    if (inField) {
                        currentField = currentField + ch;
                    }
                    break;
            }
        }
        totalColumns = maxWidth;
        //If end of file was reached with no newline, add the staging vector
        if (bytesRead < length) {
            addRow(stagingVector, lineCount);
        }
    }

    /**
     * Adds the staging vector to the baseData
     * @param stagingVector vector of strings representing a row of data to be added
     * @param row Row number, starting at 1
     */  
    void addRow(vector<string> stagingVector, int row) {
        for (int i = 1; i <= stagingVector.size(); i++) {
            string currentField = stagingVector[i - 1];
            Type fieldType = getFieldType(currentField);
            //update column type if needed
            updateColumnType(fieldType, i);
            //add data to a temporary 2d vector so that we dont have to read the file again
            addToBaseData(currentField, fieldType, i, row);
        }
    }

    /**
      * Builds Columns out of rawData and columnTypes vectors
      */
    void createColumns() {
        for (int i = 0; i < columnTypes.size(); i++) {
            vector<string> base = baseData[i];
            switch (columnTypes[i]) {
                case FLOAT: {
                    vector<float> trans;
                    trans.resize(base.size());
                    transform(base.begin(), base.end(), trans.begin(), parseFloat);
                    columns.push_back(new TypeColumn<float>(columnTypes[i], trans, base));
                    break;
                }
                case INT: {
                    vector<unsigned int> trans;
                    trans.resize(base.size());
                    transform(base.begin(), base.end(), trans.begin(), parseFloat);
                    columns.push_back(new TypeColumn<unsigned int>(columnTypes[i], trans, base));
                    break;
                }
                case STRING: {
                    columns.push_back(new TypeColumn<string>(columnTypes[i], base, base));
                    break;
                }
                case BOOL: {
                    vector<bool> trans;
                    trans.resize(base.size());
                    transform(base.begin(), base.end(), trans.begin(), parseBool);
                    columns.push_back(new TypeColumn<bool>(columnTypes[i], trans, base));
                    break;
                }
            }
        }
    }

    /**
     * Adds a string to the 2D vector of strings, representing the raw strings from the .sor file
     * @param data string to be added
     * @param currentWidth The column the data resides in (first column having value of 1, not 0)
     * @param lineCount The row the data resides in
     */
    void addToBaseData(string data, Type type, int currentWidth, int lineCount) {
        if (currentWidth > baseData.size()) {
            baseData.resize(currentWidth);
        }
        if (lineCount > baseData.at(currentWidth - 1).size()) {
            baseData.at(currentWidth - 1).resize(lineCount, "");
        }
        baseData.at(currentWidth - 1).at(lineCount - 1) = data;
    }

    /**
     * Updates the columnType vector is the incomingType is less restrictive than the currentType for the column at current width
     * I.e. STRING replaces FLOAT replaces INT replaces BOOL
     * @param incomingType The type that will be added to the column
     * @param currentWidth The index of the column (starting from 1, not0)
     */
    void updateColumnType(Type incomingType, int currentWidth) {
        if (currentWidth > columnTypes.size()) {
            columnTypes.resize(currentWidth, incomingType);
        }
        Type oldType = columnTypes.at(currentWidth - 1);
        if (shouldChangeType(oldType, incomingType)) {
            columnTypes.at(currentWidth - 1) = incomingType;
        }
    }

    /**
     * Relating to the previous function, updateColumnType. True if newType is less restrictive. False otherwise
     * @param oldType The original type of a columnn
     * @param newType The new type coming in
     * @return True if the newType should override/replace the old type
     */
    bool shouldChangeType(Type oldType, Type newType) {
        return newType > oldType;
    }

    /**
     * Determines the most restrictive type that can be applied to a string
     *  @param fieldValue The string to be evaluated
     *  @return The most restrictive type fieldValue can represent
     */
    Type getFieldType(string fieldValue) {
        trim(fieldValue);
        if (isBool(fieldValue)) return BOOL;
        if (isInt(fieldValue)) return INT;
        if (isFloat(fieldValue)) return FLOAT;
        if (isString(fieldValue)) return STRING;
        return BOOL;
    }

    /**
     * Prints the Sor. Useful for debugging.
     */
    void printTransposed() {
        cout << "\nTranspose:";
        for (Column* c: columns) {
            cout << "\n" << typeStr(c->getType()) << " ";
            c->print(20);
        }
        cout << "\n";
    }

    /**
     * Gets the string for the value at the given column and row
     * @param column Column index (starting at 0)
     * @param row Row index (starting at 0)
     * @return The value's string representation
     */
    string getStringValueAt(unsigned int column, unsigned int row) {
        if (column >= columns.size()) {
            throw "Column index out of bounds exception.";
        }
        Column* col = columns[column];
        return col->getStringValue(row);
    }

    /**
     * Prints the column type of the column at the specified index
     * @param column the index of the column (starting at 0)
     */
    void printColumnType(unsigned int column) {
        if (column <= columns.size() - 1) {
            Type type = columns[column]->getType();
            cout << typeStr(type) << endl;
        } else {
            throw "Invalid column. Program terminated.";
        }
    }

    /**
     * Prints the string for the value at the given column and row
     * @param column Column index (starting at 0)
     * @param row Row index (starting at 0)
     */
    void printValue(unsigned int col, unsigned int offset) {
        string value = getStringValueAt(col, offset);
        cout << value << endl;
    }

    /**
     * Prints 1 if the value of the given column and row is missing
     * @param column the index of the column (starting at 0)
     * @param row the index of the row (starting at 0)
     */
    void printIsMissing(unsigned int col, unsigned int row) {
        bool missing = 0;
        if (col <= baseData.size() - 1) {
            if (row > getMaxColumnHeight() - 1) {
                throw "Index out of bounds. Program terminated.";
            } else if (row <= baseData[col].size() - 1) {
                if (baseData[col][row] == "") {
                    missing = 1;
                }
            } else {
                missing = 1;
            }
        } else {
            throw "Index out of bounds. Program terminated.";
        }
        cout << missing << endl;
    }

    unsigned int getMaxColumnHeight() {
        unsigned int max = 0;
        for(int i = 0; i < columns.size() - 1; i++) {
            int height = columns[i]->length();
            if (height > max) max = height;
        }
        return max;
    }

    ~Sor() {
        //delete all columns allocated
        for (int i = 0; i < columns.size(); i++) {
            delete columns[i];
        }
    }
};