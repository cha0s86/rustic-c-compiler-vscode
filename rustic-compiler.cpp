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
// Syötä ulostulotiedoston nimi tiedostopäätteen kanssa tai ilman.
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
};

// Define the special symbols as a set for quick lookup
std::unordered_set<std::string> keywords = {"integer", "decimal", "if", "else", "while", "for", "return"}; // Add more keywords as needed
std::unordered_set<std::string> operators = {"+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!"}; // Add more operators as needed
std::unordered_set<std::string> specialSymbols = {"(", ")", "{", "}", "[", "]", ";", ",", "#"}; // Add more special symbols as needed
std::unordered_set<std::string> types = {"int", "float", "char", "void", "string", "double"}; // Add more types as needed
std::unordered_set<std::string> literals = {"true", "false", "null"}; // Add more literals as needed
std::unordered_set<std::string> comments = {"//", "/*", "*/"}; // Add more comment types as needed
std::unordered_set<std::string> identifiers; // Add more identifiers as needed
std::unordered_set<std::string> strings; // Add more string types as needed
std::unordered_set<std::string> numbers;

pools::charpool lex(std::string codeToBeLexed) {
    pools::charpool CharPool;
    std::string currentWord;
    std::string spaceToken; // To accumulate consecutive spaces

    for (char c : codeToBeLexed) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (!currentWord.empty()) {
                CharPool.charPool.push_back(currentWord); // Add the current word to charPool
                currentWord.clear(); // Clear the current word for the next one
            }
            if (c == ' ') {
                spaceToken += " "; // Accumulate spaces
            } else {
                if (!spaceToken.empty()) {
                    CharPool.charPool.push_back(spaceToken); // Add accumulated spaces
                    spaceToken.clear();
                }
                if (c == '\n') {
                    CharPool.charPool.push_back("\n"); // Add newline to charPool
                } else if (c == '\t') {
                    CharPool.charPool.push_back("\t"); // Add tab to charPool
                }
            }
        } else if (specialSymbols.find(std::string(1, c)) != specialSymbols.end()) {
            if (!currentWord.empty()) {
                CharPool.charPool.push_back(currentWord);
                currentWord.clear();
            }
            if (!spaceToken.empty()) {
                CharPool.charPool.push_back(spaceToken); // Add accumulated spaces
                spaceToken.clear();
            }
            CharPool.charPool.push_back(std::string(1, c)); // Add special symbol to charPool
        } else if (std::isalnum(c) || c == '_') {
            if (!spaceToken.empty()) {
                CharPool.charPool.push_back(spaceToken); // Add accumulated spaces
                spaceToken.clear();
            }
            currentWord += c; // Accumulate alphanumeric characters
        } else {
            if (!currentWord.empty()) {
                CharPool.charPool.push_back(currentWord);
                currentWord.clear();
            }
            if (!spaceToken.empty()) {
                CharPool.charPool.push_back(spaceToken); // Add accumulated spaces
                spaceToken.clear();
            }
            CharPool.charPool.push_back(std::string(1, c)); // Add other characters
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

    // Parser: This function will parse the lexed object and create a keyword pool
    // It will treat all words as normal identifiers for now
    // You can add more complex parsing logic here if needed

    pools::keywordpool KeywordPool;
    std::string currentWord;
    std::string spaceToken; // To accumulate consecutive spaces

    for (const std::string& word : lexedObject.charPool) {
        if (!word.empty()) {
            KeywordPool.keywordPool.push_back(word);
        }
    }

    return KeywordPool;
}

pools::compiledobject compile(pools::keywordpool parsedObject) {
    pools::compiledobject compiledObject;

    for (int iterator = 0; iterator < parsedObject.keywordPool.size(); iterator++) {
        const std::string& token = parsedObject.keywordPool[iterator];

        if (token == "integer") {
            compiledObject.compiledString += "int";
        } else if (token == "decimal") {
            compiledObject.compiledString += "float";
        } else if (token == "\n") {
            // Add a newline without appending extra spaces
            compiledObject.compiledString += "\n";
        } else if (token == " ") {
            // Add spaces as separate tokens
            compiledObject.compiledString += " ";
        } else {
            compiledObject.compiledString += token;
        }
    }

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
    } else if (argc == 2) {
        std::cout << "Compiling: " << argv[1] << std::endl;
        filename = argv[1];
        outputfilename = "output.cpp";
    } else if (argc == 4 && std::string(argv[2]) == "-o") {
        filename = argv[1];
        outputfilename = argv[3];
        std::cout << "Compiling: " << argv[1] << std::endl;
    } else {
        std::cerr << "Error: Invalid arguments. Usage: program.exe [source] -o [output]" << std::endl;
        return 1;
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

    // Read file line by line
    while (getline(rusticcfile, line)) {
        sourceCode += line;
        if (!rusticcfile.eof()) { // Only add a newline if it's not the end of the file
            sourceCode += "\n";
        }
    }

    if (sourceCode.empty()) {
        std::cerr << "Error: Source file is empty." << std::endl;
        return 1;
    }

    // Debug: Print tokens generated by lex
    std::cout << "Lexing source code..." << std::endl;
    pools::charpool lexedObject = lex(sourceCode);
    std::cout << "Lexing completed. Tokens:" << std::endl;
    for (const auto& token : lexedObject.charPool) {
        std::cout << "Token: " << token << std::endl;
    }

    // Debug: Print parsed keywords
    std::cout << "Parsing tokens..." << std::endl;
    pools::keywordpool parsedObject = parse(lexedObject);
    std::cout << "Parsing completed. Keywords:" << std::endl;
    for (const auto& keyword : parsedObject.keywordPool) {
        std::cout << "Keyword: " << keyword << std::endl;
    }

    // Debug: Print compiled output
    std::cout << "Compiling keywords..." << std::endl;
    pools::compiledobject compiledObject = compile(parsedObject);
    std::cout << "Compilation completed. Compiled output:" << std::endl;
    std::cout << compiledObject.compiledString << std::endl;

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