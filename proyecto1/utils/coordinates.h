#ifndef COORDINATES_H
#define COORDINATES_H

/*
 * Coordinate, polygon, and province structures.
 *
 * A province can contain one or more polygons. This is necessary for
 * provinces such as Puntarenas, whose GeoJSON geometry contains multiple
 * disconnected polygon rings (for example, islands and detached areas).
 */

typedef struct {
    double x;
    double y;
} COORDINATES;

typedef struct {
    const COORDINATES *coordinates;
    int size; // number of coordinate points in this polygon
} POLYGON;

typedef struct {
    const char *name;
    const POLYGON *polygons;
    int polygon_count; // number of polygons in this province
} PROVINCE;

/* Polygon lists for each province */
extern const POLYGON Alajuela[];
extern const POLYGON Cartago[];
extern const POLYGON Heredia[];
extern const POLYGON Guanacaste[];
extern const POLYGON Limon[];
extern const POLYGON Puntarenas[];
extern const POLYGON SanJose[];

/* Province list */
extern const PROVINCE provinces[];
extern const int province_count;

#endif
