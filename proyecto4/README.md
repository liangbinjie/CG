# Scene configuration guide

This program is a small ray tracer. A scene is described in a plain text `.cfg`
file: image settings, camera, background, cut planes, objects, and lights. Each
non-comment line is a setting or a scene element. Lines beginning with `#` are
comments; blank lines are ignored.

## Build and render

From the project directory, build with:

```sh
make
```

Render the default `config.cfg`:

```sh
./main
```

Or pass a scene file:

```sh
./main scenes/cylinder_scene.cfg
```

The renderer writes `<output_name>.avs` in the current working directory. It
also tries to open the result in an ImageMagick display window. If no graphical
display is available, the AVS image is still written before the display error.
Run the program from the project directory so texture paths such as
`textures/1.avs` resolve correctly.

## Scene settings

These settings describe the output image, camera, and environment:

```cfg
width=640
height=480
window_xmin=0
window_ymin=0
window_xmax=640
window_ymax=480
output_name=scene
eye_x=320
eye_y=240
eye_z=-1000
background_color_a=1
background_color_r=0.02
background_color_g=0.02
background_color_b=0.04
ambient_intensity=0.4
```

| Setting | Meaning |
| --- | --- |
| `width`, `height` | Image dimensions in pixels. |
| `window_xmin`, `window_ymin`, `window_xmax`, `window_ymax` | Rectangle in the view plane sampled by the image. Its size need not match the pixel dimensions. |
| `output_name` | Output filename without the `.avs` suffix. |
| `eye_x`, `eye_y`, `eye_z` | Camera/eye position in scene coordinates. |
| `background_color_a` | Background alpha, from 0 to 1. |
| `background_color_r/g/b` | Background red, green, and blue channels, each from 0 to 1. |
| `ambient_intensity` | Overall ambient-light contribution. |

Scene positions are three-dimensional `(x, y, z)` values. The example scenes
use an eye at negative `z`, looking toward positive `z`.

## Objects

Each object is declared on one line beginning with `object=`. RGB channels,
alpha, and material coefficients use values from 0 to 1. `kd` is the diffuse
coefficient, `ka` is the ambient coefficient, `ks` is the specular coefficient,
and `kn` is the specular shininess exponent (an integer of at least 1). When
optional material values are omitted, `ks=0`, `kn=32`, and the color weights
are `o1=1`, `o2=0`, `o3=0`.

For the formats below, `alpha` is the object's alpha channel; it is separate
from the transparency weights `o1`, `o2`, and `o3` described later.

### Sphere

```cfg
object=sphere X Y Z alpha R G B radius kd ka [ks kn [o1 o2 [o3]]]
```

`X Y Z` is the center and `radius` must be positive.

```cfg
object=sphere 320 240 100 1.0 0.2 0.5 0.9 60 0.8 0.3 0.25 32 1.0 0.0 0.0
```

### Polygon

Polygons must have at least three coplanar vertices. The vertex count `N` is
followed by color, material values, and then `N` coordinate triples. Optional
values, when present, come after the vertices:

```cfg
object=polygon N alpha R G B kd ka X1 Y1 Z1 ... XN YN ZN [ks kn [o1 o2 [o3]] [texture_id]]
```

The valid optional endings are:

```text
(none)
texture_id
ks kn
ks kn texture_id
ks kn o1 o2
ks kn o1 o2 texture_id
ks kn o1 o2 o3 texture_id
```

Texture ID `0` means use the polygon's solid RGBA color. A positive ID selects
an AVS texture; see [Textures](#textures).

```cfg
object=polygon 4 1.0 1.0 1.0 1.0 0.8 0.3 100 100 200 300 100 200 300 300 200 100 300 200 0.2 32 1.0 0.0 0
```

### Disk

```cfg
object=disk X Y Z alpha R G B outer_radius inner_radius Nx Ny Nz kd ka [ks kn [o1 o2 [o3]]]
```

`X Y Z` is the center; `(Nx, Ny, Nz)` is the plane normal. The normal is
normalized by the program. `outer_radius` must be positive, and
`0 <= inner_radius < outer_radius`. Set `inner_radius=0` for a solid disk; a
positive inner radius makes a ring.

### Ellipse

```cfg
object=ellipse F1x F1y F1z F2x F2y F2z alpha R G B major_axis_sum Nx Ny Nz kd ka [ks kn [o1 o2 [o3]]]
```

`F1` and `F2` are the foci. The normal defines the ellipse's plane, and
`major_axis_sum` is the sum of distances from a point on the ellipse to the two
foci. It must be greater than the distance between the foci. The normal is
normalized by the program.

### Cylinder

```cfg
object=cylinder X0 Y0 Z0 Qx Qy Qz radius B F alpha R G B kd ka [ks kn [o1 o2 [o3]]] [texture_id] [cut_planes=N ID1 ... IDN]
```

`(X0,Y0,Z0)` is the anchor, `(Qx,Qy,Qz)` is the axis direction (normalized by
the program), `radius` is the lateral radius, and `B` and `F` limit axial
distance from the anchor along the axis. The implemented cylinder is its curved
side surface; it does not add end caps.

### Cone

```cfg
object=cone X0 Y0 Z0 Qx Qy Qz M N B F alpha R G B kd ka [ks kn [o1 o2 [o3]]] [cut_planes=COUNT ID...]
```

`(X0,Y0,Z0)` is the tip and `Q` is the axis direction from the tip. `M` is a
positive reference distance and `N` is the radius at that distance, so the
radius at axial distance `d` is `d*N/M`. `B` and `F` set the axial interval.
The cone is a lateral surface without end caps.

### Quadric surface

```cfg
object=quadric X Y Z alpha R G B kd ka ks kn o1 o2 [o3] A B C D E F G H I J [cut_planes=COUNT ID...]
```

The ten coefficients define the implicit surface, using coordinates relative
to `(X,Y,Z)`:

```text
A*x^2 + B*y^2 + C*z^2 + D*x*y + E*x*z + F*y*z
  + G*x + H*y + I*z + J = 0
```

At least one of the six quadratic coefficients `A` through `F` must be
nonzero. The quadric format requires `ks`, `kn`, `o1`, and `o2`; `o3` is
optional.

## Transparency and reflection weights

`o1`, `o2`, and `o3` are the weights for the object's own color, reflected
color, and transmitted color. Each must be in `[0,1]` and they must sum to 1.
For example, `o1=0.35 o2=0 o3=0.65` makes the traced result mostly come from the
object behind it. With `o3=0`, there is no transmission. Transmission follows
the ray straight through the object; it does not model refraction.

## Cut planes

Declare planes with unique IDs:

```cfg
cut_plane=ID A B C D
```

The plane is `A*x + B*y + C*z + D = 0`. The program normalizes the four
coefficients using the length of `(A,B,C)`. For an object with a cut-plane
reference, points satisfying `A*x + B*y + C*z + D <= 0` are kept and points on
the positive side are removed. A zero normal is invalid.

Attach one or more declared planes to an object by adding this suffix to its
line:

```cfg
cut_planes=COUNT ID1 ID2 ...
```

For example, this removes the half with `z < 150` from a cylinder and keeps the
half with `z >= 150`:

```cfg
cut_plane=2 0 0 -1 150
object=cylinder 320 100 150 0 1 0 85 0 260 1.0 0.9 0.3 0.1 0.8 0.4 cut_planes=1 2
```

The object references the plane by ID, not by repeating its coefficients.

Polygons receive cast shadows by default. To keep a surface lit without
receiving shadows from other objects, append `receives_shadows=0` to its
object line. This does not change the shadows that the object casts onto other
surfaces.

## Lights

Add one line per point light:

```cfg
light=X Y Z intensity c1 c2 c3
```

`X Y Z` is the light position and `intensity` is its intensity. The distance
attenuation denominator is `c1 + c2*d + c3*d^2`, where `d` is the distance
from the light to the hit point. The three attenuation values must not all be
zero. To make a light brighter, raise its intensity or reduce attenuation; the
ambient setting also affects the whole scene.

```cfg
light=150 100 -250 1.0 1.0 0.0003 0.000001
```

## Textures

Polygon, sphere, and cylinder texture files use the AVS format and are loaded
when the scene starts.
For texture ID `2`, place the file at `textures/2.avs`; in general the path is
`textures/<ID>.avs`. The AVS file contains a big-endian 32-bit width, a
big-endian 32-bit height, then four bytes per pixel in alpha, red, green, blue
order. Texture coordinates are mapped over the polygon's bounding rectangle.

Specify the texture ID after the polygon vertices and optional material/color
weights. For example:

```cfg
object=polygon 4 1 1 1 1 0.8 0.3 100 100 200 300 100 200 300 300 200 100 300 200 0.2 32 1.0 0.0 0.0 2
```

The example scenes in `scenes/` show complete configurations, including
[`config.cfg`](scenes/config.cfg),
[`cylinder_scene.cfg`](scenes/cylinder_scene.cfg),
[`cone_scene.cfg`](scenes/cone_scene.cfg), and
[`transparency_scene.cfg`](scenes/transparency_scene.cfg).
