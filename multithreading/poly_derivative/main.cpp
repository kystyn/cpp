#include <iostream>
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

struct monom
{
    int coeff;
    int
        degree;
};

struct comparator {
    bool operator()( const unsigned int &m1, const unsigned int &m2 ) const {
        return m1 > m2;
    }
};

int readNum( std::string::iterator &it )
{
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

    monom current = {0, 0};

    std::map<int, int, comparator> polynom;
    for (auto it = polynomial.begin();
         it < polynomial.end();) {

        if (*it == '+' || *it == '*') {
            it++;
            continue;
        }
        if (isdigit(int(*it))) {
            if (st == WAIT_BASE) {
                current.coeff = readNum(it);
                st = WAIT_DEG;
            }
            else if (st == WAIT_DEG){
                current.degree = readNum(it);
                st = READY;
            }
        }
        else if (*it == '-') {
            if (*(it + 1) == 'x')
                current.coeff = -1;
            else {
                it++;
                current.coeff = -readNum(it);
            }
            st = WAIT_DEG;
        }
        else if (*it == 'x' && *(it + 1) != '^') {
            current.coeff = 1;
            current.degree = 1;
            st = READY;
            it++;
        }
        else if (*it == 'x' && *(it + 1) == '^') {
            current.coeff = 1;
            it += 2;
        }

        if (st == READY){
            std::map<int, int, comparator>::iterator iter;
            if ((iter = polynom.find(current.degree)) != polynom.end())
                iter->second += current.coeff;
                //polynom[current.degree] += current.coeff;
            else
                polynom[current.degree] = current.coeff;
            st = WAIT_BASE;
        }
    }

    if (st == WAIT_DEG) {
        current.degree = 1;
        std::map<int, int, comparator>::iterator iter;
        if ((iter = polynom.find(current.degree)) != polynom.end())
            iter->second += current.coeff;
        else
            polynom[current.degree] = current.coeff;
    }

    std::ostringstream oss;

    bool first = true;
    for (auto it = polynom.begin(); it != polynom.end(); it++) {
        if (!first) {
            if (it->second > 0)
                oss << "+";
            else if (it->second == 0)
                continue;
        }
        first = false;
        if (it->second != 1 && it->second != 0)
            oss << it->second * it->first;
        if (it->first - 1 != 0) {
            oss << "*x";
            if (it->first - 1 != 1)
                oss << "^" << (it->first - 1);
        }
    }
    oss.str();
    return oss.str();
}

int main()
{
    std::cout << derivative("x^3 + x + 1 + x + 2 * x + 5 * x^2");
    return 0;
}
