#ifndef COORDINATES_H
#define COORDINATES_H

/*
* This header file defines what it would be the structure of a coordinate and a province
* a province have a name, a list of coordinates (defined in map.c)
* and the size, which this means is the total amount of coordinates points it got that province
*/

typedef struct {
  double x;
  double y;
} COORDINATES;

typedef struct {
  const char *name;
  const COORDINATES *coordinates;
  int size; // how many coordinates "points" it have
} PROVINCE;

/* Individual provinces - List of coordinates of province */

extern const COORDINATES Alajuela[];
extern const COORDINATES SanJose[];
extern const COORDINATES Heredia[];
extern const COORDINATES Guanacaste[];
extern const COORDINATES Limon[];
extern const COORDINATES Puntarenas[];

/* Province List */

extern const PROVINCE provinces[];
extern const int province_count;


#endif