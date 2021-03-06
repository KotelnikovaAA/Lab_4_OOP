//
// Created by Anna on 07.01.2021.
//

#ifndef LAB_4_CSVPARSEREXCEPTION_H
#define LAB_4_CSVPARSEREXCEPTION_H

#include <exception>
#include <string>

namespace CSVParser {
    enum class ExceptionType {
        InvalidData,
        DataOverflow,
        DataUnderflow
    };

    class CSVParserException : std::exception {
    private:
        ExceptionType errorType_;
        std::string errorMessage_;
        size_t errorColumnNumber_ = 0;

    public:
        CSVParserException() = default;

        explicit CSVParserException(const std::string &errorMessage, const unsigned int errorColumnNumber,
                                    ExceptionType errorType) {
            errorMessage_ = errorMessage;
            errorColumnNumber_ = errorColumnNumber;
            errorType_ = errorType;
        }

        [[nodiscard]] const char *what() const noexcept override {
            return errorMessage_.c_str();
        }

        [[nodiscard]] unsigned int getErrorColumn() const noexcept {
            return errorColumnNumber_;
        }

        [[nodiscard]] ExceptionType getErrorType() const noexcept {
            return errorType_;
        }
    };

}

#endif //LAB_4_CSVPARSEREXCEPTION_H
