//
// Created by Anna on 07.01.2021.
//

#ifndef OOP_FIT_C_TEMPLATETUPLEREADER_H
#define OOP_FIT_C_TEMPLATETUPLEREADER_H

#pragma once

#include <tuple>
#include <sstream>

#include "CSVParserException.h"

namespace OperationsOnTuples {

    template<typename FirstParamType>
    std::tuple<FirstParamType> readTuple(std::istringstream &inputStrStream, size_t &columnNumber) {
        if (inputStrStream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", columnNumber,
                                                CSVParser::ExceptionType::DataUnderflow);
        }

        FirstParamType inputData;
        inputStrStream >> inputData;

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
        return std::make_tuple(inputData);
    };


    template<typename FirstParamType, typename SecondParamType, typename ...Arguments>
    std::tuple<FirstParamType, SecondParamType, Arguments...>
    readTuple(std::istringstream &strStream, size_t &columnNumber) {
        if (strStream.eof()) {
            throw CSVParser::CSVParserException("less data was encountered than expected.", columnNumber,
                                                CSVParser::ExceptionType::DataUnderflow);
        }

        FirstParamType inputData;
        strStream >> inputData;

        if (strStream.fail()) {
            throw CSVParser::CSVParserException("", columnNumber,
                                                CSVParser::ExceptionType::InvalidData);
        }

        ++columnNumber;
        return std::tuple_cat(std::make_tuple(inputData),
                              readTuple<SecondParamType, Arguments...>(strStream, columnNumber));
    };


    template<typename ...Arguments>
    std::tuple<Arguments...> &operator>>(std::istringstream strStream, std::tuple<Arguments...> &tuple) {
        size_t columnNumber = 1;
        tuple = readTuple<Arguments...>(strStream, columnNumber);
        return strStream;
    }

}

#endif //OOP_FIT_C_TEMPLATETUPLEREADER_H
