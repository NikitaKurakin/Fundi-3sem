#include <iostream>
#include <math.h>
#include <string>
#include <cstring>

#define DATA_TYPE double
#define COMMANDS_SIZE 7

class Node
{
public:
    DATA_TYPE polynom;
    DATA_TYPE degree;
    Node* next;

    ~Node()
    {
    }


    Node(DATA_TYPE polynom, DATA_TYPE degree) : polynom{polynom}, degree{degree}
    {
        next = nullptr;
    }

    Node& operator *= (const Node& right)
    {
        this->degree += right.degree;
        this->polynom *= right.polynom;
        return *this;
    }

    Node& operator * (const Node& right)
    {
        Node* result = new Node(this->polynom * right.polynom, this->degree + right.degree);
        return *result;
    }


};

class List
{
private:
    Node* first;
    int size;
public:
    List()
    {
        first = nullptr;
        size = 0;
    }

    ~List()
    {
        Node* cur = first;
        for (int i = 0; i < size; ++i)
        {
            Node* del = cur;
            cur = cur->next;
            delete del;
        }
    }


    void add(const Node &node, int i)
    {
        if (size < i)
        {
            std::cout << "size";
            return;
        }

        Node *a = new Node(node);
        if (i == 0 && size == 0)
        {
            first = a;
        }
        else if (i == 0)
        {
            a->next = first;
            first = a;
        }
        else
        {
            Node *prev = first;
            Node *cur = first->next;
            for (int j = 1; j < i; ++j)
            {
                prev = cur;
                cur = cur->next;
            }
            a->next = cur;
            prev->next = a;
        }
        size++;
    }

    Node get(int i) const
    {
        if (i > size)
            std::cout << "Out of ";

        Node *cur = first;

        for (int j = 0; j < i; ++j)
        {
            cur = cur->next;
        }
        return *cur;
    }

    void set(const Node& node, int i)
    {
        Node *cur = first;

        for (int j = 0; j < i; ++j)
        {
            cur = cur->next;
        }

        cur->degree = node.degree;
        cur->polynom = node.polynom;
    }

    Node *pop(int i)
    {
        Node *result = first;

        if (i == 0)
        {
            first = first->next;
            size--;
            return result;
        }

        Node *prev = first;
        result = result->next;

        for (int j = 1; j < i; ++j)
        {
            prev = result;
            result = result->next;
        }

        prev->next = result->next;
        size--;
        return result;
    }

    int getSize() const
    {
        return size;
    }
};

class Polynomial
{
private:
    List data;

public:

    Polynomial &operator= (const Polynomial& right)
    {
        this->data = List();
        for (int i = 0; i < right.data.getSize(); ++i)
        {
            Node node = right.data.get(i);
            node.next = nullptr;
            this->data.add(node, i);
        }
        return *this;
    }


    Polynomial(const Polynomial& polynomial)
    {
        this->data = List();
        int size = polynomial.data.getSize();
        for (int i = 0; i < size; ++i)
        {
            Node node = polynomial.data.get(i);
            node.next = nullptr;
            this->data.add(node, i);
        }
    }


    Polynomial()
    {
        this->data = List();
    }


    ~Polynomial() = default;


    void add(const Node& node)
    {
        for (int i = 0; i < data.getSize(); ++i)
        {
            if (node.degree >= data.get(i).degree)
            {
                if (node.degree == data.get(i).degree)
                {
                    DATA_TYPE polynom = data.get(i).polynom + node.polynom;
                    data.set(Node(polynom, node.degree), i);
                }
                else
                {
                    data.add(node, i);
                }

                if (data.get(i).polynom == 0)
                    data.pop(i);
                return;
            }
        }
        data.add(node, data.getSize());
    }


    Polynomial eval(Polynomial left)
    {
        Polynomial result;
        double x = left.data.get(0).polynom;
        double tmp = 0;
        double r = 0;

        for (int i = 0; i < data.getSize(); ++i)
        {
            tmp = pow(x, data.get(i).degree);
            r += tmp * data.get(i).polynom;
        }

        result.add(Node(r, 0));
        return result;
    }


    Polynomial diff()
    {
        Polynomial result;

        for (int i = 0; i < data.getSize(); ++i)
        {
            double polynom = data.get(i).polynom * data.get(i).degree;
            double degree = data.get(i).degree - 1;
            Node node(polynom, degree);
            result.add(node);
        }

        return result;
    }


    void print() const
    {
        for (int i = 0; i < data.getSize(); ++i)
        {
            if (data.get(i).polynom == 0)
                continue;
            if (i != 0 && data.get(i).polynom >0)
            {
                std::cout << "+";
            }
            if (data.get(i).polynom < 0)
            {
                if (data.get(i).polynom == -1)
                    std::cout << "-";
            }
            if (data.get(i).degree == 0 || data.get(i).polynom == 1 || data.get(i).polynom == -1 || data.get(i).degree == 1)
            {
                if (data.get(i).degree == 0)
                {
                    std::cout << abs(data.get(i).polynom);
                }
                else if (data.get(i).polynom == 1 || data.get(i).polynom == -1)
                {
                    std::cout << "x^" << data.get(i).degree;
                }
                else if (data.get(i).degree == 1)
                {
                    std::cout << data.get(i).polynom << "x";
                }
            }
            else
            {
                std::cout << data.get(i).polynom << "x^" << data.get(i).degree;
            }
        }

        std::cout << std::endl;
    }


    Polynomial&operator+=(const Polynomial& right)
    {
        for (int i = 0; i < right.data.getSize(); ++i)
        {
            this->add(right.data.get(i));
        }
        return *this;
    }


    Polynomial&operator-=(const Polynomial& right)
    {
        for (int i = 0; i < right.data.getSize(); ++i)
        {
            Node node = right.data.get(i);
            node.polynom *= -1;
            this->add(node);
        }
        return *this;
    }


    Polynomial& operator*=(const Polynomial & right)
    {
        Polynomial result;
        for (int i = 0; i < this->data.getSize(); ++i)
        {
            Node left = this->data.get(i);

            for (int j = 0; j < right.data.getSize(); ++j)
            {
                DATA_TYPE polynom = left.polynom * right.data.get(j).polynom;
                DATA_TYPE degree = left.degree + right.data.get(j).degree;
                result.add(*new Node(polynom, degree));
            }
        }

        *this = result;
        return *this;
    }


    Polynomial&operator/=(const Polynomial & right)
    {
        Polynomial result;
        Polynomial ost = *this;

        while (ost.data.getSize() != 0 && right.data.getSize() != 0 && ost.data.get(0).degree >= right.data.get(0).degree)
        {
            DATA_TYPE polynom = ost.data.get(0).polynom / right.data.get(0).polynom;
            DATA_TYPE degree = ost.data.get(0).degree - right.data.get(0).degree;
            Node tmp = Node(polynom, degree);
            result.data.add(tmp, result.data.getSize());

            for (int i = 0; i < right.data.getSize(); ++i)
            {
                Node minus = right.data.get(i);
                minus.polynom = -1 * minus.polynom;
                ost.add(tmp * minus);
            }
        }

        *this = result;
        return *this;
    }


    Polynomial&operator%=(const Polynomial & right)
    {
        Polynomial result;
        Polynomial ost = *this;

        while (ost.data.getSize() != 0 && right.data.getSize() != 0 && ost.data.get(0).degree >= right.data.get(0).degree)
        {
            DATA_TYPE polynom = ost.data.get(0).polynom / right.data.get(0).polynom;
            DATA_TYPE degree = ost.data.get(0).degree - right.data.get(0).degree;
            Node tmp = Node(polynom, degree);
            result.data.add(tmp, result.data.getSize());
            for (int i = 0; i < right.data.getSize(); ++i)
            {
                Node minus = right.data.get(i);
                minus.polynom = -1 * minus.polynom;
                ost.add(tmp * minus);
            }
        }

        *this = ost;
            return *this;
    }

    bool isNull()
    {
        return data.getSize() == 0;
    }


    friend Polynomial& operator % (const Polynomial& left, const Polynomial & right);
    friend Polynomial& operator / (const Polynomial& left, const Polynomial & right);
    friend Polynomial& operator + (const Polynomial& left, const Polynomial & right);
    friend Polynomial& operator - (const Polynomial& left, const Polynomial & right);
    friend Polynomial& operator * (const Polynomial& left, const Polynomial & right);
    friend std::istream& operator >> (std::istream& in, Polynomial &polynomial);
    friend std::ostream& operator << (std::ostream& out, const Polynomial &polynomial);
};

Polynomial& operator*(const Polynomial& left, const Polynomial& right)
{
    Polynomial *tmp = new Polynomial(left);
    return *tmp *= right;
}

Polynomial& operator+(const Polynomial& left, const Polynomial& right)
{
    Polynomial *tmp = new Polynomial(left);
    return *tmp += right;
}

Polynomial& operator-(const Polynomial& left, const Polynomial& right)
{
    Polynomial *tmp = new Polynomial(left);
    return *tmp -= right;
}

Polynomial &operator%(const Polynomial &left, const Polynomial &right)
{
    Polynomial *tmp = new Polynomial(left);
    return *tmp %= right;
}

Polynomial& operator /(const Polynomial &left, const Polynomial &right)
{
    Polynomial *tmp = new Polynomial(left);
    return *tmp /= right;
}

std::istream &operator>>(std::istream &in, Polynomial &polynomial)
{
    int size = 0;
    in >> size;
    int polynom = 0;
    int degree = 0;
    for (int i = 0; i < size; ++i)
    {
        in >> polynom >> degree;
        polynomial.add(Node(polynom, degree));
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Polynomial &polynomial)
{
    int size = polynomial.data.getSize();
    out << size;
    for (int i = 0; i < size; ++i)
    {
        out << " " << polynomial.data.get(i).polynom << " " << polynomial.data.get(i).degree;
    }
    return out;
}


const char* commands[COMMANDS_SIZE] =
{
        "Add", "Sub", "Mult", "Div", "Mod", "Eval", "Diff"
};

Polynomial& getPolUpToSep(char* buf, char sep)
{
    Polynomial *result = new Polynomial;
    double polynom = 0;
    double degree = 0;
    char *b = buf;

    while (*b != sep)
    {
        if (*b == '-')
        {
            b++;
            polynom = -1;
        }
        else
        {
            if (*b == '+')
            {
                b++;
            }
            polynom = 1;
        }

        while (*b != '+' && *b != '-' && *b != sep)
        {
            if (isdigit(*b))
            {
                double tmp = polynom;
                sscanf(b, "%lf", &polynom);
                polynom *= tmp;
                while (*b != '+' && *b != '-' && *b != 'x' && *b != sep && *b != ')')
                    b++;
            }

            if (*b == 'x' && *(b + 1) == '^')
            {
                b += 2;
                sscanf(b, "%lf", &degree);
                while (*b != '+' && *b != '-' && *b != 'x' && *b != sep && *b != ')')
                    b++;
            }
            else if (*b == 'x')
            {
                degree = 1;
                b++;
            }
            else if (*b != '+' && *b != '-')
            {
                degree = 0;
            }
            result->add(Node(polynom, degree));
        }
    }
        return *result;
}

int main(int argc, char* argv[])
{
    FILE* file;

    if (!(file = fopen(argv[1], "r")))
    {
        exit(-1);
    }

    char buf[BUFSIZ], *b = buf;
    char c;
    Polynomial sum;
    while (!feof(file))
    {
        fscanf(file, "%c", &c);
        if (feof(file))
            break;
        if (c == ' ' || c == '\n')
            continue;
        if (c == ';')
        {
            *b = 0;
            int i;

            for (i = 0; i < COMMANDS_SIZE; ++i)
            {
                if (strstr(buf, commands[i]) == buf)
                {
                    break;
                }
            }

            b = buf;
            b += strlen(commands[i]) + 1;
            int countOfArg = 0;

            if (strstr(buf, ","))
            {
                countOfArg = 2;
            }
            else
            {
                countOfArg = 1;
            }

            Polynomial left;
            Polynomial right;
            left = getPolUpToSep(b, ')');

            if (countOfArg == 2)
            {
                left = getPolUpToSep(b, ',');
                while (*b != ',') b++;
                b++;
                right = getPolUpToSep(b, ')');
            }

            if (countOfArg == 1 && i != 6)
                right = sum;

            switch (i)
            {
                case 0:
                    sum = left + right;
                    break;

                case 1:
                    sum = left - right;
                    break;

                case 2:
                    sum = left * right;
                    break;

                case 3:
                    sum = left / right;
                    break;

                case 4:
                    sum = left % right;
                    break;

                case 5:
                    sum = right.eval(left);
                    break;

                case 6:
                    if (left.isNull())
                        left = sum;
                    sum = left.diff();
                    break;
            }

            sum.print();
            b = buf;
        }
        else
        {
            *b++=c;
        }
    }
    fclose(file);
}
