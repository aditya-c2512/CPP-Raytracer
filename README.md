# CPP-Raytracer
This is a repository of my Raytracer created from scratch in C++. Resources : Scratchapixel , "Ray Tracing in One Weekend", and "Ray Tracing The Next Week"

FEATURES :
- Customizable Camera. Change FOV, Position + Orientation, Depth of Field, Aspect Ratio, Aperture, Motion Blur.
- Simple Shapes like Spheres, Moving Spheres, Planes, Axis-Aligned Bounding Boxes.
- Fully Raytraced scenes.
- Per pixel multisampling using Monte Carlo approximation methods.
- Inheritable Material class for custom materials.
- Simple to use world generator.
- Bounding Volume Hierarchy Node.
- Image Based Textures.
- Direct PNG Image Renders.
- Perlin Noise Implementation and Texture. (Turbulence included)

SCENES :
- Random Scene with Moving Spheres and Checkered Textures.
- Two Spheres with Perlin Noise Textures.
- Earth Textured Sphere.

FEATURE ROADMAP :
- More simple shapes. [Cylinder, Cone, Cube, Paraboloid]
- Bounding Volume Hierarchy[BVH]. => DONE.
- Textures. => DONE
- Lights and Shadows.
- Parallelizing the main render loop. [CUDA]
- Direct PNG Image formation. => DONE.
- Denoiser.
- GUI to change camera properties.

Current Output for Raytraced Scene :

![render.png](render.png)
