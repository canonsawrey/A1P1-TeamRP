#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "sor.h"
#include "helper.h"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <string>

TEST_CASE("Helpers", "[helper]") {
 
    SECTION("has_only_digits") {
        REQUIRE(has_only_digits("") == true);
        REQUIRE(has_only_digits("0") == true);
        REQUIRE(has_only_digits("123") == true);
        REQUIRE(has_only_digits("12a3") == false);
        REQUIRE(has_only_digits("abc") == false);
        REQUIRE(has_only_digits("6874a") == false);
    }

    SECTION("isInt") {
        REQUIRE(isInt("") == false);
        REQUIRE(isInt("0") == true);
        REQUIRE(isInt("123") == true);
        REQUIRE(isInt("+123") == true);
        REQUIRE(isInt("-123") == true);
        REQUIRE(isInt("1.23") == false);
        REQUIRE(isInt("hello") == false);
    }

    SECTION("isFloat") {
        REQUIRE(isFloat("") == false);
        REQUIRE(isFloat("1") == false);
        REQUIRE(isFloat("1.2") == true);
        REQUIRE(isFloat("+1.2") == true);
        REQUIRE(isFloat("-1.2") == true);
        REQUIRE(isFloat("0.23") == true);
    }

    SECTION("isBool") {
        REQUIRE(isBool("") == false);
        REQUIRE(isBool("\"1\"") == false);
        REQUIRE(isBool("1") == true);
        REQUIRE(isBool("0") == true);
    }

    SECTION("isString") {
        REQUIRE(isString("") == true);
        REQUIRE(isString("\"hello") == false);
        REQUIRE(isString("hello") == true);
        REQUIRE(isString("\"hello\"") == true);
        REQUIRE(isString("wow hi!") == false);
    }

    SECTION("trim") {
        string s1 = "  hello  ";
        string s2 = "bye";
        trim(s1);
        trim(s2);
        REQUIRE(s1 == "hello");
        REQUIRE(s2 == "bye");
    }

    SECTION("trimQuotes") {
        string* s = new string("\"hello\"");
        REQUIRE(*s == "\"hello\"");
        trimQuotes(s);
        REQUIRE(*s == "hello");
    }
}

TEST_CASE("Column", "[column]") {
    Column col = Column();
    Column col2 = Column();

    SECTION("getType, addValue, length, getValue 1") {
        REQUIRE(typeStr(col.getType()) == "BOOL");
        REQUIRE(typeStr(col2.getType()) == "BOOL");
        REQUIRE(col.length() == 0);
    }

    col.addValue(new string("1"), 0);
    col2.addValue(new string("123.1"), 0);

    SECTION("getType, addValue, length, getValue 2") {
        REQUIRE(typeStr(col.getType()) == "BOOL");
        REQUIRE(typeStr(col2.getType()) == "FLOAT");
        REQUIRE(col.length() == 1);
        REQUIRE(*col.getValue(0) == "1");
        REQUIRE(*col2.getValue(0) == "123.1");
    }
    
    col.addValue(new string("0"), 1);
    col2.addValue(new string("Hello"), 1);

    SECTION("getType, addValue, length, getValue 3") {
        REQUIRE(typeStr(col.getType()) == "BOOL");
        REQUIRE(typeStr(col2.getType()) == "STRING");
        REQUIRE(col.length() == 2);
        REQUIRE(*col.getValue(1) == "0");
        REQUIRE(*col2.getValue(1) == "Hello");
    }

    SECTION("updateColumnType") {
        REQUIRE(typeStr(col.getType()) == "BOOL");
        col.updateColumnType(INT);
        REQUIRE(typeStr(col.getType()) == "INT");
        col.updateColumnType(FLOAT);
        REQUIRE(typeStr(col.getType()) == "FLOAT");
        col.updateColumnType(INT);
        REQUIRE(typeStr(col.getType()) == "FLOAT");

    }
}

TEST_CASE("SorAdapter value retrieval", "[sor]") {
    SorAdapter * adapter = new SorAdapter(0, 1000000, "test.sor");
    SorAdapter * adapter2 = new SorAdapter(1, 1000000, "test.sor");

    SECTION("getValueAt") {
        REQUIRE(adapter->getValueAt(0, 0) == "1");
        REQUIRE(adapter->getValueAt(2, 2) == "432.12");
        REQUIRE(adapter->getValueAt(0, 6) == "");
        REQUIRE(adapter->getValueAt(0, 13) == "<<<>");
        REQUIRE(adapter->getValueAt(0, 5) == "1+1");
        REQUIRE(adapter->getValueAt(2, 2) == "432.12");
        REQUIRE(adapter->getValueAt(2, 4) == "0.65234");
        REQUIRE(adapter->getValueAt(1, 13) == "");
        REQUIRE(adapter->getValueAt(0, 10) == "-\"\"-");
    }

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 14);
    }

    SECTION("getValueAt") {
        REQUIRE(adapter2->getValueAt(0, 0) == "3");
        REQUIRE(adapter2->getValueAt(2, 2) == "8.1");
        REQUIRE(adapter2->getValueAt(0, 6) == "");
        REQUIRE(adapter2->getValueAt(0, 12) == "<<<>");
        REQUIRE(adapter2->getValueAt(0, 5) == "");
    }

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter2->getMaxColumnHeight() == 13);
    }
}

TEST_CASE("SorAdapter write data", "[sor]") {
    SorAdapter * adapter = new SorAdapter(0, 1000000, "test.sor");

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 14);
    }
    SECTION("getColumnTypes") {
        REQUIRE(typeStr(adapter->columns[2]->getType()) == "FLOAT");
        REQUIRE(typeStr(adapter->columns[3]->getType()) == "BOOL");
    }
    
    //Prepare and add new data to be added
    vector<string>* stagingVector = new vector<string>();
    stagingVector->push_back("");
    stagingVector->push_back("");
    stagingVector->push_back("421asd");
    stagingVector->push_back("41");
    adapter->writeData(*stagingVector, 14);

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 15);
    }
    SECTION("getColumnTypes") {
        REQUIRE(typeStr(adapter->columns[2]->getType()) == "STRING");
        REQUIRE(typeStr(adapter->columns[3]->getType()) == "INT");
    }
}