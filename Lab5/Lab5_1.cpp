#include <string>
#include <iostream>
#include <cmath>
#include <chrono>

#define eps 0.00000000001

using namespace std;

class Matrix
{
    friend double getTrace(const Matrix& matrix);
    friend double getDeterminant(const Matrix matrix);
    friend Matrix transpose(const Matrix& matrix);
    friend Matrix reverse(const Matrix& matrix);
    friend Matrix exp(const Matrix& matrix);
    friend ostream& operator<<(ostream& out, const Matrix& matrix);
    friend istream& operator>>(istream& in, Matrix& matrix);
private:
    long double **matrix;
    string name;
    int size;

public:

    ~Matrix()
    {
        delete[] matrix;
    }

    Matrix(int size): size{size}
    {
        this->matrix = new long double *[size];
        for (int i = 0; i < size; ++i)
        {
            this->matrix[i] = new long double[size];
            for (int j = 0; j < size; ++j)
            {
                if (i == j)
                    this->matrix[i][j] = 1;
                else
                    this->matrix[i][j] = 0;
            }
        }

        this->name = "E";

    }

    Matrix(string name, int size, long double ** matrix): name{name}, size{size}, matrix{matrix}
    {
    }

    Matrix(const Matrix &matrix)
    {
        this->size = matrix.size;
        this->matrix = new long double * [size];
        for (int i = 0; i < size; ++i)
        {
            this->matrix[i] = new long double[size];
            for (int j = 0; j < size; ++j)
            {
                this->matrix[i][j] = matrix.matrix[i][j];
            }
        }
        this->name = matrix.name;
    }

    Matrix(Matrix&& matrix) : matrix{matrix.matrix}, size{matrix.size}, name{matrix.name}
    {
    }

    Matrix& operator=(Matrix&& right)
    {
        if (this == &right) return *this;
        matrix = right.matrix;
        name = right.name;
        size = right.size;

        right.matrix = nullptr;
        right.size = 0;
        return *this;
    }

    Matrix& operator=(const Matrix& right)
    {
        if (&right == this) return *this;
        delete [] this->matrix;
        this->size = right.size;
        this->matrix = new long double * [size];
        for (int i = 0; i < size; ++i)
        {
            this->matrix[i] = new long double[size];
            for (int j = 0; j < size; ++j)
            {
                this->matrix[i][j] = right.matrix[i][j];
            }
        }

        this->name = right.name;

        return *this;
    }



    Matrix(string name, int size, double num = 0.) : name{name}, size{size}
    {
        matrix = new long double *[size];
        for (int i = 0; i < size; ++i)
        {
            matrix[i] = new long double[size];
            for (int j = 0; j < size; ++j)
            {
                matrix[i][j] = num;
            }
        }
    }

    Matrix(string name, int size, int num) : name{name}, size{size}
    {
        matrix = new long double *[size];
        for (int i = 0; i < size; ++i)
        {
            matrix[i] = new long double[size];
            for (int j = 0; j < size; ++j)
            {
                matrix[i][j] = random() % num;
            }
        }
    }


    Matrix &operator+=(const Matrix &right)
    {
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                this->matrix[i][j] += right.matrix[i][j];
            }
        }

        return *this;
    }

    Matrix &operator-=(const Matrix &right)
    {
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                this->matrix[i][j] -= right.matrix[i][j];
            }
        }

        return *this;
    }

    const Matrix &operator/=(int n) const
    {
        for (int i = 0; i < this->size; ++i)
        {
            for (int j = 0; j < this->size; ++j)
            {
                this->matrix[i][j] /= n;
            }
        }
        return *this;
    }

    const Matrix &operator/(int n) const
    {
        return *this/=n;
    }

    Matrix &operator*=(const Matrix &right)
    {
        double tmp[size][size];
        for (int l = 0; l < size; ++l)
        {
            for (int i = 0; i < size; ++i)
            {
                tmp[l][i] = 0;
            }
        }

        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                for (int k = 0; k < size; ++k)
                {
                    tmp[i][j] += this->matrix[i][k] * right.matrix[k][j];
                }
            }
        }

        for (int m = 0; m < size; ++m)
        {
            for (int i = 0; i < size; ++i)
            {
                this->matrix[m][i] = tmp[m][i];
            }
        }

        return *this;
    }

    Matrix &operator*(const Matrix& right)
    {
        return *this *= right;
    }

    bool operator==(const Matrix &right)
    {
        if (this->size != right.size) return false;
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                if (this->matrix[i][j] != right.matrix[i][j])
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Matrix &right)
    {
        return !(*this == right);
    }

    Matrix &operator+(Matrix right)
    {
        Matrix *result = new Matrix{this->name, this->size};
        *result += *this;
        *result += right;
        return *result;
    }


    Matrix &operator-(Matrix right)
    {
        Matrix *result = new Matrix{this->name, this->size};
        *result -= *this;
        *result -= right;
        return *result;
    }


    void print() const
    {
        cout << this-> name << endl;
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    const Matrix reduceColRow(int row, int col) const
    {
        long double ** temp_matr = new long double *[size-1];
        for (int i = 0; i < size - 1; ++i)
        {
            temp_matr[i] = new long double[size-1];
        }

        int ki = 0;
        for (int i = 0; i < size; i++)
        {
            if(i != row)
            {
                for (int j = 0, kj = 0; j < size; j++)
                {
                    if (j != col)
                    {
                        temp_matr[ki][kj] = matrix[i][j];
                        kj++;
                    }
                }
                ki++;
            }
        }
        Matrix result(name, size - 1, temp_matr);

        return result;
    }



};

double getTrace(const Matrix& matrix)
{
    double result = 0;
    for (int i = 0; i < matrix.size; ++i)
    {
        result += matrix.matrix[i][i];
    }
    return result;
}

double getDeterminant(const Matrix matrix)
{
    if (matrix.size == 1)
        return matrix.matrix[0][0];
    double det = 0;
    double sg = -1;
    for (int i = 0; i < matrix.size; ++i)
    {
        sg *= -1;
        Matrix add = matrix.reduceColRow(i, 0);
        det+= matrix.matrix[i][0]*sg*(getDeterminant(add));
    }
    return det;
}

Matrix reverse(const Matrix& matrix)
{
    int sgn = 1;
    double det = getDeterminant(matrix);

    if (det == 0)
        throw std::exception();
    Matrix adj(matrix.name, matrix.size);
    for (int i = 0; i < matrix.size; ++i)
    {
        for (int j = 0; j < matrix.size; ++j)
        {
            sgn = (((i+j) % 2 == 0) ? 1:-1);
            adj.matrix[i][j] = (sgn * getDeterminant(matrix.reduceColRow(i, j)) )/ det;
        }
    }
    adj = transpose(adj);
    return adj;
}

Matrix transpose(const Matrix& matrix)
{
    Matrix result(matrix.name, matrix.size);
    for (int i = 0; i < matrix.size; ++i)
    {
        for (int j = 0; j < matrix.size; ++j)
        {
            result.matrix[i][j] = matrix.matrix[j][i];
        }
    }
    return result;
}


long double fact(int N)
{
    if(N < 0)
        return 0;
    if (N == 0)
        return 1;
    else
        return N * fact(N - 1);
}


Matrix exp(const Matrix &matrix)
{
    int d = 1, flag;
    Matrix result = Matrix(matrix.size);
    Matrix a = matrix;

    for (int i = 0; i < 1000; i++)
    {
        result = result + (a / fact(d++));
        a *= matrix;

        flag = 1;
        for (int j = 0; j < a.size; j++)
        {
            if (flag == 0)
                break;
            for (int k = 0; k < a.size; k++)
                if (a.matrix[j][k] > eps)
                {
                    flag = 0;
                    break;
                }
        }

        if (flag)
            break;
    }

    return result;
}


ostream& operator<<(ostream &out, const Matrix& matrix)
{
    out << matrix.name << " " << matrix.size << " ";
    for (int i = 0; i < matrix.size; ++i)
    {
        for (int j = 0; j < matrix.size; ++j)
        {
            out << matrix.matrix[i][j] << " ";
        }
    }
    return out;
}

istream &operator>>(istream &in, Matrix &matrix)
{
    in >> matrix.name;
    in >> matrix.size;
    for (int i = 0; i < matrix.size; ++i)
    {
        for (int j = 0; j < matrix.size; ++j)
        {
            in >> matrix.matrix[i][j];
        }
    }

    return in;
}

int main()
{
    srand(time(NULL));

    Matrix matrix("name", 3, 99);
    matrix.print();
    cout << "determinant = " << getDeterminant(matrix) << endl;
    Matrix rev = reverse(matrix);
    cout << "reversed ";
    rev.print();
    Matrix res = rev * matrix;
    cout << "reversed * normal ";
    res.print();
    Matrix transp = transpose(matrix);
    cout << "transpose ";
    transp.print();
    cout << "trace = " << getTrace(matrix) << endl;
    Matrix Exp = exp(matrix);
    cout << "expanent ";
    Exp.print();

    return 0;
}
