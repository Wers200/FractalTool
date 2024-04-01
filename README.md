# FractalTool
A simple 2D fractal renderer, implemented via Direct3D 11 and HLSL (Shader Model 6.0).

The default fractal rendered is the Mandelbrot set.

## Rendering parameters
You can change some variables used in the fractal rendering process inside the program.
1. A complex constant `C`, changed via the left mouse button.
2. The zoom, varied via a dynamic white-tinted window with the right mouse button.
3. The maximum iteration count, changed in steps of 25 and 250 via the arrow keys (L/R and U/D).
4. The rendering type, changed by pressing Enter from "Mandelbrot" to "Julia" and back.

## Custom shaders
In order to replace the default shaders, put the new `compute.hlsl` file as the new compute shader and the `pixel.hlsl` file as the new pixel shader in the same directory with the program instead of the ones already present. 

In structure, the shaders are recommended to match the usual default shaders, only with the `main` function affected in your pixel shader and the `getIter` function in your compute shader, perhaps with some additional helper functions appended. Note that using `double` instead of `float` to increase precision slows down the program immensely.

Examples of custom shaders are included in the `/Custom` folder and are also contained in the release.

### Hot reload
The program also supports editing your shaders on the way, so if you save your `.hlsl` file while the program is running, it will compile the new shaders and update them accordingly. Note that deleting and creating or copying a new shader from elsewhere doesn't trigger reloading, so you have to save the new file with no changes for it to work.

### Example custom shaders
The present custom compute shaders are advised to be used in the following pairings with the provided pixel shaders:

1. `Burning ship.hlsl` with `Reddish.hlsl` or `Raddish.hlsl`, the latter being better
2. `Feather fractal.hlsl` with `Reddish.hlsl` or `Raddish.hlsl`, the former being better
3. `Hénon map.hlsl` with anything except `Nova fractal.hlsl`
4. `Mandelbrot set.hlsl` with anything except `Nova fractal.hlsl`
5. `/Compute shaders/Nova fractal.hlsl` only with `/Pixel shaders/Nova fractal.hlsl`
6. `Timelybrot set.hlsl` with anything except `Nova fractal.hlsl`

Note that `Nova fractal.hlsl` does not support the "Mandelbrot" rendering type and is not affected by clicking Enter and that all other fractals do indeed work with the feature.
