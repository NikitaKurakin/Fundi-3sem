using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;
using System.Security.AccessControl;
using Microsoft.Win32;

namespace Praktika_Task_1
{
    public class Drob : IEquatable<Drob>, IComparable, IComparable<Drob>, ICloneable
    {

        public BigInteger _chisl;
        public BigInteger _znam;

        public Drob(BigInteger chisl, BigInteger znam)
        {
            if (znam == 0)
            {
                throw new ArgumentException("Знаменатель не может быть равен 0!");
            }
            else if (chisl == 0)
            {
                _chisl = 0;
                _znam = 1;
                return;
            }

            var gcd = BigInteger.GreatestCommonDivisor(BigInteger.Abs(chisl), BigInteger.Abs(znam));
            _chisl = BigInteger.Abs(chisl / gcd);
            _znam = BigInteger.Abs(znam / gcd);

            if (chisl * znam < 0)
            {
                _chisl *= -1;
            }
        }

        public Drob(BigInteger chisl) : this(chisl, 1) { }

        public Drob(Drob other) : this(other._chisl, other._znam) { }

        #region arifmetic

        public static Drob Add(Drob first, Drob second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(first._chisl * second._znam + first._znam * second._chisl, first._znam * second._znam);
        }

        public static Drob Add(Drob first, BigInteger second)
        {
            if (first is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(first._chisl + second * first._znam);
        }

        public static Drob Sub(Drob first, Drob second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(first._chisl * second._znam - first._znam * second._chisl, first._znam * second._znam);
        }

        public static Drob Sub(Drob first, BigInteger second)
        {
            if (first is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(first._chisl - second * first._znam);
        }

        public static Drob Mult(Drob first, Drob second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(first._chisl * second._chisl, first._znam * second._znam);
        }

        public static Drob Div(Drob first, Drob second)
        {
            if (first is null || second is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(first._chisl * second._znam, first._znam * second._chisl);
        }

        public Drob Pow(int pow)
        {
            if (this is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            if(pow < 0 && this._chisl < 0)
            {
                throw new ArithmeticException("Нельзя возводить ноль в отрицательную степень");
                return this;
            }
           
            return new Drob(BigInteger.Pow(this._chisl, pow), BigInteger.Pow(this._znam, pow));
        }   

        public Drob Abs()
        {
            if (this is null)
            {
                throw new ArgumentException("Дробь не может не существовать");
            }

            return new Drob(BigInteger.Abs(_chisl), _znam);
        }

        public Drob Sqrt()
        {
            if (_chisl < 0)
            {
                throw new ArgumentException("Нельзя взять корень из отрицательного числа");
            }

            Drob eps = new Drob(1, 10000000000000), x = new Drob(1),
                nx = new Drob(1), div2 = new Drob(1, 2);
            for (; ; )
            {
                nx = (x + this / x) * div2;
                if ((x - nx).Abs() < eps)
                    break;
                x = nx;
            }

            return new Drob(x);
        }

        public Drob Sqrt(int count)
        {
            int fl = 0;
            if (_chisl < 0 && count % 2 == 0)
            {
                throw new ArgumentException("It is impossible to extract a root from a negative number");
            }
            else if (_chisl < 0)
            {
                fl = 1;
                this._chisl *= -1;
            }

            Drob eps = new Drob(1, 10000000000), x = new Drob(1),
                nx = new Drob(1), div2 = new Drob(1, count);

            for (; ; )
            {
                nx = (x * (count - 1) + this / x.Pow(count - 1)) * div2;
                if ((x - nx).Abs() < eps)
                    break;
                x = nx;
            }

            if (fl == 1)
            {
                nx *= -1;
                return new Drob(nx);
            }
            else
            {
                return new Drob(nx);
            }
        }

        public Drob Cos()
        {
            Drob eps = new Drob(1, 100000000);
            Drob arg = new Drob(1);
            var ptr = -1;
            Drob sum = new Drob(0);

            BigInteger fact = 2;

            for (var i = 2; i < Int32.MaxValue - 2; i += 2)
            {
                arg += this.Pow(i) * ptr / fact;
                if ((arg - sum).Abs() < eps)
                    break;
                sum = arg;
                fact = fact * (i + 1) * (i + 2);
                ptr *= (-1);
            }

            return arg;
        }

        public Drob Sin()
        {
            Drob eps = new Drob(1, 100000000);
            Drob arg = new Drob(this);
            var ptr = -1;
            Drob sum = new Drob(0);

            BigInteger fact = 6;

            for (var i = 3; i < Int32.MaxValue - 2; i += 2)
            {
                arg += this.Pow(i) * ptr / fact;
                if ((arg - sum).Abs() < eps)
                    break;
                sum = arg;
                fact = fact * (i + 1) * (i + 2);
                ptr *= (-1);
            }

            return arg;
        }

        public static Drob operator +(Drob first, Drob second)
        {
            return Drob.Add(first, second);
        }

        public static Drob operator +(Drob first, BigInteger second)
        {
            return Drob.Add(first, second);
        }

        public static Drob operator -(Drob first, Drob second)
        {
            return Drob.Sub(first, second);
        }

        public static Drob operator -(Drob first, BigInteger second)
        {
            return Drob.Sub(first, second);
        }

        public static Drob operator *(Drob first, Drob second)
        {
            return Drob.Mult(first, second);
        }

        public static Drob operator *(Drob first, BigInteger second)
        {
            return Drob.Mult(first, new Drob(second, 1));
        }

        public static Drob operator /(Drob first, Drob second)
        {
            return Drob.Div(first, second);
        }

        public static Drob operator /(Drob first, BigInteger second)
        {
            return Drob.Div(first, new Drob(second, 1));
        }

        #endregion

        #region ToString

        public override string ToString()
        {
            return (_chisl.ToString() + "/" + _znam.ToString());
        }

        public string ToString(int count)
        {
            var result = new StringBuilder();
            if (_chisl < 0)
            {
                result.Append('-');
            }
            BigInteger num = BigInteger.Abs(_chisl), lastNumb = num / _znam;
            result.Append(BigInteger.Abs(lastNumb)).Append(',');
            lastNumb = (num - _znam * lastNumb) * 10;
            for (int i = 0; i < count; i++)
            {
                result.Append(lastNumb / _znam);
                lastNumb = (lastNumb - (lastNumb / _znam) * _znam) * 10;
            }

            return result.ToString();
        }

        #endregion

        #region CompareTo

        public int CompareTo(object obj)
        {
            if (obj is null)
            {
                throw new ArgumentNullException(nameof(obj));
            }
            if (obj is Drob other)
            {
                return CompareTo(other);
            }

            throw new ArgumentException("invalid type");
        }

        public int CompareTo(Drob other)
        {
            if (other is null)
            {
                throw new ArgumentNullException("Дроби не существует");
            }

            return (this._chisl * other._znam).CompareTo(this._znam * other._chisl);
        }

        #endregion

        #region Clone
        public object Clone()
        {
            return new Drob(_chisl, _znam);
        }
        #endregion

        #region Equals

        public override bool Equals(object obj)
        {
            if (obj is null)
            {
                throw new ArgumentNullException(nameof(obj));

            }

            if (obj is Drob other)
                return Equals(other);

            return false;
        }

        public bool Equals(Drob other)
        {
            if (other is null)
                throw new ArgumentNullException(nameof(other));

            return (_chisl == other._chisl && _znam == other._znam);
        }

        public static bool operator ==(Drob first, Drob second)
        {
            if (first is null && second is null)
            {
                return true;
            }
            else if (first is object && second is null)
            {
                return first.Equals(second);
            }
            else
            {
                return second.Equals(first);
            }
        }

        public static bool operator !=(Drob first, Drob second)
        {
            return !(first == second);
        }

        public static bool operator >(Drob first, Drob second)
        {
            if (first is null)
            {
                throw new ArgumentNullException(nameof(first));
            }

            if (first.CompareTo(second) == 1)
            {
                return true;
            }

            return false;
        }

        public static bool operator <(Drob first, Drob second)
        {
            if (first is null)
            {
                throw new ArgumentNullException(nameof(first));
            }

            if (first.CompareTo(second) == -1)
            {
                return true;
            }

            return false;
        }

        public static bool operator >=(Drob first, Drob second)
        {
            return !(first < second);
        }

        public static bool operator <=(Drob first, Drob second)
        {
            return !(first > second);
        }

        #endregion

    }
}
