# Usage instructions

## Controlling function parameters

The function to be sampled is controlled using the top part of the left GUI.
There is one integer parameter, Iterations, and 8 float parameters, A-F.
These may have no effect, it depends on the distance function currently used.
Try them and see.

The region of function space to be sampled is controlled using the Centre and Extent parameters.
Extent is like zoom in reverse - a smaller Extent will sample a smaller region of function space,
so the resulting object will appear larger.

## Changing mesh and render mode.

Below the function parameters is a four way toggle to select the grid resolution to sample. 
This is the resolution along each axis, so each doubling of resolution samples eight times 
as many cubes.

Below this is a toggle for wireframe mode which allows you to see the mesh structure.
You may need to zoom in to see this at high resolutions.

Below this is a two way toggle allowing you to switch extraction mode between
Marching Cubes and Dual Marching Cubes..

## Loading a new function

At the bottom of the left GUI is a button to load new distance functions.
This will open a browser window to select a GLSL file.
Some examples are provided in data/DistanceFunctions.

## Controlling lights

The right GUI contains the light parameters for two lights. 
Each light has an RGB color and a direction. 
The direction is given in spherical coordinates - the first parameter controls longitude,
and the second controls latitude.

## Controlling view

Left-drag to rotate the object.

Scroll or right-drag to zoom.

Middle-drag to pan.

Left-and-right click to reset.

## Hiding the GUIs

Press 1 or 2 to toggle the left and right GUIs respectively

## Writing distance functions

You can write your own distance functions in GLSL to use in Meshify.

You can declare up to 8 uniform floats named A - H (uppercase), and one uniform int named Iterations.

The floats will have a range of -1 to 1, and the int will have range 1-100, it's up to you to
convert these into useful values in your shader.

Then create a function with the following signature:

```glsl
float DE(vec3 p)
```
Negative distance values represent the object's interior, positive values the exterior.
The mesh will be generated where the function crosses zero.

Your function will be pasted into Meshify's built-in shaders, so you don't need to write any other code,
only the distance function itself.
