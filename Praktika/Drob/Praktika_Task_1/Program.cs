using System;

namespace Praktika_Task_1
{
    class Program
    {
        static void Main(string[] args)
        {
            Drob a = new Drob(1, 2);
            Drob b = new Drob(1, 4);
            Drob c = new Drob(-1, 1);

            Console.WriteLine("Arithmetic with a = {0} and b = {1}:", a, b);
            Console.WriteLine("Sum: {0}", a + b);
            Console.WriteLine("Sub: {0}", a - b);
            Console.WriteLine("Mult: {0}", a * b);
            try
            {
                Console.WriteLine("Div: {0} {1}", a / b, Environment.NewLine);
            }
            catch (DivideByZeroException ex)
            {
                Console.WriteLine(ex.Message);
            }
            Console.WriteLine("Pow ({0})^(0): {1}", a, a.Pow(0));
            Console.WriteLine("({0}).ToString(int count): {1}", c, c.ToString(10));
            Console.WriteLine("ToString(): {0}", a);
            Console.WriteLine("({0}).CompareTo({1}) = {2}", a, b, a.CompareTo(b));
            Console.WriteLine("Equals: {0} and {1} - {2}", a, b, a.Equals(b));
            Console.WriteLine("1/4 < 1/5 = {0}", new Drob(1, 4) < new Drob(1, 5));
            Console.WriteLine("1/4 > 1/5 = {0}", new Drob(1, 4) > new Drob(1, 5));
            Console.WriteLine("1/4 <= 1/5 = {0}", new Drob(1, 4) <= new Drob(1, 5));
            Console.WriteLine("1/4 <= 1/4 = {0}", new Drob(1, 4) <= new Drob(1, 4));
            Console.WriteLine("1/4 >= 1/5 = {0}", new Drob(1, 4) >= new Drob(1, 5));

            Console.WriteLine("Sqrt {0} ", a.Sqrt().ToString(100));
            Console.WriteLine("Cos {0} ", a.Cos().ToString(5));
            Console.WriteLine("Sin {0} ", a.Sin().ToString(5));
            Console.WriteLine("SqrtN {0}", b.Sqrt(3).ToString(10));

        }

    }

}
