using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DmpToBinaryVideoMemory
{
    class ConvertOptions
    {
        public bool HideZeros { get; set; }
        public int ScreenByteWidth { get; set; }

        public static ConvertOptions ForTRS80()
        {
            return new ConvertOptions
            {
                ScreenByteWidth = 0x20
            };
        }

        public ConvertOptions WithHideZeros()
        {
            HideZeros = true;
            return this;
        }
    }
}
