#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "rustic-c-compiler.h"
using namespace std;

// パソコンいいこと！
// Can someone help me with this code?

pools::charpool lex(std::string codetobelexed) {

    // The conversion of a stream of characters to a stream of meaningful tokens

    // Create object for accessing pools
    pools::charpool charpool;

    int iterator = 0;       // Iterator for scanning and/or counting
    int wordindex = 0;      // Iterator for scanning words
    int characterindex = 0; // Iterator for scanning characters
    
    // Create parser for dividing string into keywords and storing them in an array.

    // While codetobelexed[iterator] doesn't equal nullterminate character ('\0')
    for (iterator = 0; codetobelexed[iterator] != '\0'; iterator++)
    {
        // Check for space bar, if we get space bar, store it in charpool.
        if (codetobelexed[iterator] == ' '
            || codetobelexed[iterator] == '#'
            || codetobelexed[iterator] == '('
            || codetobelexed[iterator] == ')'
            || codetobelexed[iterator] == '<'
            || codetobelexed[iterator] == '>'
            || codetobelexed[iterator] == '{'
            || codetobelexed[iterator] == '}'
            || codetobelexed[iterator] == '['
            || codetobelexed[iterator] == ']'
            || codetobelexed[iterator] == ';'
            || codetobelexed[iterator] == ','
            || codetobelexed[iterator] == '\n'
            || codetobelexed[iterator] == '\t')
            {
                characterindex = 0;
                charpool.charpool[wordindex][characterindex] = codetobelexed[iterator];
                characterindex = 0;
                if (codetobelexed[iterator+1] == codetobelexed[iterator]) {
                    for (int characteriterator = 0; codetobelexed[characteriterator] == codetobelexed[iterator+1]; characteriterator++) {
                        charpool.charpool[wordindex][characterindex] = codetobelexed[iterator+1];
                    }
                }
                wordindex++;
                characterindex = 0;
            } 
            else 
            {   
                charpool.charpool[wordindex][characterindex] = codetobelexed[iterator];
                if (codetobelexed[iterator+1] == ' '
                    || codetobelexed[iterator+1] == '#'
                    || codetobelexed[iterator+1] == '('
                    || codetobelexed[iterator+1] == ')'
                    || codetobelexed[iterator+1] == '<'
                    || codetobelexed[iterator+1] == '>'
                    || codetobelexed[iterator+1] == '{'
                    || codetobelexed[iterator+1] == '}'
                    || codetobelexed[iterator+1] == '('
                    || codetobelexed[iterator+1] == ')'
                    || codetobelexed[iterator+1] == ';'
                    || codetobelexed[iterator+1] == ','
                    || codetobelexed[iterator+1] == '\n'
                    || codetobelexed[iterator+1] == '\t') {
                        wordindex++;
                }
                else 
                {
                    characterindex++;
                }
            }
    }
    return charpool;
}

pools::keywordpool parse(pools::charpool lexedobject) {

    // A computer program that breaks down text into recognized strings of characters for further analysis.

    // Create object for variable pool
    pools::keywordpool keywordPool;

    // Create variables for iterating...
    int iterator = 0;        // Iterator for loops, counts the cycles
    int wordindex = 0;        // Word index iterator
    int worditerator = 0;        // Word index iterator #2 (fixed the whole program)
    int characterindex = 0;        // Character index iterator

    // Counter for words in charpool 
    int wordcounter = 0;

    // Count all words in charpool
    // For as long as charpool[iterator][0] != "\0", wordcounter++ and iterator++;
    for (int iterator = 0; lexedobject.charpool[iterator][0] != "\0"; iterator++) {
        wordcounter++;
    }

    // For all words in wordlist; do for loop with clause (lexedobject.charpool[wordindex][worditerator] != "\0")
    // and set the keys, as normal. then, when we get "\0"
    for (iterator = 0; iterator <= wordcounter; iterator++) {

        // Count the length of every word with characterindex, until ntchar2 found...
        // and set the keys, this sets the integer key perfectly, next we need to check for space...
        for (worditerator = 0; lexedobject.charpool[wordindex][worditerator] != "\0"; worditerator++) {
            keywordPool.keywordpool[wordindex][0] += lexedobject.charpool[wordindex][characterindex];
            characterindex++;
        }

        // To check for the space bar we need to increment wordindex by 1
        wordindex++;
        characterindex = 0;

        // If we get a space, wordindex++ and reset characterindex and go for the next word
        if (lexedobject.charpool[wordindex][characterindex] == " ") {
            // Here we can decide if we want to include spaces or not
            keywordPool.keywordpool[wordindex][0] = " "; // Comment out this line to not include spaces
            // Increment wordindex, for getting the next word since space is only 1-letter.
            wordindex++;
            // After setting the key, just reset characterindex for the next character.
            characterindex = 0;
        }
    }

    return keywordPool;
}

pools::compiledobject compile(pools::keywordpool parsedobject) {

    // Count the number of arrays/words in struct
    int wordindex = 0;   // Create variable for iterating through wordindexes

    // Define combiledobj object
    pools::compiledobject compiledobj;

    // For as long as keywordpool[wordindex][0] != dk_ntchar
    // check if data types found and convert them
    for (int iterator = 0; parsedobject.keywordpool[iterator][0] != "\0"; iterator++) {

        if (parsedobject.keywordpool[iterator][0] == "integer") {
            parsedobject.keywordpool[iterator][0] = "int";
        }

        if (parsedobject.keywordpool[iterator][0] == "decimal") {
            parsedobject.keywordpool[iterator][0] = "float";
        }
    }

    // combine the keywords into an compiledpool array
    for (int iterator = 0; parsedobject.keywordpool[iterator][0] != "\0"; iterator++) {
        compiledobj.compiledstring[0][0] += parsedobject.keywordpool[iterator][0];
    }

    // Return the compiled object
    return compiledobj;
}

int main(int argc, char* argv[]) {

    std::string filename;

    std::cout << "Enter .rc source code filename: ";
    cin >> filename;

    std::fstream rusticcfile(filename);

    // Read file
    std::string rusticcline;
    std::string linearray;

    int linecount = 0;

    while (getline(rusticcfile, rusticcline)) {
        linecount++;
    }

    rusticcfile.clear();
    rusticcfile.seekg(0);

    for (int iterator = 0; iterator <= linecount; iterator++) {
        if (iterator == linecount-1) {
            getline(rusticcfile, rusticcline);
            linearray += rusticcline;
        }
        else if (iterator < linecount) {
            getline(rusticcfile, rusticcline);
            linearray += rusticcline + "\n";
        }
    }
    // std::cout << linearray[0] << std::endl;

    // Pass the source to the parsestring function and return parsed object
    pools::charpool lexedobject = lex(linearray);

    // Pass the parsed object
    pools::keywordpool parsedobject = parse(lexedobject);

    // Compiler
    pools::compiledobject compiledobj = compile(parsedobject);

    // Create file
    std::ofstream cppfile("defaultoutput.cpp");

    // Print writing to file
    std::cout << "Writing to: defaultoutput.cpp..." << std::endl;

    // Write to file
    cppfile << compiledobj.compiledstring[0][0];

    // Close the file
    cppfile.close();

    // Pause
    system("pause");

    // return the exit code and exit program 
    return 0;
}