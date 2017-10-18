CUDA Rasterizer
===============

[CLICK ME FOR INSTRUCTION OF THIS PROJECT](./INSTRUCTION.md)

**University of Pennsylvania, CIS 565: GPU Programming and Architecture, Project 3**

* LINSHEN XIAO
* Tested on: Windows 10, Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz, 16.0GB, NVIDIA GeForce GTX 970M (Personal computer)

## Overview

In this project, I implemented a CUDA-based path tracer capable of rendering globally-illuminated images very quickly. Path tracing is a computer graphics Monte Carlo method of rendering images of three-dimensional scenes such that the global illumination is faithful to reality (reference: https://en.wikipedia.org/wiki/Path_tracing). Basially, for every pixel of the image, we shoot a ray from the camera, find an intersection, and return a color based on the material, position of the objects & lightsource and so on after a few depths. As this process for every pixel is relatively similar, we can use GPU to run the programs simultaneously to make the path tracer faster.

## Features

* Basic Lambert and Blinn-Phong
* Line & Point rasterization
* Backface culling
* UV texture mapping with bilinear texture filtering and perspective correct texture coordinates

## Results

### Rotating Duck
![](renders/duck2.gif)

### Basic Lambert and Blinn shading
|Duck with texture and point light|+Blinn-Phong|+Ambient light|
|------|------|------|
|![](renders/2017-10-17_191933.png) | ![](renders/2017-10-17_191958.png) | ![](renders/duck.png) |

### Basic Normal and depth shading
|Duck with texture as color|Duck with normal as color|Duck with depth as color|
|------|------|------|
|![](renders/2017-10-17_191933.png) | ![](renders/2017-10-17_192434.png) | ![](renders/2017-10-17_192601.png) |

### Line and point rasterization
|Duck rasterized with triangles|Duck rasterized with lines|Duck rasterized with dots|
|------|------|------|
|![](renders/2017-10-17_191933.png) | ![](renders/2017-10-17_192335.png) | ![](renders/2017-10-17_192404.png) |

### Backface culling
|Duck rasterized with front faces|Duck rasterized with back faces|
|------|------|
|![](renders/2017-10-17_191933.png) | ![](renders/2017-10-17_195707.png) |

###UV texture mapping with bilinear texture filtering and perspective correct texture coordinates
|Original Checkerboard|+Perspective correct texture coordinates|+Bilinear texture filtering|
|------|------|------|
|![](renders/no_pc.png)|![](renders/pc.png)|![](renders/pc_&_bl.png)|

## Performance Analysis

![](renders/Nobackface&backface.png)

| Pipeline                   | No Backface Culling | Backface Culling |
|----------------------------|---------------------|------------------|
| Rasterizer                 | 1716.314            | 1375.803         |
| Fragment Shader            | 681.287             | 688.537          |
| Framebuffer                | 103.308             | 103.252          |
| Primitive Assembly         | 28.59               | 28.632           |
| Vertex Assembly and Shader | 11.014              | 11.004           |

Here we take the duck scene as an example. We can clearly see that rasterizer use most of the time, as there are lots of computation in this process, to check if the pixel of the bounding box lies inside the triangle, depth test and compute the interpolated texture coordinates and normal and so on. The render function in fragment shader also has some amount of computation for the computation for the color decided by normal, light direction and texture color.

We can see that backface culling did save time in rasterizer part. As the backface triangles are ingnored in the process, the performance did improved a bit.

### Credits

* [tinygltfloader](https://github.com/syoyo/tinygltfloader) by [@soyoyo](https://github.com/syoyo)
* [glTF Sample Models](https://github.com/KhronosGroup/glTF/blob/master/sampleModels/README.md)
