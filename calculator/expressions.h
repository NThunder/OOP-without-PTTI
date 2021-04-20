#ifndef CALCULATOR_EXPRESSIONS_H
#define CALCULATOR_EXPRESSIONS_H

#include <stdexcept>
#include "tokenize.h"
#include <memory>

class WrongExpressionError : public std::runtime_error {
public:
    explicit WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {
    }
};

struct IExDescriptor {
    const char* UniqueAddr;
    int (*kCalculate)(char*);
    void (*Destroyer)(char*);
};

struct IExpression {
    IExDescriptor Descriptor;
    char OperStorage[256 - sizeof(IExDescriptor)];
    template <typename TBaser>
    explicit IExpression(TBaser r) {
        static_assert(sizeof(TBaser) <= 256 - sizeof(IExDescriptor), "too large Base");
        static const char kUniqueVar = '\0';
        Descriptor.UniqueAddr = &kUniqueVar;
        Descriptor.kCalculate = [](char* f) -> int { return (reinterpret_cast<TBaser*>(f))->Calculate(); };
        Descriptor.Destroyer = [](char* f) { (reinterpret_cast<TBaser*>(f))->~TBaser(); };
        new (OperStorage) TBaser(r);
    }
    ~IExpression() {
        if (Descriptor.UniqueAddr) {
            Descriptor.Destroyer(OperStorage);
        }
    }
    int Calculate() const {
        return Descriptor.kCalculate(const_cast<char*>(OperStorage));
    }
};

class Constant {
    int value_;

public:
    explicit Constant(int);
    int Calculate() const;
};

struct IBinDescriptor {
    const char* UniqueAddr;
    int (*kPerformOperation)(char*, int, int);
    void (*Destroyer)(char*);
};

struct IBinaryOperation {
    IBinDescriptor Descriptor;
    char OperStorage[198 - sizeof(IBinDescriptor)];
    std::shared_ptr<IExpression> first_;
    std::shared_ptr<IExpression> second_;
    int Calculate() const;
    template <typename TBaser>
    explicit IBinaryOperation(TBaser f) : first_(std::move(f.first_)), second_(std::move(f.second_)) {
        static_assert(sizeof(TBaser) <= 198 - sizeof(IBinDescriptor), "too large Base");
        static const char kUniqueVar = '\0';
        Descriptor.UniqueAddr = &kUniqueVar;
        Descriptor.kPerformOperation = [](char* f, int a, int b) -> int {
            return (reinterpret_cast<TBaser*>(f))->PerformOperation(a, b);
        };
        Descriptor.Destroyer = [](char* f) { (reinterpret_cast<TBaser*>(f))->~TBaser(); };
        new (OperStorage) TBaser(std::move(f));
    }
    ~IBinaryOperation() {
        if (Descriptor.UniqueAddr) {
            Descriptor.Destroyer(OperStorage);
        }
    }
    int PerformOperation(int a, int b) const {
        return Descriptor.kPerformOperation(const_cast<char*>(OperStorage), a, b);
    }
};

class Sum {
public:
    std::shared_ptr<IExpression> first_;
    std::shared_ptr<IExpression> second_;
    Sum(std::shared_ptr<IExpression>&&, std::shared_ptr<IExpression>&&);
    int PerformOperation(int, int) const;
};

class Subtract {
public:
    std::shared_ptr<IExpression> first_;
    std::shared_ptr<IExpression> second_;
    Subtract(std::shared_ptr<IExpression>&&, std::shared_ptr<IExpression>&&);
    int PerformOperation(int, int) const;
};

class Multiply {
public:
    std::shared_ptr<IExpression> first_;
    std::shared_ptr<IExpression> second_;
    Multiply(std::shared_ptr<IExpression>&&, std::shared_ptr<IExpression>&&);
    int PerformOperation(int, int) const;
};

class Divide {
public:
    std::shared_ptr<IExpression> first_;
    std::shared_ptr<IExpression> second_;
    Divide(std::shared_ptr<IExpression>&&, std::shared_ptr<IExpression>&&);
    int PerformOperation(int, int) const;
};

class Residual {
public:
    std::shared_ptr<IExpression> first_;
    std::shared_ptr<IExpression> second_;
    Residual(std::shared_ptr<IExpression>&&, std::shared_ptr<IExpression>&&);
    int PerformOperation(int, int) const;
};

std::shared_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos);
std::shared_ptr<IExpression> ParseAddendum(const std::vector<Token>& tokens, size_t& pos);
std::shared_ptr<IExpression> ParseMultiplier(const std::vector<Token>& tokens, size_t& pos);

#endif  // CALCULATOR_EXPRESSIONS_H