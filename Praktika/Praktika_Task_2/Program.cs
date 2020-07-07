using System;
using Praktika_Task_1;

namespace Praktika_Task_2
{
    class Program
    {
        static void Main(string[] args)
        {
            var a = new Complex(new Drob(1, -2), new Drob(1, 8));
            var b = new Complex(new Drob(1, 2), new Drob(1, 6));
            var c = a - b;
            Console.WriteLine("Sum: {0} + {1} = {2}", a, b, a + b);
            Console.WriteLine("Sub: {0} - {1} = {2}", a, b, a - b);
            Console.WriteLine("Mult: {0} * {1} = {2}", a, b, a * b);
            try
            {
                Console.WriteLine("Div: {0} / {1} = {2}", a, b, a / b);
            }
            catch (DivideByZeroException e)
            {
                Console.WriteLine(e.Message);
            }
            Console.WriteLine("Arg.ToString: {0} = {1}", b, b.Argument().ToString(20));
            Console.WriteLine("Abs: {0} = {1}", a, a.Abs().ToString(10));
            Console.WriteLine("Pow: ({0})^10 = {1}", a, a.Pow(10));

            var pow = 5;
            Complex[] arr = a.SqrtN(pow);

            //отловить отрицательный корень
            for (int i = 0; i < pow; i++)
                Console.WriteLine("Sqrt number - {0}: {1}", i, arr[i]);
            Console.WriteLine("({0}).CompareTo({1}) = {2}", a, b, a.CompareTo(b));
            Console.WriteLine("Equals: {0} and {1} - {2}", a, b, a.Equals(b));
        }
    }
}
