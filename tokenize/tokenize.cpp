#include "tokenize.h"
#include "iostream"

NumberToken::NumberToken(int val) : value_(val) {
}

UnknownToken::UnknownToken(std::string val) : value_(val) {
}

std::vector<Token> Tokenize(const std::string& input_) {
    std::string input(input_);
    input += std::string(" ");
    std::vector<Token> vec;
    std::string value;
    for (std::size_t i = 0; i < input.size(); ++i) {
        int size = 0;
        if (input[i] != ' ') {
            value += input[i];
            continue;
        } else {
            for (std::size_t j = 0; j < value.size(); ++j) {
                if ((value[j] >= '0' && value[j] <= '9') ||
                    ((value.size() > 1 && j == 0) && (value[j] == '+' || value[j] == '-'))) {
                    ++size;
                } else {
                    size = 0;
                }
            }
        }
        if (size != 0 && size == static_cast<int>(value.size())) {
            size = std::stoi(value);
            vec.push_back(NumberToken(size));
        } else if (value == "+")
            vec.push_back(PlusToken());
        else if (value == "-")
            vec.push_back(MinusToken());
        else if (value == "*")
            vec.push_back(MultiplyToken());
        else if (value == "/")
            vec.push_back(DivideToken());
        else if (value == "%")
            vec.push_back(ResidualToken());
        else if (value == "(")
            vec.push_back(OpeningBracketToken());
        else if (value == ")")
            vec.push_back(ClosingBracketToken());
        else if (value == "sqr")
            vec.push_back(SqrToken());
        else if (value == "max")
            vec.push_back(MaxToken());
        else if (value == "min")
            vec.push_back(MinToken());
        else if (value == "abs")
            vec.push_back(AbsToken());
        else if (!value.empty())
            vec.push_back(UnknownToken(value));
        value.clear();
    }
    return vec;
}