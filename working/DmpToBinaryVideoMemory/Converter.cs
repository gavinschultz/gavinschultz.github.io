using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace DmpToBinaryVideoMemory
{
    class Converter
    {
        const int DEFAULT_SCREEN_WIDTH = 0x20;
        private byte[] _memory;

        public void Convert(Stream s)
        {
            var sb = new StringBuilder();
            using (var sr = new StreamReader(s, Encoding.ASCII))
            {
                while (!sr.EndOfStream)
                {
                    var line = sr.ReadLine();
                    line = Regex.Replace(line, @"[0-9A-Fa-f]+:\s*", "");
                    line = line.Replace(" ", "");
                    sb.Append(line);
                }
            }

            LoadMemoryFromStringBuilder(sb);
        }

        public void Save(string filename, ConvertOptions options)
        {
            File.WriteAllText(filename, ToString(options));
        }

        public void PrintToConsole(ConvertOptions options)
        {
            Console.WriteLine(ToString(options));
        }

        public string ToString(ConvertOptions options)
        {
            if (options == null)
                options = new ConvertOptions();
            if (_memory == null)
                return String.Empty;

            var sb = new StringBuilder(_memory.Length * 8);
            for (int i = 0; i < _memory.Length; i++)
            {
                if (i != 0 && i % options.ScreenByteWidth == 0)
                {
                    sb.AppendLine();
                }
                var byteAsString = String.Format("{0,8}", System.Convert.ToString(_memory[i], 2));
                sb.Append(byteAsString);
            }

            string result = sb.ToString();
            if (options.HideZeros)
            {
                result = result.Replace("0", " ");
            }

            return result;
        }

        private void LoadMemoryFromStringBuilder(StringBuilder sb)
        {
            if (sb.Length % 2 != 0)
            {
                throw new InvalidOperationException(String.Format("Data length of does not have an even character count; ensure all bytes are in 2-byte hex format. Length = {0}", sb.Length));
            }

            _memory = Enumerable
                .Range(0, sb.Length)
                .Where(x => x % 2 == 0)
                .Select(x => System.Convert.ToByte(new String(new char[] { sb[x], sb[x + 1] }), 16))
                .ToArray();
        }
    }
}
