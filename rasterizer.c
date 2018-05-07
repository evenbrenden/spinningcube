
/* http://www.petesqbsite.com/sections/tutorials/tuts/perspective.html */

#include "rasterizer.h"

struct point3d rotate(struct point3d p, struct angle a)
{
    struct point3d t; // temp

    // x-rot
    t.y = p.y*cos(a.x) - p.z*sin(a.x);
    t.z = p.y*sin(a.x) + p.z*cos(a.x);
    p.y = t.y;
    p.z = t.z;

    // y-rot
    t.x = p.x*cos(a.y) + p.z*sin(a.y);
    t.z = -p.x*sin(a.y) + p.z*cos(a.y);
    p.x = t.x;
    p.z = t.z;

    // z-rot
    t.x = p.x*cos(a.z) - p.y*sin(a.z);
    t.y = p.x*sin(a.z) + p.y*cos(a.z);
    p.x = t.x;
    p.y = t.y;

    return p;
}

struct point2d shift(struct point2d p, double x, double y)
{
    p.x += x;
    p.y += y;

    return p;
}

struct point2d project(struct point3d p, struct point3d screen)
{
    struct point2d b;

    b.x = screen.x + (screen.z * p.x) / (screen.z + p.z);
    b.y = screen.y - (screen.z * p.y) / (screen.z + p.z);

    return b;
}

void render_frame(struct point2d * points, unsigned no_of_points, unsigned size_x, unsigned size_y, uint8_t frame[size_x][size_y])
{
    memset(frame, 0, size_x*size_y);

    for (unsigned i = 0; i < no_of_points; ++i)
    {
        signed x = points[i].x;
        signed y = points[i].y;
        if ((x < size_x && y < size_y) && (x >= 0 && y >= 0))
        {
            frame[x][y] = 1;
        }
    }
}

unsigned make_box(struct point3d * box, unsigned length)
{
    const signed l = length;
    const signed xs = -l/2; // x position
    const signed ys = -l/2; // y position
    const signed zs = -l/2; // z position

    for (signed n = 0; n < l; ++n)
    {
        box[n*12+0] = (struct point3d) { xs+n, ys,   zs };
        box[n*12+1] = (struct point3d) { xs,   ys+n, zs };
        box[n*12+2] = (struct point3d) { xs,   ys,   zs+n };
        box[n*12+3] = (struct point3d) { xs+l, ys+n, zs };
        box[n*12+4] = (struct point3d) { xs+l, ys,   zs+n };
        box[n*12+5] = (struct point3d) { xs+n, ys+l, zs };
        box[n*12+6] = (struct point3d) { xs,   ys+l, zs+n };
        box[n*12+7] = (struct point3d) { xs+n, ys,   zs+l };
        box[n*12+8] = (struct point3d) { xs,   ys+n, zs+l };
        box[n*12+9] = (struct point3d) { xs+n, ys+l, zs+l };
        box[n*12+10] = (struct point3d) { xs+l, ys+n, zs+l };
        box[n*12+11] = (struct point3d) { xs+l, ys+l, zs+n };
    }

    return 12*l;
}

 unsigned make_line(struct point3d * line, unsigned length)
{
    const signed l = length;
    const signed xs = -l/2; // x position

    for (signed n = 0; n < l; ++n)
    {
        line[n] = (struct point3d) { xs+n, 0, 0 };
    }
    return l;
}

