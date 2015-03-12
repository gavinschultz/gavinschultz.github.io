using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace bitmap_to_grid
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            int i = 0;
            string path = args[0];
            bool copyToClipboard = false;
            bool hideZero = false;
            if (args.Length > 1)
            {
                if (args[1] == "-clipboard")
                {
                    copyToClipboard = true;
                }
            }
            if (args.Length > 2)
            {
                if (args[2] == "-hidezero")
                {
                    hideZero = true;
                }
            }
            string text = "";
            byte[] raw_bytes = File.ReadAllBytes(path);
            byte[] transformed_bytes = new byte[raw_bytes.Length];
            text += "\\draw[dotted,step=1cm,color=gray] (-8,-8) grid (8,8);\r\n";
            text += "\\draw[step=2cm,color=black,semithick] (-8-0.001,-8-0.001) grid (8,8);\r\n";
            text += "\\matrix[matrix of nodes,nodes in empty cells,nodes={inner sep=0pt,text width=1cm,align=center,anchor=center,minimum height=1cm}]{\r\n";

            try
            {
                for (int bi = 0; bi < raw_bytes.Length; bi++)
                {
                    int index = (bi * 2) - (bi % 2) + (bi / 32) * 28 - (bi / 16) * 30;
                    transformed_bytes[bi] = raw_bytes[index];
                }
                //for (int bi = 0; bi < raw_bytes.Length - 16; bi++)
                //{
                //    if (bi % 4 < 2)
                //    {
                //        transformed_bytes[bi / 2 + bi % 4] = raw_bytes[bi];
                //        // [0,1] = [0,1] = [(0 / 32) * 32 + (0 % 4), (1 / 32) * 32 + (1 % 4)] = [0 + 0, 0 + 1]
                //        // [2,3] = [4,5] = [(2 / 32) * 32 + (2 % 4), (3 / 32) * 32 + (3 % 4)] = [0 + 2, 0 + 3]
                //        // [4,5] = [8,9]
                //        // [6,7] = [12,13]
                //        // [8,9] = [16,17]
                //        // [10,11] = [20,21]
                //        // [12,13] = [24,25]
                //        // [14,15] = [28,29]
                //        // [16,17] = [2,3] = [(16 / 32) * 32 + (16 % 4) + 2, (17 / 32) * 32 + (17 % 4) + 2] = [0 + 0 + 2, 0 + 1 + 2]
                //        // [18,19] = [6,7] = [(18 / 32) * 32 + (18 % 4) + 2, (19 / 32) * 32 + (19 % 4) + 2] = [0 + 2 + 2
                //        // [20,21] = [10,11]
                //        // [22,23] = [14,15]
                //        // [24,25] = [18,19]
                //        // [26,27] = [22,23]
                //        // [28,29] = [26,27]
                //        // [30,31] = [30,31]
                //        // [32,33] = [32,33] = [(32 / 32) * 32 + (32 % 4), (33 / 32) * 32 + (33 % 4)] = [1*32 + 0, 1*32 + 1]
                //        // [34,35] = [36,37] = [(34 / 32) * 32 + (34 % 4), (35 / 32) * 32 + (35 % 4)] = [1*32 + 2, 1*32 + 3]
                //    }
                //    else
                //    {
                //        transformed_bytes[bi / 2 + bi % 4 + 2] = raw_bytes[bi];
                //        // [0,1] = [2,3]
                //        // [2,3] = [6,7]
                //        // [4,5] = [10,11]
                //        // [6,7] = [14,15]
                //    }
                //}

                foreach (byte b in transformed_bytes)
                {
                    if (i % 16 == 0)
                    {
                        text += "  ";
                    }
                    if (hideZero && b == 0
                        && !(i > 16 && i % 2 == 1 && i % 32 > 16 && transformed_bytes[i-16] > 0)) // if 0 on every second line and other items in this square set
                        text += "    ";
                    else
                        text += String.Format(@"\${0:X2} ", b);

                    i++;

                    if (i % 16 == 0)
                    {
                        text += "\\\\\r\n";
                    }
                    else
                    {
                        text += "& ";
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }

            text += "};\r\n";

            if (copyToClipboard)
            {
                System.Windows.Forms.Clipboard.SetText(text);
            }

            Console.ReadLine();
        }
    }
}
