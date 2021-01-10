//
// Created by Anna on 07.01.2021.
//

#ifndef OOP_FIT_C_TEMPLATETUPLEREADER_H
#define OOP_FIT_C_TEMPLATETUPLEREADER_H

#include <tuple>
#include <sstream>

#include "CSVParserException.h"


static const size_t NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING = 1;

static void skipSymbols(std::istringstream &inputStrStream, size_t skipSymbolsNumber) {
    for (int skipSymbolsCounter = 0; skipSymbolsCounter < skipSymbolsNumber; skipSymbolsCounter++) {
        inputStrStream.get();
    }
}

template<typename FirstParamType>
std::tuple<FirstParamType>
readNextTuple(std::istringstream &inputStrStream, size_t &columnNumber, const char delimiterSymbol,
              const char screenedSymbol) {
    if (inputStrStream.eof()) {
        throw CSVParser::CSVParserException("less data was encountered than expected.", columnNumber,
                                            CSVParser::ExceptionType::DataUnderflow);
    }

    FirstParamType inputData;
    inputStrStream >> inputData;
    std::string s;
    bool flag = false;
    while (inputStrStream.peek() == screenedSymbol) {
        flag = true;
        s = std::to_string(inputData);
        if (inputStrStream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", columnNumber,
                                                CSVParser::ExceptionType::DataUnderflow);
        }

        size_t lastSymbolStringIndex = s.size() - 1;
        s[lastSymbolStringIndex] = delimiterSymbol;
        std::string extraData;
        skipSymbols(inputStrStream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
        inputStrStream >> extraData;
        s += extraData;

        if (inputStrStream.fail()) {
            throw CSVParser::CSVParserException("", columnNumber,
                                                CSVParser::ExceptionType::InvalidData);
        }

    }

    if (inputStrStream.fail()) {
        throw CSVParser::CSVParserException("", columnNumber,
                                            CSVParser::ExceptionType::InvalidData);
    }

    if (!inputStrStream.eof()) {
        throw CSVParser::CSVParserException(
                "more data was encountered than expected. Extra data will be skipped ...", columnNumber,
                CSVParser::ExceptionType::DataOverflow);
    }

    ++columnNumber;
    if (flag) {
        return std::make_tuple(s);
    } else {
        return std::make_tuple(inputData);
    }
};


template<typename FirstParamType, typename SecondParamType, typename ...Arguments>
std::tuple<FirstParamType, SecondParamType, Arguments...>
readNextTuple(std::istringstream &inputStrStream, size_t &columnNumber, const char delimiterSymbol,
              const char screenedSymbol) {
    if (inputStrStream.eof()) {
        throw CSVParser::CSVParserException("less data was encountered than expected.", columnNumber,
                                            CSVParser::ExceptionType::DataUnderflow);
    }

    FirstParamType inputData;
    inputStrStream >> inputData;
    std::string s;
    bool flag = false;
    while (inputStrStream.peek() == screenedSymbol) {
        flag = true;
        s = std::to_string(inputData);
        if (inputStrStream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", columnNumber,
                                                CSVParser::ExceptionType::DataUnderflow);
        }

        size_t lastSymbolStringIndex = s.size() - 1;
        s[lastSymbolStringIndex] = delimiterSymbol;
        std::string extraData;
        skipSymbols(inputStrStream, NEEDED_NUMBER_SKIPPED_SYMBOLS_FOR_SCREENING);
        inputStrStream >> extraData;
        s += extraData;

        if (inputStrStream.fail()) {
            throw CSVParser::CSVParserException("", columnNumber,
                                                CSVParser::ExceptionType::InvalidData);
        }

    }

    if (inputStrStream.fail()) {
        throw CSVParser::CSVParserException("", columnNumber,
                                            CSVParser::ExceptionType::InvalidData);
    }

    ++columnNumber;
    if (flag) {
        return std::tuple_cat(std::make_tuple(s),
                              readNextTuple<SecondParamType, Arguments...>(inputStrStream, columnNumber, delimiterSymbol, screenedSymbol));
    } else {
        return std::tuple_cat(std::make_tuple(inputData),
                              readNextTuple<SecondParamType, Arguments...>(inputStrStream, columnNumber, delimiterSymbol, screenedSymbol));
    }
};


template<typename ...Arguments>
void readTuples(std::istringstream &strStream, std::tuple<Arguments...> &tuple,
                const char delimiterSymbol,
                const char screenedSymbol) {
    size_t columnNumber = 1;
    tuple = readNextTuple<Arguments...>(strStream, columnNumber, delimiterSymbol, screenedSymbol);
}


#endif //OOP_FIT_C_TEMPLATETUPLEREADER_H
