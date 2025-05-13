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

namespace rustic {
    struct CharPool {
        std::vector<std::string> charPool; // Dynamic list of strings
    };

    struct KeywordPool {
        std::vector<std::string> keywordPool; // Dynamic list of strings
    };

    struct CompiledObject {
        std::string compiledString; // Single dynamic string for the compiled output
    };
}

// Define the special symbols as a set for quick lookup
std::unordered_set<std::string> keywords = {"integer", "decimal", "if", "else", "while", "for", "return"}; // Add more keywords as needed
std::unordered_set<std::string> operators = {"+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!"}; // Add more operators as needed
std::unordered_set<char> specialSymbols = {'(', ')', '{', '}', '[', ']', ';', ',', '#'}; // Add more special symbols as needed
std::unordered_set<std::string> cpptypes = {"int", "float", "char", "void", "string", "double"}; // Add more types as needed
std::unordered_set<std::string> literals = {"true", "false", "null"}; // Add more literals as needed
std::unordered_set<std::string> comments = {"//", "/*", "*/"}; // Add more comment types as needed
std::unordered_set<std::string> identifiers; // Add more identifiers as needed
std::unordered_set<std::string> strings; // Add more string types as needed
std::unordered_set<std::string> numbers;

rustic::CharPool lex(const std::string& charizards) {
    rustic::CharPool charPool;
    std::string currentWord;

    for (char charmander : charizards) {

        // Check if the character is a letter or digit
        // If it is, add it to the current word
        if (std::isalpha(charmander) || std::isdigit(charmander)) {
            currentWord += charmander;
        } else if (charmander == '_') { // Allow underscores in identifiers
            currentWord += charmander;
        }

        // Check if the character is a space or newline
        // If it is, add the current word to the charPool and clear it
        else if (std::isspace(charmander)) {
            if (!currentWord.empty()) {
                charPool.charPool.push_back(currentWord);
                currentWord.clear();
            }
            if (charmander == ' ') {
                charPool.charPool.push_back(" ");
            } else if (charmander == '\n') {
                charPool.charPool.push_back("\n");
            } else if (charmander == '\t') {
                charPool.charPool.push_back("\t");
            }
        }

        // Check if the character is a special symbol
        // If it is, add the current word to the charPool and clear it
        else if (specialSymbols.find(charmander) != specialSymbols.end()) {
            if (!currentWord.empty()) {
                charPool.charPool.push_back(currentWord);
                currentWord.clear();
            }
            charPool.charPool.push_back(std::string(1, charmander));
        }

        // If the character is not a letter, digit, space, or special symbol
        // Add it to the current word
        else {
            if (!currentWord.empty()) {
                charPool.charPool.push_back(currentWord);
                currentWord.clear();
            }
            charPool.charPool.push_back(std::string(1, charmander));
        }

    }

    // Add the last word if it exists
    if (!currentWord.empty()) {
        charPool.charPool.push_back(currentWord);
    }

    return charPool;
}

rustic::KeywordPool parse(rustic::CharPool lexedObject) {

    // Parser: This function will parse the lexed object and create a keyword pool
    // It will treat all words as normal identifiers for now
    // You can add more complex parsing logic here if needed.

    rustic::KeywordPool keywordPool;

    for (const std::string& token : lexedObject.charPool) { // Iterate through the charPool
        keywordPool.keywordPool.push_back(token); // Add each token to the keyword pool
    }

    return keywordPool;
}

rustic::CompiledObject compile(rustic::KeywordPool parsedObject) {

    // Compiler: This function will compile the parsed object into a single string
    // It will replace keywords with their C/C++ equivalents

    rustic::CompiledObject compiledObject;

    for (int iterator = 0; iterator < parsedObject.keywordPool.size(); iterator++) {
        const std::string& token = parsedObject.keywordPool[iterator];

        if (token == "integer") {
            compiledObject.compiledString += "int";
        } else if (token == "decimal") {
            compiledObject.compiledString += "float";
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

        // Prompt user for output filename
        std::cout << "Enter output filename (e.g., output.cpp): ";
        std::cin >> outputfilename;
    } else if (argc == 2) {
        std::cout << "Compiling: " << argv[1] << std::endl;
        filename = argv[1];

        // Prompt user for output filename
        std::cout << "Enter output filename (e.g., output.cpp): ";
        std::cin >> outputfilename;
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
    rustic::CharPool lexedObject = lex(sourceCode);
    std::cout << "Lexing completed." << std::endl;

    // Debug: Print parsed keywords
    std::cout << "Parsing tokens..." << std::endl;
    rustic::KeywordPool parsedObject = parse(lexedObject);
    std::cout << "Parsing completed." << std::endl;

    // Debug: Print compiled output
    std::cout << "Compiling keywords..." << std::endl;
    rustic::CompiledObject compiledObject = compile(parsedObject);
    std::cout << "Compilation completed." << std::endl;

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