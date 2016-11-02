[POV-Ray](http://www.povray.org/) - The Persistence of Vision Raytracer
=======================================================================

What this branch is all about...
--------------------------------------

I love Pov-Ray, I absolutely do. But... I don't particularly love the .pov language. That is, it's an interpreted (and therefore inherently slow)
language, it doesn't support things like for loops, classes and so on (only the preprocessor declare's and while's)... the list just continues and 
continues. Sure, pov has great value for simple scenes, but as soon as you make things complex, your includes and pov code will become a hell.

Evidenced by the fact that I've already had dozens of tools in my hands that generate trees, do collision detection, physics stuff, ... I 
assume that I'm not the only one who has this problem.

I think this is a shame for an otherwise fantastic piece of machinery that I've used for so many years. 

This feature introduces a solution to this problem. It talks directly to the POV Core library and does all the plumbing required to generate 
a scene. The only thing you have to do is add objects in C#. 

Like this:

    var settings = new Settings(renderResult.Width, renderResult.Height);
    var scene = new Scene(settings);
    scene.Add(new Camera(new Vector3(0, 20, -100), new Vector3(0, 0, 0)));
    scene.Add(new LightSource(new Vector3(0, 100, 0), new RGB(1, 1, 1)));
    scene.Add(new Sphere(new Vector3(0, 0, 0), 10)
    {
        Surface = new Texture() { Pigment = new Pigment(new RGBFT(1, 0, 0)) }
    });

	using (RenderCallback target = new RenderCallback())
    {
        scene.Render(target);

        // Do something with your rendered image which is now in 'target.Image'
    }

You'll end up with something like this:

[[https://github.com/atlaste/povray/Example.png|alt=Example]]

Since ray-tracing can take quite some time, async ray-tracing (with a status bar) is also included in the example code.

Getting started
--------------------------------------

You need Visual Studio 2015, Community Edition will do just fine. Open the solution in 
Windows/vs10/povray.sln; select "Debug / x64" as configution and build.

The project "povclrtest" contains the example code you need to get started.

This project is a WIP
--------------------------------------

This document is still a work in progress. The POV-Ray project itself
has existed for more than 20 years and is loaded with tons of features. Having 
support for all of the stuff in POV-Ray takes time... so please bear with us, it might
take some time.

It's worth noting that the library only uses the POV-Ray core and base libraries. 
We didn't want a dependency on the parser, message queue and all the other boilerplate 
code, so when this was developed, the boilerplate was ignored. Unfortunately a *lot* of 
logic is still in that code, even though it doesn't belong there. Migrating everything 
to the proper place, while carefully crafting around legacy features, is what costs the 
majority of the effort. 

This also means that multi-threading and AA are currently not supported; these are solved
in the frontend/backend/parser code of the 'normal' POV-Ray project. However, because of the 
way this is designed, it's fairly trivial to add at a later stage; the main idea here is to 
simply use Parallel.ForEach to do the majority of the work.

Last edit: 2016-11-02

License
--------------------------------------

The source for POV-Ray v3.7 is licensed under the AGPL3. The documentation is under the
Creative Commons Attribution-Noncommercial-ShareAlike 2.5 license, and support files such
as SDL includes, macros, sample scenes and so forth are under the Creative Commons Attribution-ShareAlike
3.0 Unported License (see each file header for the specific one).

The povclr project is licensed under the same license, AGPL3. By doing this, we hope that the 
feature will some day be included in POV-Ray.

Building this extension
--------------------------------------


API Documentation
--------------------------------------

The API for this extension will closely follow the POV-Ray documentation. The only 
namespace you ever need is the 'povclr' namespace.

When built and installed via the means provided in the source tree, all versions
of POV-Ray come with documentation. For the Unix build, this is in the form of a
manpage giving basic usage, and full HTML-based documentation. For the Windows
version, there is a HtmlHelp (.CHM) file provided.

The official location for the online documentation is http://www.povray.org/documentation/,
however please be aware that at the time of writing this still has the version 3.6 docs.
This will be updated shortly.
