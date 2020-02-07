#pragma once

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <regex>
#include "type.h"
#include "column.h"
#include "parse_state.h"

using namespace std;

/**
 * Object representation of a .sor file
 * Parsing of .sor is done in the constructor of the class
 */
class SorAdapter {
    // vector of our completed Columns
    vector<Column*> columns;

    public:
        /**
         * Constructor for SorAdapter class
         */
        SorAdapter(
            unsigned int from,
            unsigned int length,
            string filename
        ) {
            columns = vector<Column*>();
            processSor(filename, from, length);
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
            // skips to the end of the line to eliminate partial start line.
            while ((int)ch != 10 && fin >> noskipws >> ch) { }
        }

        /**
         * Parses the .sor file at filename, loading data into columns vector
         * @param filename .sor filename to be processed
         * @param from the number of bytes to skip forward
         * @param length Number of bytes to be read
         */
        void processSor(string filename, unsigned int from, unsigned int length) {
            // Used to keep track of the current state of parsing
            ParseState parseState = ParseState();
            // Used to collect all the data within a row. Once a newline is hit, 
            // the staging vector will be pushed to all of the columns
            vector<string> stagingVector;

            //create the filestream and skip to the proper location in the file
            fstream fin(filename, fstream::in);
            if (from != 0) {
                skipTo(fin, from);
            }

            // go character by character until the given length is reached
            while (fin >> noskipws >> parseState.ch && parseState.bytesRead < length) {
                parseState.bytesRead++;
                int ascii = (int)parseState.ch;
                switch (ascii) {
                    // New line Character
                    case 10:
                        handleNewLine(&parseState, &stagingVector);
                        break;
                    // < Character
                    case 60:
                        handleOpenTag(&parseState);
                        break;
                    // > Character
                    case 62:
                        handleCloseTag(&parseState, &stagingVector);
                        break;
                    // " Character
                    case 34:
                        handleQuote(&parseState);
                        break;
                    // Space character
                    case 32:
                        if (parseState.inQuotes) {
                            parseState.currentField = parseState.currentField + parseState.ch;
                        }
                        break;
                    default:
                        if (parseState.inField) {
                            parseState.currentField = parseState.currentField + parseState.ch;
                        }
                        break;
                }
            }
            //If end of file was reached with no newline, add the staging vector
            if (parseState.bytesRead < length) {
                writeData(stagingVector, parseState.lineCount);
            }
        }

        /**
         * Updates the given ParseState and stagingVector when a new line is encountered
         * @param parseState The given ParseState configuration
         * @param stagingVector The current stagingVector
         */
        void handleNewLine(ParseState* parseState, vector<string>* stagingVector) {
            if (!parseState->inQuotes) {
                // add the staging vector to columns
                writeData(*stagingVector, parseState->lineCount);
                // clear the stagingVector for the next line
                stagingVector->clear();
                parseState->lineCount++;
                parseState->inField = false;
                if (parseState->currentWidth > parseState->maxWidth) {
                    // update the max width of the parse state
                    parseState->maxWidth = parseState->currentWidth;
                }
                parseState->currentWidth = 0;
            } else {
                parseState->currentField = parseState->currentField + parseState->ch;
            }
        }

        /**
         * Updates the given ParseState when a `<` is encountered
         * @param parseState The given ParseState configuration
         */
        void handleOpenTag(ParseState* parseState) {
            if (parseState->inQuotes || parseState->inField) {
                parseState->currentField = parseState->currentField + parseState->ch;
            } else if (!parseState->inField) {
                // set the inField flag to true to indicate we are at the beginning of a new field
                parseState->inField = true;
            }
        }

        /**
         * Updates the given ParseState and stagingVector when a `>` is encountered
         * @param parseState The given ParseState configuration
         * @param stagingVector The current stagingVector
         */
        void handleCloseTag(ParseState* parseState, vector<string>* stagingVector) {
            if (!parseState->inQuotes) {
                if (parseState->inField) {
                    parseState->currentWidth++;
                    parseState->inField = false;
                    stagingVector->push_back(parseState->currentField);
                    parseState->currentField = "";
                }
            } else {
                parseState->currentField = parseState->currentField + parseState->ch;
            }
        }

        /**
         * Updates the given ParseState when a quote is encountered
         * @param parseState The given ParseState configuration
         */
        void handleQuote(ParseState* parseState) {
            if (parseState->inField) {
                parseState->inQuotes = !parseState->inQuotes;
                parseState->currentField = parseState->currentField + parseState->ch;
            }
        }

        /**
         * Adds the staging vector to the columns
         * @param stagingVector vector of strings representing a row of data to be added
         * @param row Row number, starting at 1
         */  
        void writeData(vector<string> stagingVector, int row) {
            for (int i = 0; i < stagingVector.size(); i++) {
                string currentField = stagingVector[i];
                if (i >= columns.size()) {
                    // add new columns if the row is longer than the current column count
                    columns.push_back(new Column());
                }
                // add the data contained within the field to columns
                columns[i]->addValue(new string(currentField), row);
            }
        }

        /**
         * Prints the Sor. Useful for debugging.
         */
        void printTransposed() {
            cout << "\nTranspose:\n";
            for (Column* c: columns) {
                c->print(10);
                cout << "\n";
            }
        }

        /**
         * Gets the string for the value at the given column and row
         * @param column Column index (starting at 0)
         * @param row Row index (starting at 0)
         * @return The value's string representation
         */
        string getValueAt(unsigned int column, unsigned int row) {
            if (column >= columns.size()) {
                fail("Column index out of bounds exception.");
            }
            string* str = columns[column]->getValue(row);
            if (str == nullptr) {
                return "";
            } else {
                return *str;
            }
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
                fail("Invalid column. Program terminated.");
            }
        }

        /**
         * Prints the string for the value at the given column and row
         * @param col Column index (starting at 0)
         * @param offset Row index (starting at 0)
         */
        void printValue(unsigned int col, unsigned int offset) {
            string value = getValueAt(col, offset);
            if (columns[col]->getType() == STRING) {
                value = "\"" + value + "\"";
            }
            cout << value << endl;
        }

        /**
         * Prints 1 if the value of the given column and row is missing
         * @param col the index of the column (starting at 0)
         * @param row the index of the row (starting at 0)
         */
        void printIsMissing(unsigned int col, unsigned int row) {
            bool missing = 0;
            if (col <= columns.size() - 1) {
                if (row > getMaxColumnHeight() - 1) {
                    fail("Index out of bounds. Program terminated.");
                } else if (row <= columns[col]->values.size() - 1) {
                    if (columns[col]->getValue(row) == nullptr) {
                        missing = 1;
                    }
                } else {
                    missing = 1;
                }
            } else {
                fail("Index out of bounds. Program terminated.");
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

        ~SorAdapter() {
            //delete all columns allocated
            for (int i = 0; i < columns.size(); i++) {
                delete columns[i];
            }
        }
};