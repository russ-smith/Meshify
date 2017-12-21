# Build instructions

This project requires openFrameworks 0.9.X to build, i used 0.9.8

To build under Windows, use Visual Studio 2015 - this is an openFrameworks limitation.

The repo should be cloned to (openFrameworks_root_dir)/apps/myApps/Meshify

Then move the ofxGuiExtended folder to (openFrameworks_root_dir)/addons

Then open the solution and build.

It should also build on Linux using any of the supported IDEs but I haven't tested it.

On Linux, copy ofxGuiExtended to the addons folder first.

Use the openFrameworks project generator to create a new project file and include ofxGuiExtended.

Then add the source files to your project and copy the contents of bin/data to your own bin/data folder.

It won't build on Mac as it requires OpenGL 4.5 
