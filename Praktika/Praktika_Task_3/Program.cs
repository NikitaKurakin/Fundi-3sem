using System;
using System.Collections.Generic;
using System.Text;
using Praktika_Task_1;

namespace Praktika_Task_3
{
    class Programm
    {

        public static Drob zero = new Drob(0);
        public delegate Drob DelegateTest(Drob x);
        public static void Main(string[] args)
        {
            DelegateTest method = Func5;

            Console.WriteLine(Dihot(new Drob(-3), new Drob(5),
                method, new Drob(1, 1000000000000)).ToString(10));
        }

        #region functions

        public static Drob Func1(Drob x)
        {
            return new Drob((x - 2) * x);
        }

        public static Drob Func2(Drob x)
        {
            return new Drob((x.Pow(3) * 3 + x.Pow(2) * 7 + 5));
        }

        public static Drob Func3(Drob x)
        {
            return new Drob(x.Pow(5) * (x.Pow(2) - x * 3) + 25);
        }

        public static Drob Func4(Drob x)
        {
            return new Drob((x + x) / (x));
        }

        public static Drob Func5(Drob x)
        {
            return new Drob((x.Pow(2) + x * 5 + 6) / (x - 1));
        }

        #endregion

        public static Drob Dihot(Drob first, Drob second, DelegateTest temp, Drob eps)
        {
            if (first == null || second == null)
                throw new ArgumentNullException();

            if (temp(first) * temp(second) > zero)
                throw new ArgumentException("Root doesn't exist");

            if (temp(first) == zero)
                return first;

            if (temp(second) == zero)
                return second;

            if (first > second)
            {
                var ptr = first;
                first = second;
                second = ptr;
            }

            while ((second - first).Abs() > eps)
            {
                Drob res = (first + second) / 2;
                if (temp(second) * temp(res) < zero)
                    first = res;
                else if (temp(first) * temp(res) < zero)
                    second = res;
                else
                {
                    return res;
                }
            }
            return (first + second) / 2;
        }

    }
}