////
//// Created by Anna on 09.01.2021.
////
//
//#ifndef LAB_4_ARGUMENTSHANDLER_H
//#define LAB_4_ARGUMENTSHANDLER_H
//
//// TODO: come up with how to named the namespace
//
//#include <string>
//
//class ArgumentsHandler {
//    // TODO: need to implement validator
//    // TODO: check that argc[1] is actually the number (use the function "isDigit") - получить строку и разобрать её по чиселкам
//};
//
//
//#endif //LAB_4_ARGUMENTSHANDLER_H




#pragma once

#include <string>

class ArgumentsHandler {
private:
    size_t argc_;
    int skipLinesNumber_ = 0;
    char delimiter_ = ';';
public:
    ArgumentsHandler(int argc, char *argv[]) {
        argc_ = argc;
        if (argc_ >= 2) {
            skipLinesNumber_ = std::stoi(argv[1]);
            if (skipLinesNumber_ < 0) {
                std::cerr << "Handler: invalid skip lines number! Set default = 0" << std::endl;
                skipLinesNumber_ = 0;
            }
        }
        if (argc_ >= 3) {
            delimiter_ = argv[2][0];
        }
    }

    [[nodiscard]] size_t getArgc() const {
        return argc_;
    }

    [[nodiscard]] size_t getSkipLinesNumber() const {
        return skipLinesNumber_;
    }

    [[nodiscard]] char getDelimiter() const {
        return delimiter_;
    }
};