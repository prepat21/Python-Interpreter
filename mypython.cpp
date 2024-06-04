#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <fstream>

#include "scanner.h"
#include "parser.h"
#include "statement.h"
#include "printer.h"
#include "interpreter.h"

std::string openFile(std::string filename) {
    std::ifstream file;
    file.open(filename);

    std::string contents;
    while (file.good()) {
        char c;
        file.get(c);
        contents += c;
    }

    file.close();
    return contents;
}

// Main function
int main(int argc, char * argv[]) {
    if (argc != 2) {
        std::cout << "usage: mypython <file.py>\n";
        return 1;
    }
    std::string filename = std::string(argv[1]);

    //std::string filename = "./testcases/in08.py";

    std::string code = openFile(filename);
    Scanner scan(code);
    Parser parser(scan.getTokens());
    std::vector<Statement*> s = parser.parse();

    //Printer printer;
    //printer.print(s);

    Interpreter interpreter;
    interpreter.run(s);

    return 0;
}
