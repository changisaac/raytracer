# raytracer

This is an offline C++ implemented 3D ray tracer. It is still a work in progress but so far includes the following:

- global illumination (recursive ray-trace)
- antialiasing
- diffuse lambertian material
- specular metal material and imperfect metal (fuzzy metal)
- dielectric non-metal material
- camera properties
    - positionable
    - depth of field
- shapes: spheres

Planned future features:

- acceleration structure (bounding volume hierarchies)
- textures and texture mapping
- emmisive material (lights)
- constant density mediums (fog, smoke, etc.)
- shapes: rectangles

![Alt text](images/metal_scene.png?raw=true "Metal Materials Scene")