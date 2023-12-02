#include <iostream>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <cassert>

// Объявление типов.
// Токен (лексема):
typedef char Token;
// Стек токенов:
typedef std::stack<Token> Stack;
// Последовательность токенов:
typedef std::queue<Token> Queue;
// Множество различных токенов:
typedef std::set<Token> Set;
// Таблица значений переменных:
typedef std::map<Token, Token> Map;
// Пара переменная—значение:
typedef std::pair<Token, Token> VarVal;
// Строка символов:
typedef std::string String;

// Является ли токен числом?
inline bool isNumber(Token t)
{
    return t == '0' || t == '1';
}

// Является ли токен переменной?
inline bool isVariable(Token t)
{
    return (t >= 'A' && t <= 'Z') || (t >= 'a' && t <= 'z');
}

// Является ли токен операцией?
inline bool isOperation(Token t)
{
    return (t == '|' || t == '&' || t == '!' || t == '>' || t == '~');
}

// Является ли токен открывающей скобкой?
inline bool isOpeningPar(Token t)
{
    return t == '(';
}

// Является ли токен закрывающей скобкой?
inline bool isClosingPar(Token t)
{
    return t == ')';
}

// Вернуть величину приоритета операции
// (чем больше число, тем выше приоритет)
inline int priority(Token op)
{
    assert(isOperation(op));
    int res = 0;
    switch (op)
    {
    case '!':
        // Отрицание — наивысший приоритет
        res = 5;
        break;
    case '&':
        // Конъюнкция
        res = 4;
        break;
    case '|':
        // Дизъюнкция
        res = 3;
        break;
    case '>':
        // Импликация
        res = 2;
        break;
    case '~':
        // Эквивалентность — наинизший приоритет
        res = 1;
        break;
    }
    return res;
}

// Преобразовать последовательность токенов,
// представляющих выражение в инфиксной записи,
// в последовательность токенов, представляющих
// выражение в обратной польской записи
// (алгоритм Дейкстры «Сортировочная станция»)
Queue infixToPostfix(Queue input)
{
    // Выходная последовательность (очередь вывода):
    Queue output;
    // Рабочий стек:
    Stack s;
    // Текущий входной токен:
    Token t;
    // Пока есть токены во входной последовательности:
    while (!input.empty())
    {
        // Получить токен из начала входной последовательности
        t = input.front();
        input.pop();
        // Если токен — число или переменная, то:
        if (isNumber(t) || isVariable(t))
        {
            // Добавить его в очередь вывода
            output.push(t);
            // Если токен — операция op1, то:
        }
        else if (isOperation(t))
        {
            // Пока на вершине стека присутствует токен-операция op2
            // и у op1 приоритет меньше либо равен приоритету op2, то:
            while (!s.empty() && isOperation(s.top()) && priority(t) <= priority(s.top()))
            {
                // переложить op2 из стека в выходную очередь
                output.push(s.top());
                s.pop();
            }
            // Положить op1 в стек
            s.push(t);
            // Если токен — открывающая скобка, то:
        }
        else if (isOpeningPar(t))
        {
            // Положить его в стек
            s.push(t);
            // Если токен — закрывающая скобка, то:
        }
        else if (isClosingPar(t))
        {
            // Пока токен на вершине стека не является открывающей скобкой:
            while (!s.empty() && !isOpeningPar(s.top()))
            {
                // Перекладывать токены-операции из стека
                // в выходную очередь
                assert(isOperation(s.top()));
                output.push(s.top());
                s.pop();
            }
            // Если стек закончился до того,
            // как был встречен токен-«открывающая скобка», то:
            if (s.empty())
            {
                // В выражении пропущена открывающая скобка
                throw String("The opening parenthesis is missing!");
            }
            else
            {
                // Иначе выкинуть открывающую скобку из стека
                // (но не добавлять в очередь вывода)
                s.pop();
            }
        }
        else
        {
            // В остальных случаях входная последовательность
            // содержит токен неизвестного типа
            String msg("Unknown character \'");
            msg += t + String("\'!");
            throw msg;
        }
    }
    // Токенов на входе больше нет, но ещё могут остаться токены в стеке.
    // Пока стек не пустой:
    while (!s.empty())
    {
        // Если токен на вершине стека — открывающая скобка, то:
        if (isOpeningPar(s.top()))
        {
            // В выражении присутствует незакрытая скобка
            throw String("An unclosed bracket!");
        }
        else
        {
            // Иначе переложить токен-операцию из стека в выходную очередь
            assert(isOperation(s.top()));
            output.push(s.top());
            s.pop();
        }
    }
    // Конец алгоритма.
    // Выдать полученную последовательность
    return output;
}

// Напечатать последовательность токенов
void printSequence(Queue q)
{
    while (!q.empty())
    {
        std::cout << q.front();
        q.pop();
    }
    std::cout << std::endl;
}

// Является ли символ пробельным?
inline bool isSpace(char c)
{
    return c <= ' ';
}

// Если символ — маленькая буква, преобразовать её в большую,
// иначе просто вернуть этот же символ
inline char toUpperCase(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 'A';
    }
    else
    {
        return c;
    }
}

// Преобразовать строку с выражением в последовательность токенов
// (лексический анализатор)
Queue stringToSequence(const String &s)
{
    Queue res;
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (!isSpace(s[i]))
        {
            res.push(toUpperCase(s[i]));
        }
    }
    return res;
}

// Напечатать сообщение об ошибке
inline void printErrorMessage(const String &err)
{
    std::cerr << "*** ERROR! " << err << std::endl;
}

// Ввести выражение с клавиатуры
inline String inputExpr()
{
    String expr;
    std::cout << "Enter the formula:";
    std::getline(std::cin, expr);
    return expr;
}

// Выделить из последовательности токенов переменные
int getAmountVariables(Queue s, Set &res)
{
    int counter = 0;
    while (!s.empty())
    {
        if (isVariable(s.front()) && res.count(s.front()) == 0)
        {
            res.insert(s.front());
            counter++;
        }
        s.pop();
    }
    return counter;
}

// Получить значения переменных с клавиатуры
Map inputVarValues(const Set &var)
{
    Token val;
    Map res;
    for (char i : var)
    {
        do
        {
            std::cout << i << " = ";
            std::cin >> val;
            if (!isNumber(val))
            {
                std::cerr << "Enter 0 or 1!" << std::endl;
            }
        } while (!isNumber(val));
        res.insert(VarVal(i, val));
    }
    return res;
}

// Заменить переменные их значениями
Queue substValues(Queue expr, Map &varVal)
{
    Queue res;
    while (!expr.empty())
    {
        if (isVariable(expr.front()))
        {
            res.push(varVal[expr.front()]);
        }
        else
        {
            res.push(expr.front());
        }
        expr.pop();
    }
    return res;
}

// Является ли операция бинарной?
inline bool isBinOp(Token t)
{
    return t == '&' || t == '|' || t == '>' || t == '~';
}

// Является ли операция унарной?
inline bool isUnarOp(Token t)
{
    return t == '!';
}

// Получить bool-значение токена-числа (true или false)
inline bool logicVal(Token x)
{
    assert(isNumber(x));
    return x == '1';
}

// Преобразовать bool-значение в токен-число
inline Token boolToToken(bool x)
{
    if (x)
    {
        return '1';
    }
    else
    {
        return '0';
    }
}

// Вычислить результат бинарной операции
inline Token evalBinOp(Token a, Token op, Token b)
{
    assert(isNumber(a) && isBinOp(op) && isNumber(b));
    bool res;
    // Получить bool-значения операндов
    bool left = logicVal(a);
    bool right = logicVal(b);
    switch (op)
    {
    case '&':
        // Конъюнкция
        res = left && right;
        break;
    case '|':
        // Дизъюнкция
        res = left || right;
        break;
    case '>':
        // Импликация
        res = !left || right;
        break;
    case '~':
        // Эквивалентность
        res = (!left || right) && (!right || left);
        break;
    }
    return boolToToken(res);
}

// Вычислить результат унарной операции
inline Token evalUnarOp(Token op, Token a)
{
    assert(isUnarOp(op) && isNumber(a));
    bool res = logicVal(a);
    switch (op)
    {
    case '!':
        // Отрицание
        res = !res;
        break;
    }
    return boolToToken(res);
}

// Вычислить значение операции, модифицируя стек.
// Результат помещается в стек
void evalOpUsingStack(Token op, Stack &s)
{
    assert(isOperation(op));
    // Если операция бинарная, то:
    if (isBinOp(op))
    {
        // В стеке должны быть два операнда
        if (s.size() >= 2)
        {
            // Если это так, то извлекаем правый операнд-число
            Token b = s.top();
            if (!isNumber(b))
            {
                throw String("Incorrect expression!");
            }
            s.pop();
            // Затем извлекаем левый операнд-число
            Token a = s.top();
            if (!isNumber(a))
            {
                throw String("Incorrect expression!");
            }
            s.pop();
            // Помещаем в стек результат операции
            s.push(evalBinOp(a, op, b));
        }
        else
        {
            throw String("Incorrect expression!");
        }
        // Иначе операция унарная
    }
    else if (isUnarOp(op) && !s.empty())
    {
        // Извлекаем операнд
        Token a = s.top();
        if (!isNumber(a))
        {
            throw String("Incorrect expression!");
        }
        s.pop();
        // Помещаем в стек результат операции
        s.push(evalUnarOp(op, a));
    }
    else
    {
        throw String("Incorrect expression!");
    }
}

// Вычислить значение выражения, записанного в обратной польской записи
Token evaluate(Queue expr)
{
    // Рабочий стек
    Stack s;
    // Текущий токен
    Token t;
    // Пока входная последовательность содержит токены:
    while (!expr.empty())
    {
        // Считать очередной токен
        t = expr.front();
        assert(isNumber(t) || isOperation(t));
        expr.pop();
        // Если это число, то:
        if (isNumber(t))
        {
            // Поместить его в стек
            s.push(t);
            // Если это операция, то:
        }
        else if (isOperation(t))
        {
            // Вычислить её, модифицируя стек
            // (результат также помещается в стек)
            evalOpUsingStack(t, s);
        }
    }
    // Результат — единственный элемент в стеке
    if (s.size() == 1)
    {
        // Вернуть результат
        return s.top();
    }
    else
    {
        throw String("Incorrect expression!");
    }
}

// Вывести результат вычисления на экран
void printResult(Token r)
{
    assert(isNumber(r));
    std::cout << "Meaning of the expression: " << r << std::endl;
}

// Считает количество дизъюнктов, входящих в СКНФ, преобразует полученную формулу в СКНФ
int getSKNF(const Queue &expr, int **matrix, const Set &variables, int countVariables)
{
    int mask;
    int rowsCounter = 1 << countVariables; // собой количество возможных наборов
    // значений переменных в булевой функции.
    int c = 0;
    for (size_t i = 0; i < rowsCounter; i++)
    {
        Map varVariables;
        mask = rowsCounter;
        auto k = variables.begin();
        // проходимся по всем переменным и пополняем словарь парами переменная-значение
        for (size_t j = 0; j < countVariables; j++)
        {
            mask >>= 1;
            bool t = i & mask;
            Token T = boolToToken(t);
            varVariables.insert(VarVal(*k, T));
            k++;
        }
        // заменяем переменные в выражении expr на соответствующие значения
        //  переменных на текущей итерации
        Queue sExpr = substValues(expr, varVariables);

        // вычисляем значение выражения
        Token res = evaluate(sExpr);
        if (res == '0')
        {
            mask = rowsCounter;
            for (size_t j = 0; j < countVariables; j++)
            {
                mask >>= 1;
                bool t = i & mask;
                if (t)
                {
                    (*matrix)[j] = -1;
                }
                else
                    (*matrix)[j] = 0;
            }
            matrix++;
            c++;
        }
    }

    return c;
}

// Получает дизъюнкт
void writeExpr(int **a, int nums, int n, const Set &variables)
{
    std::set<int>::iterator();
    for (int i = 0, it = *variables.begin(); i < n; i++, it++)
    {
        if (a[nums][i] == -1)
            std::cout << '!';
        std::cout << (Token)(it) << " | ";
    }
    std::cout << "\b\b\b";
}

bool isFirstInput = true;

// Выводит формулу-следствие
void writeConsequence(const int *D, int **matrix, const Set &variables, int countVars, int countString)
{
    for (int i = 0; i < countString; i++)
    {
        if (D[i] != 0)
        {
            if (isFirstInput)
                std::cout << "(";
            else
                std::cout << "&(";

            writeExpr(matrix, i, countVars, variables);
            std::cout << ") ";
            isFirstInput = false;
        }
    }

}

static void subsets_inner(int i, int n, const Set &variables, int **matrix, int m)
{
    static int D[100];
    for (int x = 0; x <= 1; x++)
    {
        D[i] = x;
        if (i == n - 1)
        {
            writeConsequence(D, matrix, variables, m, n);
        }
        else
            subsets_inner(i + 1, n, variables, matrix, m);
    }
}

// Выводит все найденные формулы-следствия
void writeAllSubsetsFromExpr(int n, int m, int **matrix, const Set &variables)
{
    subsets_inner(0, n, variables, matrix, m);
    isFirstInput = true;
}

// Главная программа
int main()
{
    try
    {
        // Ввод выражения
        // String expr = inputExpr();

        String exprs[] = {
            "p>(q|(r&s))",
            "(A|B)",
            "A>B",
            "(A&B)>A"};
        for (auto expr : exprs)
        {
            // Преобразовать последовательность токенов в ОПЗ
            Queue exprInPostfixNotation = infixToPostfix(stringToSequence(expr));

            // Выделить из последовательности токенов переменные и вернуть их кол-во
            Set variables;
            int countVariables = getAmountVariables(exprInPostfixNotation, variables);
            int rowsCounter = 1 << countVariables;

            // выделение памяти под хранение матрицы
            int **matrix = new int *[rowsCounter];
            for (int i = 0; i < rowsCounter; i++)
                matrix[i] = new int[countVariables];

            // получаем СКНФ выражения
            int k = getSKNF(exprInPostfixNotation, matrix, variables, countVariables);

            // выводим формулы-следствия
            std::cout << "Formulas of consequence:";
            writeAllSubsetsFromExpr(k, countVariables, matrix, variables);
            // очищение памяти, выделенной под хранение матрицы
            for (int i = 0; i < rowsCounter; i++)
                delete[] matrix[i];
            delete[] matrix;
        }
    }
    catch (const String &err)
    {
        // Если возникла ошибка, вывести сообщение
        printErrorMessage(err);
        // И выйти из программы с неудачным кодом завершения
        exit(1);
    }

    // Конец программы
    return 0;
}
