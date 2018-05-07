
#ifndef RASTERIZER_H
#define RASTERIZER_H

#include <stdint.h>
#include <string.h>
#include <math.h>

struct point3d {
    double x;
    double y;
    double z;
};

struct angle {
    double x;
    double y;
    double z;
};

struct point2d {
    double x;
    double y;
};

struct point3d rotate(struct point3d p, struct angle a);
struct point2d shift(struct point2d p, double x, double y);
struct point2d project(struct point3d p, struct point3d screen);
void render_frame(struct point2d * points, unsigned no_of_points, unsigned size_x, unsigned size_y, uint8_t frame[size_x][size_y]);
unsigned make_box(struct point3d * box, unsigned length);
unsigned make_line(struct point3d * line, unsigned length);

#endif
