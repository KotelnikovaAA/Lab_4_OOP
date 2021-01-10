//
// Created by Anna on 09.01.2021.
//

#ifndef LAB_4_CSVTUPLEPARSER_H
#define LAB_4_CSVTUPLEPARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <algorithm>

#include "CSVParserException.h"
#include "TemplateTupleReader.h"


    template<typename ...Arguments>
    class CSVTupleParser {
    private:
        std::ifstream &csvInputStream_;
        char csvDelimiterSymbol_ = ';';
        char csvScreenedSymbol_ = '"';
        unsigned int csvSkipLinesNumber_ = 0;

        void skipLines() {
            std::string temporaryString;
            unsigned int currentLineNumber = csvSkipLinesNumber_;
            while (currentLineNumber != 0) {
                std::getline(csvInputStream_, temporaryString);
                --currentLineNumber;
            }
        }

    public:
        CSVTupleParser(std::ifstream &userInputStream, const size_t userSkipLinesNumber, const char userDelimiterSymbol,
                       const char userScreenedSymbol) :
                csvInputStream_(userInputStream),
                csvDelimiterSymbol_(userDelimiterSymbol),
                csvScreenedSymbol_(userScreenedSymbol),
                csvSkipLinesNumber_(userSkipLinesNumber) {
            skipLines();
        };

        class iterator : std::iterator<std::input_iterator_tag, std::tuple<Arguments...>> {
        private:
            std::ifstream &itInputStream_;
            bool itEndOfFile_ = false;
            std::tuple<Arguments...> *itTuple_ = nullptr;
            unsigned int itCurrentRow_;
            std::string itCurrentLine_;
            char itDelimiterSymbol_;
            char itScreenedSymbol_;
            std::streampos itPosition_;

            class CSVParserCType : public std::ctype<char> {
            private:
                mask performedTable[table_size]{};

            public:
                explicit CSVParserCType(char delimiterSymbol, size_t refs = 0)
                        : std::ctype<char>(&performedTable[0], false, refs) {
                    std::copy_n(classic_table(), table_size, performedTable);
                    performedTable[' '] = lower;
                    performedTable[delimiterSymbol] = space;
                }
            };

            void setStringStreamLocateParams(std::istringstream &stream) {
                std::locale specialLocale(std::locale::classic(), new CSVParserCType(itDelimiterSymbol_));
                stream.imbue(specialLocale);
            }

            void readNextTuple() {
                try {
                    itInputStream_.clear();
                    itInputStream_.seekg(itPosition_);
                    itInputStream_ >> itCurrentLine_;
                    itPosition_ = itInputStream_.tellg();

                    std::istringstream stringStream(itCurrentLine_);
                    setStringStreamLocateParams(stringStream);
                    readTuples<Arguments...>(stringStream, *itTuple_, itDelimiterSymbol_, itScreenedSymbol_);
                    ++itCurrentRow_;
                } catch (CSVParser::CSVParserException &exception) {
                    switch (exception.getErrorType()) {
                        case CSVParser::ExceptionType::InvalidData:
                            std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_ << ", in column "
                                      << exception.getErrorColumn()
                                      << " an inappropriate type was encountered!" << std::endl;
                            throw std::runtime_error("");
                        case CSVParser::ExceptionType::DataUnderflow:
                            // less data was encountered than expected
                            std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_ << " " << exception.what()
                                      << std::endl;
                            throw std::runtime_error("");
                        case CSVParser::ExceptionType::DataOverflow:
                            // more data was encountered than expected
                            std::cerr << "CSVTupleParser Iterator: in row " << itCurrentRow_ << " " << exception.what()
                                      << std::endl;
                            ++itCurrentRow_;
                            ++(*this);
                            break;
                    }
                }
            }


        public:
            friend class CSVTupleParser<Arguments...>;

            iterator(std::ifstream &itInputStream, const bool itEndOfFile, const unsigned int currentRow,
                     const char delimiterSymbol, const char screenedSymbol) :
                    itInputStream_(itInputStream),
                    itEndOfFile_(itEndOfFile),
                    itCurrentRow_(currentRow),
                    itDelimiterSymbol_(delimiterSymbol),
                    itScreenedSymbol_(screenedSymbol) {

                itTuple_ = new std::tuple<Arguments...>;

                if (itEndOfFile_ || itPosition_ == EOF) {      // TODO: what for?!
                    itEndOfFile_ = true;
                    return;
                }

                readNextTuple();
            }

            ~iterator() {
                delete itTuple_;
            }

            iterator &operator++() {
                if (itEndOfFile_ || itPosition_ == EOF) {       // TODO: what for?!
                    itEndOfFile_ = true;
                    return *this;
                }

                readNextTuple();
                return *this;
            }

            bool operator==(const iterator &other) const {
                if (other.itEndOfFile_ && this->itEndOfFile_) {
                    return true;
                }
                return (this->itCurrentLine_ == other.itCurrentLine_);
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }

            typename iterator::reference operator*() const {
                return *itTuple_;
            }
        };

        iterator begin() {
            csvInputStream_.clear();
            csvInputStream_.seekg(0);
            return iterator(csvInputStream_, false, csvSkipLinesNumber_ + 1, csvDelimiterSymbol_, csvScreenedSymbol_);
        }

        iterator end() {
            return iterator(csvInputStream_, true, UINT_MAX, csvDelimiterSymbol_, csvScreenedSymbol_);
        }
    };


#endif //LAB_4_CSVTUPLEPARSER_H
