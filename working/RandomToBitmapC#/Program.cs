using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace RandomToBitmap
{
    class Program
    {
        static void Main(string[] args)
        {
            var outputPath = args[0];
            var random = new Random();
            using (var bitmap = new Bitmap(512, 512, System.Drawing.Imaging.PixelFormat.Format32bppArgb))
            {
                for (int y = 0; y < 512; y++)
                {
                    for (int x = 0; x < 512; x++)
                    {
                        var value = random.Next(0, 256); // 0 inclusive, 256 exclusive
                        bitmap.SetPixel(x, y, Color.FromArgb(value, value, value));
                    }
                }
                bitmap.Save(outputPath, ImageFormat.Bmp);
            }
        }
    }
}
