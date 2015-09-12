using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace DmpToBinaryVideoMemory
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Usage();
            }

            var filename = args[0];
            if (!File.Exists(filename))
            {
                FatalError("File {0} does not exist.", filename);
            }

            try
            {
                var options = ConvertOptions.ForTRS80();
                options.HideZeros = true;
                var converter = new Converter();
                using (var fs = new FileStream(filename, FileMode.Open, FileAccess.Read))
                {
                    converter.Convert(fs);
                }
                converter.Save(Path.ChangeExtension(filename, ".binary.txt"), options);
                Console.WriteLine(converter.ToString(options));
            }
            catch (Exception ex)
            {
                FatalError(ex);
            }
        }

        static void FatalError(Exception ex) { FatalError(ex.ToString()); }
        static void FatalError(string format, params string[] args)
        {
            Console.WriteLine(format, args);
            Environment.Exit(1);
        }

        static void Usage()
        {
            Console.WriteLine("Usage: dmpvideomem {filename}");
            Environment.Exit(0);
        }
    }
}
