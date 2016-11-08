using povray;
using povray.Materials;
using povray.Materials.Patterns;
using povray.Shapes;
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
            scene.Add(new LightSource(new Vector3(1500, 1000, 2300), new RGB(1)));

            var plane = new Plane(new Vector3(0, 1, 0), 0)
            {
                Texture = new Texture() { Pigment = new Pigment(new RGBFT(0.4f, 0.7f, 0.3f)) }
            };
            scene.Add(plane);

            // --------------
            // Some textures:
            // --------------

            var concrete = new Texture()
            {
                Pigment = new Pigment(new RGBFT(1)),
                TNormal = new Normal(new Granite() /* .Scale(8) */ ),
                Finish = new FinishBuilder().Specular(0.5f).Roughness(0.15f).Diffuse(1)
            };

            var plastic = new Texture()
            {
                Pigment = new Pigment(new RGBFT(1, 0.8f, 0.9f)),
                Finish = new FinishBuilder().Specular(0.5f).Roughness(0.15f).Diffuse(1)
            };

            var metal = new Texture()
            {
                Pigment = new Pigment(new RGBFT(0.75f)),
                Finish = new FinishBuilder().Ambient(0.3f).Brilliance(3).Diffuse(0.4f).Metallic(1.0f).Specular(0.7f).Roughness(1 / 60.0f).Reflection(new RGB(0.25f))
            };

            var glassExt = new Texture()
            {
                Pigment = new Pigment(new RGBFT(1, 1, 1, 1)),
                Finish = new FinishBuilder().Specular(false, 0.6f).Roughness(0.002f).Ambient(0).Diffuse(0.1f).Reflection(new RGB(0.05f), new RGB(1), 1).ConserveEnergy()
            };
            var glassInt = new Interior() { IOR = 1.5f, FadeDistance = 2.0f, FadePower = 1001, FadeColour = new RGB(0.184314f, 0.309804f, 0.184314f) };

            // TODO FIXME: Not all wood texture modifiers have been implemented in POV.Net... color maps and turbulence in particular here.
            var wood = new Texture()
            {
                Pigment = new Pigment(new RGBFT(133/255.0f, 87/255.0f, 35/255.0f))
            };

            // Some constants of our structure:
            float AngleDelta = 10; // Difference in angles; 10 means we use 5 degrees per section.

            int SectionsY = 25;
            float SectionHeight = 5;
            float SectionDepth = 15;
            float SectionDepth2 = 12;
            float SectionAngle = AngleDelta / 2;

            float ComplexSlack = 5;
            float ComplexRadius = 250;
            float ComplexHeight = SectionHeight * SectionsY;

            float RoofHeight = (float)(Math.Sin(2 * Math.PI / 180) * 2 * ComplexRadius);

            // ----------------------
            // Create base structure:
            // ----------------------

            var concreteParts = new Union();

            // Base
            concreteParts.Add(new Cone(new Vector3(0, 0.01f, 0), ComplexRadius, new Vector3(0, 10f, 0), 10));

            // Cylon
            concreteParts.Add(new Cylinder(new Vector3(0), new Vector3(0, ComplexHeight + RoofHeight / 2, 0), 10));

            // Vertical pillars
            for (float angle = 0; angle < 360; angle += AngleDelta / 2)
            {
                concreteParts.Add(
                    new Box(new Vector3(ComplexRadius - ComplexSlack - SectionDepth - 0.25f, 0, -1),
                            new Vector3(ComplexRadius - ComplexSlack - SectionDepth + 0.25f, ComplexHeight, 1)).Rotate(0, angle, 0));
            }

            // Frame between pillars
            for (float height = 0; height < ComplexHeight; height += SectionHeight)
            {
                concreteParts.Add(new Difference(
                    new Cylinder(new Vector3(0, height, 0), new Vector3(0, height + 0.5f, 0), ComplexRadius - ComplexSlack - SectionDepth + 0.25f),
                    new Cylinder(new Vector3(0, height - 1, 0), new Vector3(0, height + 1.5f, 0), ComplexRadius - ComplexSlack - SectionDepth - 0.25f)
                ));
            }

            // Pikes on the frame
            for (float height = SectionHeight; height < ComplexHeight; height += SectionHeight)
            {
                for (float angle = 0; angle < 360; angle += AngleDelta / 2)
                {
                    /* TODO FIXME: Implement prism shape

                     prism { 
                        linear_sweep linear_spline
                        -1, 1,
                        4, <0,0>, <0, 0.5>, <SectionDepth * 0.6, 0.5>, <0, 0>
                        rotate x * 270
                        translate x * (ComplexRadius - ComplexSlack - SectionDepth)
                        rotate y * Angle
                    }
                       
                    // 2x: 
                    object { Spike rotate y * (Angle + AngleDelta / 6 * 1) translate y * Height }
                    object { Spike rotate y * (Angle + AngleDelta / 6 * 2) translate y * Height }

                    */
                }
            }

            // Concrete just below the roof
            float h2 = ComplexHeight - SectionHeight;
            concreteParts.Add(new Difference(
                new Cylinder(new Vector3(0, h2, 0), new Vector3(0, ComplexHeight, 0), ComplexRadius - ComplexSlack - SectionDepth + 0.25f),
                new Cylinder(new Vector3(0, h2 - 1, 0), new Vector3(0, ComplexHeight + 1, 0), ComplexRadius - ComplexSlack - SectionDepth - 0.25f)
            ));

            concreteParts.Texture = concrete;

            scene.Add(concreteParts);

            // ------------------------------
            // Metal construction at the top: 
            // ------------------------------

            for (float angle = 0; angle < 360; angle += AngleDelta)
            {
                var construction = new Union();

                construction.Add(new Cylinder(new Vector3(0), new Vector3(ComplexRadius, 0, 0), 0.5f).Rotate(0, 0, 2));
                construction.Add(new Cylinder(new Vector3(0), new Vector3(ComplexRadius, 0, 0), 0.5f).Rotate(0, 0, -2));

                float DistPerM = (float)Math.Sin(2 * Math.PI / 180);
                float BeamDistX = (float)Math.Cos(AngleDelta * Math.PI / 180);
                float BeamDistZ = (float)Math.Sin(AngleDelta * Math.PI / 180);

                float BeamDelta = 15;

                for (float BeamIdx = 2 * BeamDelta; BeamIdx < ComplexRadius - BeamDelta - 50; BeamIdx += 2 * BeamDelta)
                {
                    construction.Add(new Cylinder(new Vector3(BeamIdx, DistPerM * BeamIdx, 0), new Vector3(BeamIdx + BeamDelta, -DistPerM * (BeamIdx + BeamDelta), 0), 0.5f));
                    construction.Add(new Cylinder(new Vector3(BeamIdx, DistPerM * BeamIdx, 0), new Vector3(BeamIdx - BeamDelta, -DistPerM * (BeamIdx - BeamDelta), 0), 0.5f));
                }

                BeamDelta = ComplexRadius / 5;
                for (float BeamIdx = BeamDelta; BeamIdx < ComplexRadius; BeamIdx += BeamDelta)
                {
                    float X1 = BeamIdx;
                    float X2 = ComplexRadius - BeamDistX * (ComplexRadius - BeamIdx);
                    float Y1 = DistPerM * BeamIdx;
                    float Y2 = -Y1;
                    float Z1 = 0;
                    float Z2 = BeamDistZ * (ComplexRadius - BeamIdx);

                    float Dist = (float)Math.Sqrt((X2 - X1) * (X2 - X1) + (Z2 - Z1) * (Z2 - Z1));
                    float DX = (X2 - X1) / Dist;
                    float DZ = (Z2 - Z1) / Dist;

                    construction.Add(new Cylinder(new Vector3(X1, Y1, Z1), new Vector3(X2, Y1, Z2), 0.5f));
                    construction.Add(new Cylinder(new Vector3(X1, Y2, Z1), new Vector3(X2, Y2, Z2), 0.5f));

                    float HorBeamFrac = (float)Math.Floor(0.5 + Dist / BeamDelta);

                    if (HorBeamFrac > 0)
                    {
                        float HorBeamDelta = Dist / HorBeamFrac;

                        for (float HorBeamIdx = 0; HorBeamIdx < Dist;)
                        {
                            float HorBeamIdx2 = HorBeamIdx + HorBeamDelta / 2;
                            float HorBeamIdx3 = HorBeamIdx + HorBeamDelta;

                            construction.Add(new Cylinder(new Vector3(X1 + DX * HorBeamIdx, Y1, Z1 + DZ * HorBeamIdx), new Vector3(X1 + DX * HorBeamIdx2, Y2, Z1 + DZ * HorBeamIdx2), 0.5f));
                            construction.Add(new Cylinder(new Vector3(X1 + DX * HorBeamIdx2, Y2, Z1 + DZ * HorBeamIdx2), new Vector3(X1 + DX * HorBeamIdx3, Y1, Z1 + DZ * HorBeamIdx3), 0.5f));

                            HorBeamIdx = HorBeamIdx3;
                        }
                    }
                }

                construction.Translate(-ComplexRadius, (ComplexHeight + DistPerM * ComplexRadius - RoofHeight / 2), 0).Rotate(0, angle, 0);

                construction.Texture = metal;

                scene.Add(construction);
            }

            // -----------
            // Glass roof:
            // -----------

            var roof = new Difference(
                new Cone(new Vector3(0, ComplexHeight + 0.5f + 0.003f, 0), ComplexRadius, new Vector3(0, ComplexHeight + RoofHeight / 2 + 0.5f + 0.003f, 0), 10),
                new Cone(new Vector3(0, ComplexHeight + 0.5f, 0), ComplexRadius, new Vector3(0, ComplexHeight + RoofHeight / 2 + 0.5f, 0), 10)
            );

            roof.Texture = glassExt;
            roof.Interior = glassInt;

            scene.Add(roof);

            // --------------------
            // A single appartment:
            // --------------------

            // Update radius for sections:
            ComplexRadius = ComplexRadius - ComplexSlack;

            // Between each two appartments, we have 30 cm slack
            float SectionSlackSpace = 0.3f;
            float SectionAngleSlack = (float)(360 * SectionSlackSpace / (2 * Math.PI * ComplexRadius));

            SectionAngle = SectionAngle - SectionAngleSlack;
            SectionHeight = SectionHeight - SectionSlackSpace;

            var appartment = new Union();

            // Window:
            appartment.Add(
                new Difference(
                    new Cylinder(new Vector3(0), new Vector3(0, SectionHeight, 0), ComplexRadius + 0.002f),
                    new Union(
                        new Cylinder(new Vector3(0), new Vector3(0, SectionHeight + 1, 0), ComplexRadius).Translate(0, -0.5f, 0),
                        new Plane(new Vector3(-1, 0, 0), 0).Rotate(0, -SectionAngle / 2, 0),
                        new Plane(new Vector3(1, 0, 0), 0).Rotate(0, SectionAngle / 2, 0)))
                {
                    Texture = glassExt,
                    Interior = glassInt
                });

            // Walls:
            var walls = new Union();

            // Right wall
            walls.Add(new Box(new Vector3(ComplexRadius - SectionDepth, 0, 0), new Vector3(ComplexRadius, SectionHeight, -0.1f)).Rotate(0, -(270 + SectionAngle / 2), 0));

            // Right wall in the floor below
            walls.Add(new Box(new Vector3(ComplexRadius - SectionDepth, 0, 0), new Vector3(ComplexRadius, SectionHeight / 2, -0.1f)).Rotate(0, -(270 + SectionAngle / 4), 0));

            // Left wall
            walls.Add(new Box(new Vector3(ComplexRadius - SectionDepth, 0, 0), new Vector3(ComplexRadius, SectionHeight, 0.1f)).Rotate(0, 270 - SectionAngle / 2, 0));

            // Back wall
            walls.Add(new Difference(
                new Cylinder(new Vector3(0), new Vector3(0, SectionHeight, 0), ComplexRadius - SectionDepth + 0.1f),
                new Union(
                    new Cylinder(new Vector3(0), new Vector3(0, SectionHeight + 1, 0), ComplexRadius - SectionDepth).Translate(0, -0.5f, 0),
                    new Plane(new Vector3(-1, 0, 0), 0).Rotate(0, -(SectionAngle / 2), 0),
                    new Plane(new Vector3(1, 0, 0), 0).Rotate(0, (SectionAngle / 2), 0))
            ));

            // Ceiling
            walls.Add(new Difference(
                new Cylinder(new Vector3(0), new Vector3(0, 0.1f, 0), ComplexRadius).Translate(0, SectionHeight - 0.1f, 0),
                new Union(
                    new Cylinder(new Vector3(0), new Vector3(0, 1, 0), ComplexRadius - SectionDepth - 0.1f).Translate(0, SectionHeight - 0.5f, 0),
                    new Plane(new Vector3(-1, 0, 0), 0).Rotate(0, -(SectionAngle / 2), 0),
                    new Plane(new Vector3(1, 0, 0), 0).Rotate(0, (SectionAngle / 2), 0))
            ));

            walls.Texture = plastic;

            // Floors:
            var floors = new Union();

            // Bottom floor
            floors.Add(new Difference(
                new Cylinder(new Vector3(0), new Vector3(0, 0.1f, 0), ComplexRadius),
                new Union(
                    new Cylinder(new Vector3(0), new Vector3(0, 1, 0), ComplexRadius - SectionDepth - 0.1f).Translate(0, -0.5f, 0),
                    new Plane(new Vector3(-1, 0, 0), 0).Rotate(0, -(SectionAngle / 2), 0),
                    new Plane(new Vector3(1, 0, 0), 0).Rotate(0, (SectionAngle / 2), 0))
            ));

            // Middle floor
            floors.Add(new Difference(
                new Cylinder(new Vector3(0), new Vector3(0, 0.1f, 0), ComplexRadius - SectionDepth + SectionDepth2).Translate(0, SectionHeight / 2, 0),
                new Union(
                    new Cylinder(new Vector3(0), new Vector3(0, 0.1f, 0), ComplexRadius - SectionDepth - 0.1f).Translate(0, SectionHeight / 2 - 0.5f, 0),
                    new Plane(new Vector3(-1, 0, 0), 0).Rotate(0, -(SectionAngle / 2), 0),
                    new Plane(new Vector3(1, 0, 0), 0).Rotate(0, (SectionAngle / 2), 0))
            ));

            floors.Texture = wood;

            // ----------------------------------
            // Put appartments on superstructure:
            // ----------------------------------

            for (int secty = 0; secty < SectionsY; ++secty)
            {
                for (float sectangle = 0; sectangle < 360; sectangle += SectionAngle + SectionAngleSlack)
                {
                    // TODO FIXME: Not sure...

                    // object { 
                    // Section rotate y * (SectAngle + SectionAngle / 2) translate y *((SectionHeight + SectionSlackSpace) * SectY)
                    // }
                }
            }
        }
    }
}
