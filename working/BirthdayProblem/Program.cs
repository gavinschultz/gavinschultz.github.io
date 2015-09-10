using Numerics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace BirthdayProblem
{
    class Program
    {
        static void Main(string[] args)
        {
            int n = 32;
            int k = 3;
            int m = 256;

            // (m - nk - 1)!
            var top = Factorial(m - n * k - 1);

            // m^n-1
            var bottom1 = BigInteger.Pow(m, n - 1);

            // (m - n(k+1))!
            var bottom2 = Factorial(m - n * (k + 1));

            var bottom = BigInteger.Multiply(bottom1, bottom2);

            Console.WriteLine("Top   :{0}\nBottom:{1}", top, bottom);
            Console.WriteLine("Top - bottom = {0}", BigInteger.Subtract(bottom, top));

            var rational = new BigRational(top, bottom);
            var result = (double)(1 - rational);

            Console.WriteLine(result);
            Console.WriteLine((1 - rational));
        }

        static BigInteger Factorial(BigInteger i)
        {
            if (i == 1)
                return new BigInteger(1);
            BigInteger result = i * Factorial(i - 1);
            return result;
        }
    }
}
