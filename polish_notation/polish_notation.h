#define NO_RTTI
#ifndef CALCULATOR_POLISH_NOTATION_POLISH_NOTATION_H
#define CALCULATOR_POLISH_NOTATION_POLISH_NOTATION_H

#include <string>
#include <stdexcept>

class UnknownSymbolError : public std::runtime_error {
public:
    explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
    }
};

using ValueType = int;

ValueType ParsePolishNotation(const std::string& input);

#endif  // CALCULATOR_POLISH_NOTATION_POLISH_NOTATION_H
