#include <iostream>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

enum token_type
{
    COEFF, X, DEGREE
};

enum state
{
    WAIT_BASE,
    WAIT_DEG,
    READY
};

struct Monom
{
    int coeff;
    int
        degree;

    void reset( void )
    {
        coeff = 0;
        degree = 0;
    }
};

struct Comparator {
    bool operator()( const unsigned int &m1, const unsigned int &m2 ) const {
        return m1 > m2;
    }
};

struct Token {
    enum struct Type {
        NUM, OP
    };

    Type type;
    int value;
};

int readNum( std::string::iterator &it ) {
    int res = 0;
    while (isdigit(*it)) {
        res = res * 10 + int(*it++) - '0';
    }
    return res;
}

std::string derivative(std::string polynomial) {
    std::string withoutWSpace;

    withoutWSpace.reserve(polynomial.size());

    for (auto it = polynomial.begin();
         it < polynomial.end(); it++)
        if (!iswspace(wint_t(*it)))
            withoutWSpace.push_back(*it);

    polynomial.swap(withoutWSpace);

    state st = WAIT_BASE;

    Monom current = {0, 0};

    bool inverse = false;
    std::map<int, int, Comparator> polynom;
    for (auto it = polynomial.begin(); it < polynomial.end();)
    {
        if (*it == '+' || *it == '*')
        {
            if (*it == '+' && st == WAIT_BASE)
                current.coeff = 0;
            it++;
            continue;
        }
        if (st == WAIT_BASE) {
            if (isdigit(*it))
            {
                current.coeff = readNum(it);
            }
            else if (*it == 'x') {
                if (current.coeff == 0)
                {
                    current.coeff = 1;
                }
                if (*(it + 1) == '^') {
                    st = WAIT_DEG;
                    it += 2;
                }
                else
                {
                    current.degree = 1;
                    st = READY;
                    it++;
                }
            }
            else if (*it == '-')
            {
                inverse = true;
                it++;
            }

        }
        else if (st == WAIT_DEG)
        {
            if (isdigit(*it))
            {
                current.degree = readNum(it);
                st = READY;
            }
        }
        else if (st == READY)
        {
            auto iter = polynom.find(current.degree);
            if (inverse)
                current.coeff *= -1;
            inverse = false;
            if (iter == polynom.end())
                polynom[current.degree] = current.coeff;
            else
                iter->second += current.coeff;
            st = WAIT_BASE;
            current.reset();
        }
    }

    // final add
    auto iter = polynom.find(current.degree);
    if (inverse)
        current.coeff *= -1;
    inverse = false;
    if (iter == polynom.end())
        polynom[current.degree] = current.coeff;
    else
        iter->second += current.coeff;

    std::ostringstream oss;
    bool outCoeff = true, outDeg = true, outX = true;
    for (auto it = polynom.begin(); it != polynom.end(); it++)
    {
        outCoeff = true;
        outDeg = true;
        outX = true;
        if (it->second * it->first == 1)
        {
            outCoeff = false;
        }
        if (it->first - 1 == 1)
            outDeg = false;
        if (it->first - 1 == 0)
        {
            outCoeff = true;
            outX = false;
            outDeg = false;
        }
        if (it->second * it->first == 0)
            continue;

        if (outCoeff)
        {
            if (it->second * it->first > 0)
            {
                if (oss.str().size() != 0)
                    oss << "+";
            }

            oss << it->second * it->first;
        }
        if (outX)
            oss << "*x";
        if (outDeg)
            oss << "^" << it->first - 1;

    }
    if (oss.str().size() == 0)
        return "0\n";
    return oss.str() + '\n';
}

int main()
{
    std::cout << derivative("1+2-3");
    return 0;
}
