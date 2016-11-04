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
                scene.Add(new LightSource(new Vector3(0, 100, 0), new RGB(1, 1, 1)).Rotate(0, 0, 90));
                scene.Add(new Sphere(new Vector3(0, 0, 0), 10)
                {
                    Surface = new Texture()
                    {
                        Pigment = new Pigment(
                            new Agate(
                                0.5,
                                new povclr.ColorMap(
                                    new ColorMapEntry(0.0, new RGBFT(0, 0, 0)),
                                    new ColorMapEntry(0.3, new RGBFT(1, 0.5, 1)))))
                    }
                }.
                Rotate(2, 60, 124).
                Translate(0, 10, 0).
                Scale(2));

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
