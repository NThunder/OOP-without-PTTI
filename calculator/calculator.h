#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <string>
#include <stdexcept>

class UnknownSymbolError : public std::runtime_error {
public:
    explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
    }
};

using ValueType = int;

ValueType CalculateExpression(const std::string& input);

#endif  // CALCULATOR_CALCULATOR_H