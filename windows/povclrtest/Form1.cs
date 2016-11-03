using povclr;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading;
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
            private object lockObject = new object();

            public int PixelsProcessed;
            public int NumberPixels;

            public void Initialize(int width, int height)
            {
                NumberPixels = width * height;
                PixelsProcessed = 0;

                lock (lockObject)
                {
                    bmp = new Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                    bmpData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadWrite, bmp.PixelFormat);
                }
            }

            public void SetPixel(int x, int y, double red, double green, double blue, double alpha)
            {
                byte* baseptr = (byte*)bmpData.Scan0.ToPointer();
                byte* ptr = baseptr + (y * bmpData.Stride + x * (32 / 8));

                if (alpha < 0) { alpha = 0; } else if (alpha > 1) { alpha = 1; }
                if (red < 0) { red = 0; } else if (red > 1) { red = 1; }
                if (green < 0) { green = 0; } else if (green > 1) { green = 1; }
                if (blue < 0) { blue = 0; } else if (blue > 1) { blue = 1; }

                // Little endian...
                *ptr++ = (byte)(blue * 255);
                *ptr++ = (byte)(green * 255);
                *ptr++ = (byte)(red * 255);
                *ptr++ = (byte)(alpha * 255);

                Interlocked.Increment(ref PixelsProcessed);
            }

            public void Finish()
            {
                lock (lockObject)
                {
                    if (bmpData != null)
                    {
                        bmp.UnlockBits(bmpData);
                        bmpData = null;
                    }
                }
            }

            public void Dispose()
            {
                Finish();
            }

            public unsafe Image Image
            {
                get
                {
                    if (bmpData == null)
                    {
                        return bmp;
                    }
                    else
                    {
                        lock (lockObject)
                        {
                            if (bmpData == null)
                            {
                                return bmp;
                            }
                            else
                            {
                                var tmp = new Bitmap(bmp.Width, bmp.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                                var tmpData = tmp.LockBits(new Rectangle(0, 0, tmp.Width, tmp.Height), ImageLockMode.ReadWrite, bmp.PixelFormat);

                                byte* tmpStart = (byte*)tmpData.Scan0;
                                byte* srcStart = (byte*)bmpData.Scan0;
                                int count = bmpData.Stride * tmp.Height;
                                for (int i = 0; i < count; ++i)
                                {
                                    tmpStart[i] = srcStart[i];
                                }
                                tmp.UnlockBits(tmpData);
                                return tmp;
                            }
                        }
                    }
                }
            }
        }

        public class ASyncRendering
        {
            public ASyncRendering(Scene scene)
            {
                this.scene = scene;
                this.result = null;
            }

            private RenderCallback callback;
            private Scene scene;
            private Image result;

            public bool Rendering()
            {
                return callback != null;
            }

            public double PercentCompleted()
            {
                Thread.MemoryBarrier();
                var tmp = callback;
                if (tmp == null) { return 100.0; } 
                else if (tmp.NumberPixels == 0) { return 0; }
                else { return 100.0 * (double)tmp.PixelsProcessed / (double)tmp.NumberPixels; }
            }

            public Image CurrentStatus()
            {
                Thread.MemoryBarrier();
                var callback = this.callback;
                return callback == null ? result : callback.Image;
            }

            private void RenderASync()
            {
                try
                {
                    scene.Render(callback);

                    result = callback.Image;
                }
                finally
                {
                    callback.Dispose();
                    callback = null;
                }
            }

            public void Render()
            {
                // Initialize here so rendering will reflect the correct values:
                this.result = null;

                callback = new RenderCallback();
                Thread.MemoryBarrier();

                // Start the rendering thread:
                Thread t = new Thread(() => RenderASync())
                {
                    IsBackground = true,
                    Name = "POV-Ray render thread",
                    Priority = ThreadPriority.Normal
                };
                t.Start();
            }
        }

        public Form1()
        {
            InitializeComponent();
        }

        private ASyncRendering renderer = null;

        private void button1_Click(object sender, EventArgs e)
        {
            if (renderer == null)
            {
                var settings = new Settings(renderResult.Width, renderResult.Height);
                var scene = new Scene(settings);
                scene.Add(new Camera(new Vector3(0, 20, -100), new Vector3(0, 0, 0)));
                scene.Add(new LightSource(new Vector3(0, 100, 0), new RGB(1, 1, 1)));
                scene.Add(new LightSource(new Vector3(0, 100, 0), new RGB(1, 1, 1)).Rotate(0,0,90));
                scene.Add(new Sphere(new Vector3(0, 0, 0), 10)
                {
                    Surface = new Texture() { Pigment = new Pigment(new RGBFT(1, 0, 0)) }
                }.Translate(0,10,0).Scale(2));

                renderer = new ASyncRendering(scene);
                renderer.Render();

                /*
                using (RenderCallback target = new RenderCallback())
                {
                    scene.Render(target);

                    pictureBox1.Image = target.Image;
                }
                */

                renderUpdate.Start();
            }
        }

        private void renderUpdate_Tick(object sender, EventArgs e)
        {
            var tmp = this.renderer;
            this.renderProgress.Value = (int)Math.Ceiling(tmp.PercentCompleted());
            this.renderResult.Image = tmp.CurrentStatus();

            if (!tmp.Rendering())
            {
                renderUpdate.Stop();
                renderer = null;
            }
        }
    }
}
