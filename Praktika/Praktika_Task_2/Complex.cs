using Praktika_Task_1;
using System;
using System.Collections.Generic;
using System.Text;

namespace Praktika_Task_2
{
    public class Complex : IComparable, IComparable<Complex>, ICloneable, IEquatable<Complex>
    {

        private static readonly Drob Pi = new Drob(4272943, 1360120);

        private readonly Drob _real;
        private readonly Drob _imag;
        private readonly Drob Eps = new Drob(1, 10000000000);

        public Complex(Drob real, Drob imag)
        {
            if (real is null || imag is null)
            {
                throw new ArgumentNullException("Fraction can't be null");
            }
            _real = real;
            _imag = imag;
        }

        public Complex(Complex other) : this(other._real, other._imag) { }
        public Complex(Drob real) : this(real, new Drob(0)) { }

        #region arithmetic

        public static Complex Add(Complex first, Complex second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentNullException();
            }

            return new Complex(first._real + second._real, first._imag + second._imag);
        }

        public static Complex Sub(Complex first, Complex second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentNullException();
            }

            return new Complex(first._real - second._real, first._imag - second._imag);
        }

        public static Complex Multiplication(Complex first, Complex second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentNullException();
            }

            return new Complex(first._real * second._real - first._imag * second._imag,
                first._real * second._imag + first._imag * second._real);
        }

        public static Complex Division(Complex first, Complex second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentNullException();
            }

            if (second._real == 0 && second._imag == 0)
            {
                throw new DivideByZeroException();
            }

            return new Complex((first._real * second._real + first._imag * second._imag) /
                               (second._real.Pow(2) + second._imag.Pow(2)),
                        (first._imag * second._real - first._real * second._imag) /
                                (second._real.Pow(2) + second._imag.Pow(2)));
        }

        public Drob Abs()
        {
            return (_real.Pow(2) + _imag.Pow(2)).Sqrt();
        }

        public Drob Argument()
        {
            var epsilon = new Drob(1, 1000000000);

            Drob arg = new Drob(_imag / _real);
            var x = _imag / _real;
            Drob ptr = new Drob(-1);
            Drob sum = new Drob(0);

            //невозможно разложить в ряд тангенс значение = 1
            if (x.Abs() == new Drob(1))
                throw new ArithmeticException("There is no tg(x)");

            if (x.Abs() > new Drob(1))
            {
                x = new Drob(x._chisl, x._znam);
                arg = x;

                for (int i = 3; i < Int32.MaxValue - 2; i += 2)
                {
                    arg += ptr * x.Pow(i) / i;
                    if ((arg.Abs() - sum.Abs()).Abs() < epsilon)
                        break;
                    sum = arg;
                    ptr = ptr * (-1);
                }

                return new Drob((Pi / 2 - arg));
            }

            for (int i = 3; i < Int32.MaxValue - 2; i += 2)
            {
                arg += ptr * x.Pow(i) / i;
                if ((arg.Abs() - sum.Abs()).Abs() < epsilon)
                    break;
                sum = arg;
                ptr = ptr * (-1);
            }

            return arg;
        }

        public Complex[] SqrtN(int pow)
        {
            if (pow < 0)
                throw new ArgumentException(nameof(pow));
            Complex[] arr = new Complex[pow];

            for (int i = 0; i < pow; i++)
            {
                arr[i] = new Complex((this.Abs().Sqrt(pow) + Pi * 2 * i) * (this.Argument() / pow).Cos(),
                    (this.Abs().Sqrt(pow) + Pi * 2 * i) * (this.Argument() / pow).Sin());
            }

            return arr;
        }

        public Complex Pow(int pow)
        {
            return new Complex(this.Abs().Pow(pow) * (this.Argument() * pow).Cos(),
                              this.Abs().Pow(pow) * (this.Argument() * pow).Sin());
        }

        public static Complex operator +(Complex a, Complex b)
        {
            return Add(a, b);
        }

        public static Complex operator -(Complex a, Complex b)
        {
            return Sub(a, b);
        }

        public static Complex operator *(Complex first, Complex second)
        {
            return Multiplication(first, second);
        }

        public static Complex operator /(Complex first, Complex second)
        {
            return Division(first, second);
        }

        #endregion

        #region Clone

        public object Clone()
        {
            return new Complex(_real, _imag);
        }

        #endregion

        #region Equal

        public override bool Equals(object obj)
        {
            if (obj is null)
                return false;

            if (obj is Complex other)
                return Equals(other);

            return false;
        }

        public bool Equals(Complex other)
        {
            if (other is null)
                return false;

            return (_real == other._real && _imag == other._imag);
        }

        #endregion

        #region CompareTo

        public int CompareTo(object obj)
        {
            if (obj is null)
            {
                throw new ArgumentNullException("obj can't be null");
            }
            if (obj is Complex other)
            {
                return CompareTo(other);
            }

            throw new ArgumentException("invalid type");
        }

        public int CompareTo(Complex other)
        {
            if (other is null)
            {
                throw new ArgumentNullException("Complex can't be null");
            }

            return this.Abs().CompareTo(other.Abs());
        }

        #endregion

        public override string ToString()
        {
            return $"({_real.ToString(3)}) + ({_imag.ToString(3)}) * i";
        }

    }
}
