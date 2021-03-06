# CPP-Raytracer
This is a repository of my Raytracer created from scratch in C++. Resources : Scratchapixel , "Ray Tracing in One Weekend", and "Ray Tracing The Next Week"

FEATURES :
- Customizable Camera. Change FOV, Position + Orientation, Depth of Field, Aspect Ratio, Aperture, Motion Blur.
- Simple Shapes like Spheres, Moving Spheres, Axis-Aligned Rectangles, Axis-Aligned Bounding Boxes, Boxes.
- Translation and Rotation for Hittables.
- Constant Density Volumes.

- Simple to use world generator.
- Fully Raytraced scenes.

- Per pixel multisampling using Monte Carlo approximation methods.
- Inheritable Material class for custom materials.
- Bounding Volume Hierarchy Node.

- Image Based Textures.
- Direct PNG Image Renders.
- Perlin Noise Implementation and Texture. (Turbulence included)
- Emissive Material.

- Multi-threaded Render Loop.

SCENES :
- Random Scene with Moving Spheres and Checkered Textures.
- Two Spheres with Perlin Noise Textures.
- Earth Textured Sphere.
- Simple Light Test Scene.
- Cornell Box.
- Final Scene from "Raytracing : The Next Week".

FEATURE ROADMAP :
- More simple shapes. [Cylinder, Cone, Paraboloid]
- Parallelizing the main render loop. [CUDA]
- Denoiser.
- GUI to change camera properties.

Current Output for Raytraced Scene :

![render.png](render.png)
