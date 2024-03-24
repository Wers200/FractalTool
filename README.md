# FractalTool
A simple 2D fractal renderer, implemented via Direct3D 11 and HLSL (Shader Model 6.0).

The default fractal rendered is the Julia set of `z^2 + C` with fixed `C` changeable.

## Rendering parameters
You can change some variables used in the fractal rendering process inside the program.
1. A complex constant `C`, changed via the left mouse button.
2. The zoom, varied via the right mouse button.
3. The maximum iteration count, changed in steps of 25 and 250 via arrow keys (L/R and U/D).

Note that sometimes the program ignores the changes in a sporadic fashion for the reasons unknown. In this case, poking around or restarting the program might help. In the dire cases, or to set specific constant values, you might need to change the shader code directly.

## Custom shaders
In order to replace the default shaders, put new `compute.hlsl` file as the new compute shader and `pixel.hlsl` file as the new pixel shader in the same directory with the program instead of the ones already present. 

In structure the shaders must match the usual default shaders, only with the `main` function affected in your pixel shader and the `getIter` function in your compute shader, only perhaps with some additional helper functions appended. Note that using `double` instead of `float` to increase precision slows down the program immensely.

Examples of custom shaders are included in `/FractalTool/Custom` and are also contained in the release.
