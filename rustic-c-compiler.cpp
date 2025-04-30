#ifdef __WINDOWS__ 
#include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

// パソコンと言うことは物とか事とか？　事と物の違いはなんですか？
// The rustic c language is just like c or c++ with minor changes:
// int -> integer and float -> decimal
// just write like normal and pass the .rustic file to the program
// Rustic C kieli on kuin C tai C++ kieli mutta pienillä muutoksilla:
// int avainsana on -> integer ja float -> decimal.
// Kirjoita normaalisti kuin C tai C++ kieltä ja syötä tiedostonimi kääntäjälle
// Syötä ulostulo tiedoston nimi tiedostopäätteen kanssa tai ilman.
// Valitse lopuksi haluatko, että ulostulo kootaan exe tiedostoksi

namespace pools {
    struct charpool {
        std::vector<std::string> charPool; // Dynamic list of strings
    };

    struct keywordpool {
        std::vector<std::string> keywordPool; // Dynamic list of strings
    };

    struct compiledobject {
        std::string compiledString; // Single dynamic string for the compiled output
    };
}

std::unordered_set<char> specialSymbols = {' ', '#', '(', ')', '<', '>', '{', '}', '[', ']', ';', ',', '\n', '\t'};

pools::charpool lex(std::string codeToBeLexed) {
    pools::charpool CharPool;
    std::string currentWord;
    std::string spaceToken; // To accumulate consecutive spaces

    for (char c : codeToBeLexed) {
        if (specialSymbols.count(c)) {
            if (!currentWord.empty()) {
                CharPool.charPool.push_back(currentWord); // Add the current word to the pool
                currentWord.clear();
            }
            if (c == ' ') {
                spaceToken += c; // Accumulate spaces
            } else {
                if (!spaceToken.empty()) {
                    CharPool.charPool.push_back(spaceToken); // Add the accumulated spaces as a token
                    spaceToken.clear();
                }
                CharPool.charPool.push_back(std::string(1, c)); // Add other special symbols as single-character tokens
            }
        } else {
            if (!spaceToken.empty()) {
                CharPool.charPool.push_back(spaceToken); // Add the accumulated spaces as a token
                spaceToken.clear();
            }
            currentWord += c; // Append character to the current word
        }
    }

    if (!currentWord.empty()) {
        CharPool.charPool.push_back(currentWord); // Add the last word
    }
    if (!spaceToken.empty()) {
        CharPool.charPool.push_back(spaceToken); // Add the last accumulated spaces
    }

    return CharPool;
}

pools::keywordpool parse(pools::charpool lexedObject) {
    pools::keywordpool KeywordPool;

    // Iterate through all words in charpool
    for (int wordindex = 0; wordindex < lexedObject.charPool.size(); wordindex++) {
        const std::string& currentWord = lexedObject.charPool[wordindex];

        // Check if the current word is a space
        if (currentWord == " ") {
            KeywordPool.keywordPool.push_back(" "); // Add a space token
        } else {
            // Add the current word to the keyword pool
            KeywordPool.keywordPool.push_back(currentWord);
        }
    }

    return KeywordPool;
}

pools::compiledobject compile(pools::keywordpool parsedObject) {
    pools::compiledobject compiledObject;

    // Debug: Print the size of the keyword pool
    std::cout << "Keyword pool size: " << parsedObject.keywordPool.size() << std::endl;

    for (int iterator = 0; iterator < parsedObject.keywordPool.size(); iterator++) {
        // Debug: Print the current keyword being processed
        std::cout << "Processing keyword: " << parsedObject.keywordPool[iterator] << std::endl;

        if (parsedObject.keywordPool[iterator] == "integer") {
            compiledObject.compiledString += "int";
        } else if (parsedObject.keywordPool[iterator] == "decimal") {
            compiledObject.compiledString += "float";
        } else if (parsedObject.keywordPool[iterator] == "\n") {
            // Only add a newline if it's not the last token
            if (iterator != parsedObject.keywordPool.size() - 1) {
                compiledObject.compiledString += "\n";
            }
        } else {
            compiledObject.compiledString += parsedObject.keywordPool[iterator];
        }
    }

    // Debug: Print the compiled string
    std::cout << "Compiled string: " << compiledObject.compiledString << std::endl;

    return compiledObject;
}

int main(int argc, char* argv[]) {

    std::string filename;
    std::string outputfilename;

    if (argc == 1) {
        std::cout << "Code .rustic files like c/c++ but don't use int and float, use integer and decimal!" << std::endl;
        std::cout << "Enter .rustic source filename: ";
        std::cin >> filename;
        outputfilename = "output.cpp";
    }
    else if (argc == 2) {
        std::cout << "Compiling: " << argv[1] << std::endl;

        filename = argv[1];
        outputfilename = "output.cpp";
    }
    else if (argc == 3) {
            std::cout << "The syntax is: program.exe [source] -o [output]..." << std::endl;
            return 1;
    }
    else if (argc == 4) {
        std::string output = "-o";
        if (argv[2] == output) {
            filename = argv[1];
            outputfilename = argv[3];
            std::cout << "Compiling: " << argv[1] << std::endl;
        }
    }

    std::fstream rusticcfile(filename.c_str());

    // Check if file opened successfully
    if (!rusticcfile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Read file
    std::string sourceCode;
    std::string line;

    while (getline(rusticcfile, line)) {
        sourceCode += line + "\n";
    }

    if (sourceCode.empty()) {
        std::cerr << "Error: Source file is empty." << std::endl;
        return 1;
    }

    // Pass the source to the parsestring function and return parsed object
    pools::charpool lexedObject = lex(sourceCode);

    // Pass the parsed object
    pools::keywordpool parsedObject = parse(lexedObject);

    // Compiler
    pools::compiledobject compiledObject = compile(parsedObject);

    // Create file
    std::ofstream cppfile(outputfilename.c_str());

    // Print writing to file
    std::cout << "Writing to: " << outputfilename << std::endl;

    // Write to file
    cppfile << compiledObject.compiledString;

    // Close the file
    cppfile.close();


    // Create variables for exe output
    std::string answer;
    std::string executable;

    // Check if user wants to build an executable
    std::cout << "Do you want to build an executable with g++? Make sure you have it installed!" << std::endl;
    std::cout << "(yes/no): ";
    std::cin >> answer;

    // If user wants to create an executable, use g++
    if (answer == "yes") {
        std::cout << "Give your executable a name: ";
        std::cin >> executable;
        std::string cmdline = "g++ -o " + executable + " " + outputfilename;
        system(cmdline.c_str());
    } else if (answer == "no") {
        std::cout << "" << std::endl;
    } else {

        std::cout << "Syntax error: please enter correct syntax" << std::endl;
    }

    // Pause
    system("pause");

    // return the exit code and exit program 
    return 0;
}