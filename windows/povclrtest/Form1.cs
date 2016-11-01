using povclr;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace povclrtest
{
    public partial class Form1 : Form
    {
        public unsafe class RenderCallback : povclr.RenderTarget, IDisposable
        {
            public RenderCallback()
            {
                bmp = null;
                bmpData = null;
            }

            private Bitmap bmp;
            private BitmapData bmpData;

            public void Initialize(int width, int height)
            {
                bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                bmpData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadWrite, bmp.PixelFormat);
            }

            public void SetPixel(int x, int y, double red, double green, double blue, double alpha)
            {
                byte* baseptr = (byte*)bmpData.Scan0.ToPointer();
                byte* ptr = baseptr + y * bmpData.Stride + x * (32 / 8);

                if (alpha < 0) { alpha = 0; } else if (alpha > 1) { alpha = 1; }
                if (red < 0) { red = 0; } else if (red > 1) { red = 1; }
                if (green < 0) { green = 0; } else if (green > 1) { green = 1; }
                if (blue < 0) { blue = 0; } else if (blue > 1) { blue = 1; }

                *ptr++ = (byte)(alpha * 255);
                *ptr++ = (byte)(red * 255);
                *ptr++ = (byte)(green * 255);
                *ptr++ = (byte)(blue * 255);
            }

            public void Finish()
            {
                if (bmpData != null)
                {
                    bmp.UnlockBits(bmpData);
                    bmpData = null;
                }
            }

            public void Dispose()
            {
                if (bmpData != null)
                {
                    bmp.UnlockBits(bmpData);
                    bmpData = null;
                }

                if (bmp != null)
                {
                    bmp.Dispose();
                    bmp = null;
                }
            }

            public Image Image { get { return bmp; } }
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var settings = new Settings();
            var scene = new Scene();
            scene.Add(new Camera(new Vector3(0, 20, -100), new Vector3(0, 0, 0)));
            scene.Add(new LightSource(new Vector3(0, 100, 0)));
            scene.Add(new Sphere(new Vector3(0, 0, 0), 10));

            RenderCallback target = new RenderCallback();
            scene.Render(target);

            pictureBox1.Image = target.Image;
        }
    }
}
