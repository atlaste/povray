using povray;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace povray.test
{
    public class TestScene
    {
        public void Render(Scene scene)
        {
            scene.Add(new Camera(new Vector3(60, 170, -360), new Vector3(50, 125, 0)));
            scene.Add(new LightSource(new Vector3(1500, 1000, 2300), new RGB(1,1,1)));
        }
    }
}
