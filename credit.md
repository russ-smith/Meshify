The included ofxGuiExtended is an old version of an openFrameworks addon created by Deborah Schmidt as an extension 
to the builtin ofxGui that comes with openFrameworks, very slightly modified by myself.
The current version of ofxGuiextended can be found at github.com/frauzufall/ofxGuiExtended
It's rewritten from the ground up and much better.
I didn't use it because it requires ofJSON, which is not in the stable release of openFrameworks yet.

The triangle table for Marching Cubes is based on Paul Bourke's original table at http://paulbourke.net/geometry/polygonise/
modified into a shader-friendly bitfield format.

The vertex-to-edge table for Dual Marching Cubes is based on the table from Lin20's Isosurface project at
https://github.com/Lin20/isosurface , similarly modified.