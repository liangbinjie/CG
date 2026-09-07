# Line Plotting in Frame Buffer

This project purpose is to test different line plotting algorithms learned throughout the course of Computer Graphics.

The algorithms are:

- Brute force approach
- Incremental approach
- Bressenham approach

## Requirements

- C
- NASM
- Mesa Graphics Library (OpenGL)
- build-essential


## DEMO

To give it a try, once extracted the .tgz file

```sh
tar -xvzf CG_project0.tgz
```

run the makefile
```sh
make
```

If there are no error, then proceed to run the program

```sh
./main <resolution> <xlines> <xtimes>
```

## Work Description

### Brute Force
Brute force approach was basically getting the slope of the two points and calculate the `x` or `y` value based on the line equation `y=mx+b`

### Incremental
Incremental algorithms for plotting a line were based on the brute force approach, from the material learned in class, we searched for a way to not multiply inside the cycles, by calculating the differences.

### Bressenham
Bressenham Algorithm was developed based on [Wikipedia](https://es.wikipedia.org/wiki/Algoritmo_de_Bresenham) and [YouTube](https://www.youtube.com/watch?v=CceepU1vIKo) explanation, alongside with the materia learned in class.

Basically, the first thing we identify is if `dx` value dominates over `dy` or viceversa, this gives us the option to decide whether to take up horizontal octaves or vertical ones.

Then we make sure that the origin always start at the left side if using the horizontal or the bottom if it is using the vertical approach.

At the end we calculate the direction of `x` or `y` is taking, so we can know how the deltas will change, whether they will be positive or negative. 