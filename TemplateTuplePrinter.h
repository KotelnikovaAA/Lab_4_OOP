//
// Created by Anna on 07.01.2021.
//

#ifndef OOP_FIT_C_TEMPLATETUPLEPRINTER_H
#define OOP_FIT_C_TEMPLATETUPLEPRINTER_H

#include <iostream>
#include <tuple>

static const char LEFT_BORDER_SYMBOL = '<';
static const char RIGHT_BORDER_SYMBOL = '>';
static const char DATA_DELIMITER = ' ';

template<typename Ch, typename Tr, class Tuple>
static void printTuple(std::basic_ostream<Ch, Tr> &outputStream, const Tuple &tuple) {
    outputStream << DATA_DELIMITER << std::get<0>(tuple);
}

template<typename Ch, typename Tr, class Tuple, size_t tupleParamsNumber>
static void printTuple(std::basic_ostream<Ch, Tr> &outStream, const Tuple &tuple) {
    printTuple<Ch, Tr, Tuple, tupleParamsNumber - 1>(outStream, tuple);
    outStream << DATA_DELIMITER << std::get<tupleParamsNumber - 1>(tuple);
}

template<typename Ch, typename Tr, typename ...Arguments>
std::basic_ostream<Ch, Tr> &operator<<(std::basic_ostream<Ch, Tr> &outputStream, std::tuple<Arguments...> &tuple) {
    outputStream << LEFT_BORDER_SYMBOL;
    outputStream << printTuple<Ch, Tr, decltype(tuple), sizeof...(Arguments)>(outputStream, tuple);
    outputStream << RIGHT_BORDER_SYMBOL;

    return outputStream;
}


#endif //OOP_FIT_C_TEMPLATETUPLEPRINTER_H
