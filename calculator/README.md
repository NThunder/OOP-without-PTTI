### Калькулятор

*Когда говорят "программа на ЭВМ" одно из первых, что приходит на ум*
*- калькулятор. Да и, действительно, изначальное назначение*
*компьютеров - производить математические вычисления. И кажется,*
*что это настолько базовая задача, что написать калькулятор на*
*любом известном вам языке программирования не составляет никакого труда.*
*Тем не менее, оказывается, что это не такая уж тривиальная и*
*скучная задача. Как, например, вычислить значение, заданное*
*входом "1 * (((2 * 3) * (4 + 5) + 6 + 7) * (8 * 9 + 10 * 11) * 12)"?*

### Calculator

Арифметическое выражение - это сумма нескольких 
(одного и более) слагаемых. Слагаемое - это произведение (деление)
нескольких множителей. Множитель - это константа, либо выражение
заключенное в скобки. Таким образом, необходимо уметь выделять
в исходном тексте каждую из этих трех сущностей.

Выделение (парсинг) выражения состоит из парсинга слагаемого, 
которое в начале полагается равным всему выражению. Пусть уже
построено выражение некоторой длины. Тогда, если следующий элемент
это знак + или -, то парсится следующее слагаемое, а выражение
полагается равным сумме уже построенного выражения и нового
слагаемого. 

Выделение (парсинг) слагаемого эквивалентен парсингу выражения
с точностью до замены знаков +, - на *, /, %.

Парсинг множителя это простой разбор случаев: если следующий токен
 константа, то множитель - это константа. Если открывающаяся скобка,
 то множитель равен выражению, стоящему в скобках (обрабатывается
 с помощью парсинга выражения).

#### Детали реализации

Решение состоит из трех частей - токенизации, построения дерева разбора (парсинг), 
вычисления разобранного выражения.

В файле `expressions.h` объявлены классы выражений, а в 
`expressions.cpp` написана реализация их методов.

* `IExpression` - абстрактный класс для представления произвольного
выражения.
    * Содержит единственный чисто виртуальный метод `int Calculate()`,
который вычисляет значение выражения
* `Constant` - наследник `IExpression`.
    * Имеет конструктор от `int`.
    * Определяет метод `Calculate` возвращающий значение
* `IBinaryOperation` - абстрактный класс, наследник `IExpression`, представляющий произвольную бинарную операцию
     * Хранит два поля - левый операнд-выражение и правый операнд-выражение
     * Чисто виртуальный метод `int PerformOperation(int, int)`, который принимает операнды и вычисляет результат операции
     * Определяет метод `Calculate`, путем вычисления операндов и применения к ним операции
     
Программа поддерживает следующие операции: `Sum` (+), `Subtract` (-),
`Multiply`(*), `Divide`(/), `Residual`(%).
Каждая операция унаследована от нужного класса и 
реализован соответствующий метод `PerformOperation`.

В тех же файлах определены функции `ParseExpression`, 
`ParseAddendum`, `ParseMultiplier`, возвращающие умный указатель
на выражение и принимающие `const std::vector<Token>& tokens`(токены)
 с `size_t& pos` (позиция текущего токена).
 
В файлах `calculator.h` и `calculator.cpp` 
определена функция
`int CalculateExpression(const std::string& input)`,
принимающая выражение в виде строки и возвращающая ответ.

Программа бросает исключения `UnknownSymbolError`, если
в результате токенизации был получен неизвестный токен и 
`WrongExpressionError`, в случае возникновения ошибок
 при вычислении или разборе
 
 **Замечания.**
 
Можно добавлять другие операции. Для этого будет достаточно ввести новый класс и правильно
его унаследовать.

В задаче вместо механизма виртуального наследования и абстрактных классов реализован упращённый аналог.

