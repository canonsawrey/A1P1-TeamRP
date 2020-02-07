#pragma once
#include <string>
#include <regex>
#include "type.h"

using namespace std;

/**
 * Exits the program with a message
 * @param message The string that will be printed to the console
 */
void fail(string message = "") {
    cout << message << endl;
    exit(-1);
}

/**
 * Returns if the string only contains number characters
 * @param s The string to be evaluated
 * @return bool if s only contains digits
 */
bool has_only_digits(const string s){
    return s.find_first_not_of( "0123456789" ) == string::npos;
}

/**
 * Returns if the string is an integer
 * @param s The string to be evaluated
 * @return if s is an integer
 */
bool isInt(string value) {
    if (value.find("\"")  != string::npos) {
        return false;
    } else {
        string intPattern(R"(^(?: *)(?:\+|\-|)(\d+))");
        regex intRegex(intPattern);
        return regex_match(value, intRegex);
    }
}

/**
 * Convenience wrapper around stoi, returns an integer from a string
 * @param s The string to be parsed
 * @return the parsed integer
 */
int parseInt(string value) {
    if (value == "") {
        return NULL;
    } else {
        return stoi(value);
    }
}

/**
 * Is the string is a float
 * @param s The string to be evaluated
 * @return bool if the string is a float
 */
bool isFloat(string value) {
    if (value.find("\"")  != string::npos) {
        return false;
    } else {
        string floatPattern(R"(^(?: *)(?:\+|\-|)(\d+).(\d+))");
        regex floatRegex(floatPattern);
        return regex_match(value, floatRegex);
    }
}

/**
 * Convenience wrapper around stof, returns a float from a string
 * @param s The string to be parsed
 * @return parsed float
 */
float parseFloat(string value) {
    if (value == "") {
        return NULL;
    } else {
        return stof(value);
    }
}

/**
 * Returns if the string is a 0 or 1, defined to be a bool by the assignment 1 spec
 * @param s The string to be evaluated
 * @return bool: if string is 1 or 0
 */
bool isBool(string value) {
    if (value.find("\"")  != string::npos) {
        return false;
    } else {
        return value == "1" || value == "0";
    }
}

/**
 * Returns bool from given string
 * @param s The string to be parsed
 * @return bool from string
 */
bool parseBool(string value) {
    if (value == "") {
        return NULL;
    } else if (value == "1") {
        return 1;
    } else if (value == "0") {
        return 0;
    } else {
        fail("Unable to parse boolean from string: " + value);
    }
}

/**
 * Returns if a given value is a string
 * @param value The string to be evaluated
 * @return bool from string
 */
bool isString(string value) {
    if (value == "") return true;
    //regex used when in quotes
    regex quotedRegex("^(\")(.*)(\")");
    //regex used when no quotes present
    regex nonQuotedRegex("^ *[^ ]+ *$");
    if (value.find("\"") != string::npos) {
      return regex_match(value, quotedRegex);
    }
    return regex_match(value, nonQuotedRegex);
}

/**
 * Returns a string representation of the given Type
 * @param t The type to be evaluated
 * @return string representation of t
 */
string typeStr(Type t) {
    switch (t) {
        case INT:
            return "INT";
            break;
        case STRING:
            return "STRING";
            break;
        case FLOAT:
            return "FLOAT";
            break;
        case BOOL:
            return "BOOL";
            break;
    }
    return "";
}

/**
 * Relating to the previous function, updateColumnType. True if newType is less restrictive. False otherwise
 * @param oldType The original type of a columnn
 * @param newType The new type coming in
 * @return True if the newType should override/replace the old type, false otherwise
 */
bool shouldChangeType(Type oldType, Type newType) {
    return newType > oldType;
}

/**
 * Trims quotes from a string
 * @param s the string to be trimmed 
 */
void trimQuotes(string* s) {
    if (s == nullptr || s->length() < 2) {
        return;
    }
    string first = s->substr(0, 1);
    string last = s->substr(s->length() - 1, s->length());
    if (first == "\"" && last == "\"") {
        *s = s->substr(1, s->length() - 2);
    }
}

/**
 * Trims whitespace at the beginning of a string
 * @param s The string to be trimmed
 * @return string with whitespace trimmed on the left
 */
static inline string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
    return s;
}

/**
 * Trims whitespace at the end of a string
 * @param s The string to be trimmed
 * @return string with whitespace trimmed on the right
 */
static inline string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

/**
 * Trims whitespace on both sides of a string
 * @param s The string to be trimmed
 * @return string with whitespace trimmed on both sides
 * Credit: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
 */
static inline string &trim(string &s) {
    return ltrim(rtrim(s));
}

/**
 * Pads or slices the input string to the input size
 * @param s The string to be padded of shortened
 * @param l The desired size
 */
static string padString(string s, size_t l) {
    if (s.length() > l) {
        s = s.substr(0, l);
    } else {
        while (s.length() < l) {
            s.append(" ");
        }
    }
    return s;
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