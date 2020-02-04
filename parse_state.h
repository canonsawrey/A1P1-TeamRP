#pragma once

struct ParseState {
    public:
        char ch;
        bool inField = false;
        bool inQuotes = false;
        unsigned int bytesRead = 0;
        int lineCount = 1;
        int currentWidth = 0;
        int maxWidth = 0;
        string currentField = "";
};