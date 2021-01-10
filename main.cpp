#include <iostream>

#include "CSVTupleParser.h"
#include "ArgumentsHandler.h"

int main(int argc, char *argv[]) {
    std::ifstream inputFileStream;
    inputFileStream.open("input.csv", std::ios::binary);
    std::freopen("log.txt", "w", stderr);

    ArgumentsHandler handler(argc, argv);
    CSVTupleParser<int, long, double, double, std::string, int> parser(inputFileStream,
                                                                  handler.getSkipLinesNumber(),
                                                                  handler.getDelimiter(), '"');
    try {
        for (auto it : parser) {
            //std::cout << it << std::endl;
        }
    } catch (std::exception &e) {
        std::cerr << "Main: program stopped!" << std::endl;
    }
    return 0;
}