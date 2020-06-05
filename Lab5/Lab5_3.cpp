#include <cctype>
#include <cstdio>
#include <iostream>

using namespace std;

int lenOfInt(int i)
{
    int count = 0;
    if (i < 0)
        count ++;
    while (i != 0)
    {
        i/=10;
        count++;
    }
    return count;
}

int countOfAlphaInStr(char* str)
{
    int count = 0;
    while (*str)
    {
        if (isalpha(*str))
        {
            count++;
        }
        str++;
    }
    return count;
}

class Monom
{
private:
    int count;
    char *vars;
    int *degrees;
    int ks;

public:
    Monom(const Monom& monom)
    {
        *this = monom;
    }

    Monom& operator = (const Monom& right)
    {
        this->count = right.count;
        this->ks = right.ks;
        this->degrees = new int[count];
        this->vars = new char[count];
        for (int i = 0; i < right.count; ++i)
        {
            this->degrees[i] = right.degrees[i];
            this->vars[i] = right.vars[i];
        }
        return *this;
    }

    Monom(char* str)
    {
        char* ptr = str;
        count = countOfAlphaInStr(str);

        vars = new char[count];
        degrees = new int[count];
        int i = 0;
        if (*ptr == '<')
        {
            ptr++;
        }
        if (*ptr == '+')
            ptr++;
        if (isdigit(*ptr))
        { //5*x^2y^3z
            sscanf(ptr, "%d", &ks);
            ptr += lenOfInt(ks);
        }
        else
        {
            if (*ptr == '-')
            {
                ks = -1;
                ptr++;
            }
            else if (*ptr == '+')
            {
                ks = 1;
                ptr++;
            }
            ks = 1;
        }
        while (*ptr != '>' && *ptr != 0)
        {
            if (*ptr == '*')
            {
                ptr++;
                sscanf(ptr, "%c", &vars[i]);
                ptr++;
            }
            else if (isalpha(*ptr))
            {
                sscanf(ptr, "%c", &vars[i]);
                degrees[i] = 1;
                ptr++;
            }
            if (*ptr == '^')
            {
                ptr++;
                sscanf(ptr, "%d", &degrees[i]);
                ptr+= lenOfInt(degrees[i]);
            }
            else if (*ptr)
            {
                degrees[i] = 1;
                if (*ptr == '>')
                    break;
            }
            i++;
        }
    }

    bool equalExceptKs(const Monom* monom)
    {
        if (this->count != monom->count)
        {
            return false;
        }
        if (count == 0)
            return true;

        for (int i = 0; i < this->count; ++i)
        {
            int flag = 0;
            for (int j = 0; j < this->count; ++j)
            {
                if (this->vars[i] == monom->vars[j])
                {
                    if (this->degrees[i] == monom->degrees[j])
                    {
                        flag = 1;
                        continue;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (flag == 0 && j == this->count - 1)
                {
                    return false;
                }
            }
        }
        return true;
    }


    ~Monom()
    {
        delete []degrees;
        delete []vars;
    }

    void remove()
    {
        delete this;
    }

    friend Monom operator * (const Monom & left, const Monom& right);
    friend Monom operator / (const Monom & left, const Monom& right);
    friend istream& operator >> (istream& in, Monom& right);
    friend ostream& operator << (ostream& out, const Monom& right);

    Monom&operator *=(const Monom& right)
    {
        *this = *this * right;
        return *this;
    }

    Monom&operator /=(const Monom& right)
    {
        *this = *this / right;
        return *this;
    }



    void setKs(int k)
    {
        this->ks = k;
    }

    void setDegrees(int* d)
    {
        degrees = d;
    }

    void setVars(char* v)
    {
        vars = v;
    }

    char* getVars()
    {
        return vars;
    }

    int getKs()
    {
        return ks;
    }

    int getCount()
    {
        return count;
    }

    void setCount(int c)
    {
        count = c;
    }

    void print() const
    {
        cout << *this;
    }
};

istream& operator >> (istream &in, Monom & right)
{
    char buf[BUFSIZ];
    in >> buf;
    right = Monom(buf);
    return in;
}

ostream &operator<<(ostream& out,const Monom &right)
{
    if (right.ks != 0)
    {
        cout << right.ks;
        if (right.count != 0)
        {
            cout << '*';
        }
    }
    for (int i = 0; i < right.count; ++i)
    {
        cout << right.vars[i];
        if (right.degrees[i] != 0 && right.degrees[i] != 1)
        {
            cout << "^" << right.degrees[i];
        }
    }
    return out;
}

Monom operator*(const Monom &left, const Monom &right)
{
    int freeSpaceInd = 0;
    Monom l = left;
    Monom r = right;
    if (r.count > l.count)
    {
        Monom tmp = r;
        r = l;
        l = tmp;
    }

    freeSpaceInd = r.count;

    r.ks *= l.ks;
    l.ks = r.ks;
    if (l.count != r.count)
    {
        Monom tmp = r;
        r.count = l.count;
        r.degrees = new int[l.count];
        r.vars = new char[l.count];
        for (int i = 0; i < tmp.count; ++i)
        {
            r.degrees[i] = tmp.degrees[i];
            r.vars[i] = tmp.vars[i];
        }
    }

    if (right.count == 0 || left.count == 0)
    {
        for (int i = 0; i < l.count; ++i)
        {
            r.degrees[i] = l.degrees[i];
            r.vars[i] = l.vars[i];
        }
        return r;
    }


    for (int i = 0; i < freeSpaceInd; ++i)
    {
        for (int j = 0; j < l.count; ++j)
        {
            if (r.vars[i] == l.vars[j])
            {
                l.degrees[j] += r.degrees[i];
                break;
            }
            else if (j == l.count - 1)
            {
                l.vars[freeSpaceInd] = r.vars[i];
                l.degrees[freeSpaceInd] = r.degrees[i];
                freeSpaceInd++;
            }
        }
    }
    return l;
}

Monom operator/(const Monom &left, const Monom &right)
{
    Monom result = left;
    result.ks /= right.ks;
    for (int i = 0; i < right.count; ++i)
    {
        for (int j = 0; j < left.count; ++j)
        {
            if (right.vars[i] == result.vars[j])
            {
                result.degrees[j] -= right.degrees[i];
                break;
            }
            else if (j == left.count - 1)
            {
                result.vars[result.count] = right.vars[i];
                result.degrees[result.count] = -right.degrees[i];
                result.count++;
            }
        }
    }
    return result;
}

class Polynom
{
    class List
    {
    private:
        class Node
        {
        public:
            Node *prev;
            Node *next;
            Monom *data;

            Node(Monom *d) : data{d}
            {
                prev = nullptr;
                next = nullptr;
            }

            Node()
            {
                prev = nullptr;
                next = nullptr;
                data = nullptr;
            };
        };

        Node *first;
        Node *last;

    public:
        List(Monom* monom)
        {
            first = new Node(monom);
            last = first;
            first->prev = nullptr;
            last->next = nullptr;
        }

        List() : first{nullptr}, last{nullptr} {};


        Monom* get(int i) const
        {
            Node* cur = first;
            for (int j = 0; j < i; ++j)
            {
                if (cur == nullptr || cur->next == nullptr)
                    return nullptr;
                cur = cur->next;
            }

            if (cur == nullptr)
                return nullptr;

            return cur->data;
        }

        void addToEnd(Monom* monom)
        {
            if (last == nullptr)
            {
                last = new Node(monom);
                first = last;
                return;
            }
            last->next = new Node(monom);
            last->next->prev = last;
            last = last->next;
        }

        void set(int i, Monom * monom)
        {

            Node* cur = first;

            for (int j = 0; j < i; ++j)
            {
                cur = cur->next;
            }
            cur->data = monom;
        }

        void remove(int i)
        {
            if (first == nullptr)
                return;
            if (first == last)
            {
                delete first;
                last = nullptr;
                first = nullptr;
                return;
            }


            Node* cur = first;

            for (int j = 0; j < i; ++j)
            {
                cur = cur->next;
            }

            if (i == 0)
            {
                Node* d = first;
                first = first->next;
                first->prev= nullptr;

                delete d;
                return;
            }
            if (cur == last)
            {
                last = cur->prev;
                cur->prev->next = nullptr;
                delete cur;
                return;
            }

            cur->prev->next = cur->next;
            cur->next->prev = cur->prev;
            delete cur;
        }
    };

private:
    List list;
public:
    Polynom(Monom* monom): list{List(monom)} {}

    Polynom()
    {
        list = List();
    }

    Polynom& operator = (const Polynom& right)
    {
        for (int i = 0; right.list.get(i) != nullptr; ++i)
        {
            Monom* m = new Monom(*right.list.get(i));
            this->list.addToEnd(m);
        }
        return *this;
    }

    Polynom(const Polynom& polynom)
    {
        *this = polynom;
    }

    Polynom(char* str)
    { // 4x^2y^3-xy^2+4xy+1
        Polynom result;
        char buf[BUFSIZ];
        char* ptr = str;
        char* b = buf;
        char c = -1;
        char action = '+';
        c = *ptr++;
        while (c != 0 )
        {
            if (*ptr == 0)
            {
                *b++ = c;
            } else {
                if (c == '-')
                {
                    action = '-';
                    c = *ptr++;
                }
                while (c != '+' && c != '-' )
                {
                    if (*ptr == 0)
                    {
                        *b++ = c;
                        break;
                    }
                    *b++ = c;
                    c = *ptr++;
                }
            }

            *b = 0;
            b = buf;
            Polynom n(new Monom(buf));
            if (action == '+')
            {
                result += n;
            }
            else if (action == '-')
            {
                result -= n;
            }
            if (c == '+' || c == '-')
            {
                action = c;
            }
            c = *ptr++;
        }
        *this = result;
    }

    void simplify()
    { // 3+1*x-1 + 4*xy+5
        for (int i = 0; list.get(i) != nullptr; ++i)
        {
            for (int j = i + 1; list.get(j) != nullptr; ++j)
            {
                if (list.get(i)->equalExceptKs(list.get(j)))
                {
                    int k = list.get(i)->getKs() + list.get(j)->getKs();
                        Monom *r = list.get(j);
                        r->setKs(k);
                        list.set(i, r);

                        list.remove(j);

                        j--;
                }
            }
        }

        for (int l = 0; list.get(l) !=nullptr; ++l)
        {
            if (list.get(l)->getKs() == 0)
            {
                list.remove(l);
                l--;
            }
        }
    }

    void print() const
    {
        for (int i = 0; list.get(i) != nullptr; ++i)
        {
            if (list.get(i)->getKs() > 0) cout << "+";
            list.get(i)->print();
        }
    }

    Polynom& operator += (const Polynom& right)
    {

        for (int i = 0; right.list.get(i) != nullptr; ++i)
        {
            this->list.addToEnd(right.list.get(i));
        }

        this->simplify();
        return *this;
    }

    Polynom& operator -= (const Polynom& right)
    {

        for (int i = 0; right.list.get(i) != nullptr; ++i)
        {
            Monom* tmp = right.list.get(i);
            tmp->setKs(-1*right.list.get(i)->getKs());
            this->list.addToEnd(tmp);
        }

        this->simplify();

        return *this;
    }

    Polynom& operator *= (const Polynom& right)
    {
        Polynom tmp = *this;
        while (this->list.get(0) != nullptr) list.remove(0);
        for (int i = 0; tmp.list.get(i) != nullptr; ++i)
        {
            for (int j = 0; right.list.get(j) != nullptr; ++j)
            {
                Monom t = *tmp.list.get(i);

                t = t * *right.list.get(j);

                this->list.addToEnd(new Monom(t));
            }
        }
        this->simplify();
        return *this;
    }


    bool operator == (const Polynom& right)
    {
        Polynom result = *this;
        result -= right;
        if (result.list.get(0) == nullptr)
        {
            return true;
        }

        return false;
    }

    bool operator != (const Polynom& right)
    {
        return !(*this == right);
    }

    friend Polynom operator + (const Polynom& left, const Polynom & right);
    friend Polynom operator - (const Polynom& left, const Polynom & right);
    friend Polynom operator * (const Polynom& left, const Polynom & right);
    friend istream& operator >> (istream& in,  Polynom & right);
    friend ostream& operator << (ostream& out, const Polynom & right);
};

Polynom operator+(const Polynom& left, const Polynom & right)
{
    Polynom result = left;
    return result += right;
}

Polynom operator-(const Polynom &left, const Polynom &right)
{
    Polynom result = left;
    return  result -= right;
}

Polynom operator*(const Polynom &left, const Polynom &right)
{
    Polynom result = left;
    return  result *= right;
}

istream &operator>>(istream &in, Polynom &right)
{

    char buf[BUFSIZ];
    in >> buf;
    right = Polynom(buf);
    return in;
}

ostream &operator<<(ostream &out, const Polynom &right)
{
    for (int i = 0; right.list.get(i) != nullptr; ++i)
    {
        if (right.list.get(i)->getKs() > 0)
            cout << "+";
        out << right.list.get(i);
    }
    return out;
}


int main(int argc, char* argv[])
{
    FILE* file;
    if (argc == 1 || (!(file = fopen(argv[1], "r"))))
    {
        exit(-1);
    }
    char buf[BUFSIZ];
    char* b = buf;
    char c;
    char action = 0;
    while (!(feof(file)))
    {
        fscanf(file, "%c", &c);
        if (feof(file))
            break;
        if (c == ' ' || c == '\n')
        {
            continue;
        }
        else if (c == ';')
        {
            *b++ = ';';
            *b = 0;
            b = buf;
            Polynom result;

            while (*b != ';')
            {
                char bufForPolynom[BUFSIZ], *ptr = bufForPolynom;
                if (*b == '<')
                    action = '+';
                else
                {
                    action = *b;
                    b++;
                    if (action == '=' || action == '!')
                        b++;
                }
                if (*b == '<')
                    b++;
                while (*b != '>')
                {
                    *ptr++ = *b++;
                }
                *ptr = 0;
                b++;
                Polynom add(bufForPolynom);

                switch (action)
                {
                    case '+':
                        result += add;
                        break;

                    case '-':
                        result -= add;
                        break;

                    case '*':
                        result *= add;
                        break;

                    case '=':
                        cout << (result == add) << endl;
                        break;

                    case '!':
                        cout << (result != add) << endl;
                        break;
                }
            }

            if (action != '!' && action != '=')
            {
                result.print();
                cout << endl;
            }

            b = buf;

        }
        else
        {
            *b++ = c;
        }
    }
}
