#define NO_RTTI
#ifndef CALCULATOR_POLISH_NOTATION_OPERATION_H
#define CALCULATOR_POLISH_NOTATION_OPERATION_H

#include <memory>
#include <stack>
#include <utility>
#include <stdexcept>
#include <iostream>

class WrongOperandsNumberError : public std::runtime_error {
public:
    explicit WrongOperandsNumberError(const std::string& msg = "")
        : std::runtime_error("WrongOperandsNumberError: " + msg) {
    }
};

class WrongExpressionError : public std::runtime_error {
public:
    explicit WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {
    }
};

struct IOperation;

using Stack = std::stack<std::unique_ptr<IOperation>>;

struct Constant {
    int value;
    explicit Constant(int);
    int Value() const;
    void Calculate(Stack*) const;
};

//-------------------------------------------------------------

struct IOpDescriptor {
    const char* UniqueAddr;
    void (*kCalculate)(char*, Stack*);
    void (*Destroyer)(char*);
};

struct IOperation {
    IOpDescriptor Descriptor;
    char OperStorage[128 - sizeof(IOpDescriptor)];
    template <typename TBaser>
    explicit IOperation(TBaser r) {
        static_assert(sizeof(TBaser) <= 128 - sizeof(IOpDescriptor), "too large Base");
        static const char kUniqueVar = '\0';
        Descriptor.UniqueAddr = &kUniqueVar;
        Descriptor.kCalculate = [](char* f, Stack* s) { return (reinterpret_cast<TBaser*>(f))->Calculate(s); };
        Descriptor.Destroyer = [](char* f) { (reinterpret_cast<TBaser*>(f))->~TBaser(); };
        new (OperStorage) TBaser(r);
    }
    ~IOperation() {
        if (Descriptor.UniqueAddr) {
            Descriptor.Destroyer(OperStorage);
        }
    }
    void Calculate(Stack* s) const {
        Descriptor.kCalculate(const_cast<char*>(OperStorage), s);
    }
};

struct IUnDescriptor {
    const char* UniqueAddr;
    int (*kPerformOperation)(char*, int);
    void (*Destroyer)(char*);
};

struct IUnaryOperation {
    IUnDescriptor Descriptor;
    char OperStorage[104 - sizeof(IUnDescriptor)];
    int GetOperand(Stack*) const;
    void Calculate(Stack*) const;
    template <typename TBaser>
    explicit IUnaryOperation(TBaser f) {
        static_assert(sizeof(TBaser) <= sizeof(OperStorage), "too large Base");
        static const char kUniqueVar = '\0';
        Descriptor.UniqueAddr = &kUniqueVar;
        Descriptor.kPerformOperation = [](char* f, int ind) -> int {
            return (reinterpret_cast<TBaser*>(f))->PerformOperation(ind);
        };
        Descriptor.Destroyer = [](char* f) { (reinterpret_cast<TBaser*>(f))->~TBaser(); };
        new (OperStorage) TBaser(std::move(f));
    }
    ~IUnaryOperation() {
        if (Descriptor.UniqueAddr) {
            Descriptor.Destroyer(OperStorage);
        }
    }
    int PerformOperation(int ind) const {
        return Descriptor.kPerformOperation(const_cast<char*>(OperStorage), ind);
    }
};

struct IBinDescriptor {
    const char* UniqueAddr;
    int (*kPerformOperation)(char*, std::pair<int, int>);
    void (*Destroyer)(char*);
};

struct IBinaryOperation {
    IBinDescriptor Descriptor;
    char OperStorage[104 - sizeof(IBinDescriptor)];
    std::pair<int, int> GetOperands(Stack*) const;
    void Calculate(Stack*) const;
    template <typename TBaser>
    explicit IBinaryOperation(TBaser f) {
        static_assert(sizeof(TBaser) <= sizeof(OperStorage), "too large Base");
        static const char kUniqueVar = '\0';
        Descriptor.UniqueAddr = &kUniqueVar;
        Descriptor.kPerformOperation = [](char* f, std::pair<int, int> ind) -> int {
            return (reinterpret_cast<TBaser*>(f))->PerformOperation(ind);
        };
        Descriptor.Destroyer = [](char* f) { (reinterpret_cast<TBaser*>(f))->~TBaser(); };
        new (OperStorage) TBaser(std::move(f));
    }
    ~IBinaryOperation() {
        if (Descriptor.UniqueAddr) {
            Descriptor.Destroyer(OperStorage);
        }
    }
    int PerformOperation(std::pair<int, int> ind) const {
        return Descriptor.kPerformOperation(const_cast<char*>(OperStorage), ind);
    }
};

class Square {

public:
    int PerformOperation(int) const;
};

class AbsoluteValue {

public:
    int PerformOperation(int) const;
};

class Sum {

public:
    int PerformOperation(std::pair<int, int>) const;
};

class Subtract {

public:
    int PerformOperation(std::pair<int, int>) const;
};

class Multiply {

public:
    int PerformOperation(std::pair<int, int>) const;
};

class Divide {

public:
    int PerformOperation(std::pair<int, int>) const;
};

class Residual {

public:
    int PerformOperation(std::pair<int, int>) const;
};

class Minimum {

public:
    int PerformOperation(std::pair<int, int>) const;
};

class Maximum {

public:
    int PerformOperation(std::pair<int, int>) const;
};

#endif  // CALCULATOR_POLISH_NOTATION_OPERATION_H