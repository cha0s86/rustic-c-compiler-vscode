#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// パソコンいいこと！
// Can someone share this code to bisqwit for him to read?

union pools {

    struct dfkeys {
    public:
        char dk_space = ' ';
        std::string dk_space2 = " ";
        char dk_nullterminate = '\0';
        std::string dk_nullterminate2 = "\0";
        char dk_colon = ',';
        std::string dk_colon2 = ",";
        char dk_semicolon = ';';
        std::string dk_semicolon2 = ";";
        char dk_newline = '\n';
        std::string dk_newline2 = "\n";
        char dk_tab = '\t';
        std::string dk_tab2 = "\t";
        char dk_openingparenthesis = '(';
        char dk_closingparenthesis = ')';
        char dk_openingbracket = '{';
        char dk_closingbracket = '}';
        char dk_openingsquarebracket = '{';
        char dk_closingsquarebracket = '}';
    };

    struct charpools {
    public:
        std::string charpool[64][64];
    };

    struct keywordpools {
    public:
        std::string keywordpool[64][64];
        int codelength;
    };

    struct compiledpool {
    public:
        std::string compiledpool[64][64];
        std::string compiledstring[64][64];
    };
};

pools::charpools parsestring(std::string codetobeparsed) {

    // Parser / Lexical analyzer
    // Parser: a computer program that breaks down text into recognized strings of characters for further analysis.

    // Create object for accessing pools
    pools::charpools charPool;

    pools::dfkeys dfkeys;

    int iterator = 0;       // Iterator for scanning and/or counting
    int wordindex = 0;      // Iterator for scanning words
    int characterindex = 0; // Iterator for scanning characters

    // Create parser for dividing string into keywords and storing them in an array.

    // While codetobeparsed[iterator] doesn't equal nullterminate character ('\0')
    while (codetobeparsed[iterator] != dfkeys.dk_nullterminate)
    {
        // Check for space bar, if we get space bar, store it in charpool.
        if (codetobeparsed[iterator] == dfkeys.dk_space)
        {
            // Checkpoint! We know how to get a word!, now if we get space key, we store it and wordindex++! and do the keyscanning once again...
            // characterindex = 0;
            if (codetobeparsed[iterator - 1] == ' ') {
                // charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                break;
            }

            // wordindex++;
            // charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
            if (codetobeparsed[iterator - 1] == ')') {

            }
            else {
                wordindex++;
            }

            // if previous character is special symbol
            if (codetobeparsed[iterator] == ')') {
                characterindex = 0;
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
            }
            else {
                // wordindex++;
            }
            // increment the character index after skipping space so, that when we scan the next char we dont have the space.
            characterindex = 0; // Reset this to characterindex = 0
            // Iterate as long as the char is not null terminate character or space and not at the EOF and set the chars..
            while (codetobeparsed[iterator] != dfkeys.dk_nullterminate && codetobeparsed[iterator] != dfkeys.dk_space)
            {
                // Set the keys
                characterindex = 0;
                iterator++;
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                characterindex = 0;
            }
        }
        // Continue if we don't have space
        if (codetobeparsed[iterator] != dfkeys.dk_nullterminate)
        {
            // If we get semicolon, store it in charpool.
            switch (codetobeparsed[iterator]) {
            case '(':
                characterindex = 0;
                wordindex++;
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case ')':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case '{':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case '}':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case '[':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case ']':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case ';':
                // We encountered a semicolon. store it in the array after wordindex++
                wordindex++;
                characterindex = 0;
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                break;
            case ',':
                // We encountered a semicolon. store it in the array after wordindex++
                wordindex++;
                characterindex = 0;
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                break;
            case '\n':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case '\t':
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                characterindex = 0;
                break;
            case ' ':
                // wordindex++;
                characterindex = 0;
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                wordindex++;
                break;
            default:
                // If the previous checks did not complete, go to the next character in the array and set the key
                // just set the character as usual.
                charPool.charpool[wordindex][characterindex] = codetobeparsed[iterator];
                characterindex++;
                break;
            }
        }

        // Increment the iterator to scan the next word.
        iterator++;
    }

    return charPool;
}

pools::keywordpools lexobject(pools::charpools parsedobject, std::string rusticcline) {

    // Lexer: categorize keywords and define them as types, identifiers, names, operators, values and special symbols.

    // Create object for variable pool
    pools::keywordpools keywordPool;

    // Define definedkeys object
    pools::dfkeys dfkeys;

    // Create variables for iterating...
    int iterator = 0;        // Iterator for loops, counts the cycles
    int wordindex = 0;        // Word index iterator
    int worditerator = 0;        // Word index iterator #2 (fixed the whole program)
    int characterindex = 0;        // Character index iterator

    // Variable for string length
    int stringlength = rusticcline.length();

    // Define variable for code length
    keywordPool.codelength = stringlength;

    // Counter for words in charpool 
    int wordcounter = 0;

    // Count all words in charpool
    // For as long as charpool[iterator][0] != "\0", wordcounter++ and iterator++;
    for (int iterator = 0; parsedobject.charpool[iterator][0] != dfkeys.dk_nullterminate2; iterator++) {
        wordcounter++;
    }

    // For all words in wordlist; do for loop with clause (parsedobject.charpool[wordindex][worditerator] != "\0")
    // and set the keys, as normal. then, when we get "\0"
    for (iterator = 0; iterator <= wordcounter; iterator++) {

        // Count the length of every word with characterindex, until ntchar2 found...
        // and set the keys, this sets the integer key perfectly, next we need to check for space...
        for (worditerator = 0; parsedobject.charpool[wordindex][worditerator] != dfkeys.dk_nullterminate2; worditerator++) {
            keywordPool.keywordpool[wordindex][0] += parsedobject.charpool[wordindex][characterindex];
            characterindex++;
        }

        // To check for the space bar we need to increment wordindex by 1
        wordindex++;
        characterindex = 0;

        // If we get a space, wordindex++ and reset characterindex and go for the next word
        if (parsedobject.charpool[wordindex][characterindex] == dfkeys.dk_space2) {
            // Here we can decide if we want to include spaces or not
            keywordPool.keywordpool[wordindex][0] = dfkeys.dk_space2; // Comment out this line to not include spaces
            // Increment wordindex, for getting the next word since space is only 1-letter.
            wordindex++;
            // After setting the key, just reset characterindex for the next character.
            characterindex = 0;
        }
    }

    return keywordPool;
}

pools::compiledpool compile(pools::keywordpools lexedobject) {

    // Count the number of arrays/words in struct
    int wordindex = 0;   // Create variable for iterating through wordindexes

    // Define combiledobj object
    pools::compiledpool compiledobj;

    // Define defined keys object
    pools::dfkeys dfkeys;

    // For as long as keywordpool[wordindex][0] != dk_ntchar
    // check if data types found and convert them
    for (int iterator = 0; lexedobject.keywordpool[iterator][0] != "\0"; iterator++) {

        if (lexedobject.keywordpool[iterator][0] == "integer") {
            lexedobject.keywordpool[iterator][0] = "int";
        }

        if (lexedobject.keywordpool[iterator][0] == "decimal") {
            lexedobject.keywordpool[iterator][0] = "float";
        }
    }

    // combine the keywords into an compiledpool array
    for (int iterator = 0; lexedobject.keywordpool[iterator][0] != dfkeys.dk_nullterminate2; iterator++) {
        compiledobj.compiledpool[iterator][0] += lexedobject.keywordpool[iterator][0];
    }

    // Now combine the elements from the array to compiledobj.compiledstring[0];
    for (int iterator = 0; compiledobj.compiledpool[iterator][0] != dfkeys.dk_nullterminate2; iterator++) {
        compiledobj.compiledstring[0][0] += compiledobj.compiledpool[iterator][0];
    }

    // Return the compiled object
    return compiledobj;
}

std::string returnstring(pools::compiledpool compiledobj) {
    std::string returnstring = compiledobj.compiledstring[0][0];
    return returnstring;
}

int main(int argc, char* argv[]) {

    std::string filename;

    if (argc == 1) {
        std::cout << "Error: No arguments passed." << std::endl;
        // return 1;
    }
    else if (argc == 2) {
        std::cout << "Passed in: " << argv[1] << std::endl;
    }

    filename = "rusticc2.rc";

    std::fstream rusticcfile(filename.c_str());

    // Read file
    std::string rusticcline;
    std::string linearray[128];

    int linecount = 0;

    while (getline(rusticcfile, rusticcline)) {
        linecount++;
    }

    rusticcfile.clear();
    rusticcfile.seekg(0);

    for (int iterator = 0; iterator <= linecount; iterator++) {
        if (iterator == linecount-1) {
            getline(rusticcfile, rusticcline);
            linearray[iterator] = rusticcline;
        }
        else if (iterator < linecount) {
            getline(rusticcfile, rusticcline);
            linearray[iterator] = rusticcline + "\n";
        }
    }

    // We got the lines... Now we need to combine them

    std::string concatenatedcode;

    // For every line in the linearray
    for (int lineiterator = 0; lineiterator < linecount; lineiterator++) {
        // Set each line into concatenatedcode
        concatenatedcode += linearray[lineiterator];
    }

    std::cout << "Compilation string: " << "\n" << concatenatedcode << std::endl;

    // Example print
    // std::cout << linearray[0] << "\n" << linearray[1] << "\n" << linearray[2] << "\n" << linearray[3] << "\n";

    // std::cout << concatenatedcode << std::endl;

    // std::cout << "Combined string: " << linearray[0] << "\n" << linearray[1] << "\n" << linearray[2] << "\n" << linearray[3];

    // The language is basically just c++, but "int" is "integer" and "float" is "decimal".

    // Pass the source to the parse code function and include it in the parsedobject object
    pools::charpools parsedobject = parsestring(concatenatedcode);

    // PARSER DONE! we've now successfully parsed the string and
    pools::keywordpools lexedobject = lexobject(parsedobject, rusticcline);

    // LEXER DONE! now we need to make it translate (compile) the words to c++ for example integer -> int & decimal -> float.

    // Print compiling
    std::cout << "Compiling..." << std::endl;

    // Compiler
    pools::compiledpool compiledobj = compile(lexedobject);

    // Print rustic-c string
    // std::cout << "rustic c code:      " << rusticcline << std::endl;

    // Print the compiled code
    // std::cout << "translated c++ code:  " << compiledobj.compiledstring[0][0] << std::endl;

    // Create file
    std::ofstream cppfile("compiledcode.cpp");

    // Print writing to file
    std::cout << "writing to file compiledcode.cpp..." << std::endl;

    // Write to file
    cppfile << compiledobj.compiledstring[0][0];

    // Close the file
    cppfile.close();

    // Pause
    system("pause");

    // return the exit code, (you'll exit the matrix if you input 420 lol)... shushh...     
    return 0;

}