
#include <stdio.h>
#include "rasterizer.h"

// frame metrics
#define DURATION 150
#define SIZE_X 640
#define SIZE_Y 480
#define ROTATION_PER_FRAME (2*3.1416f/DURATION)
#define INTENSITY_INC_PER_FRAME 256.0f/(DURATION/2)

// viewer relative to screen
#define SCREEN_X 0
#define SCREEN_Y 0
#define SCREEN_Z 640000 // ->Inf makes box look squarer along z

// rotation angles
#define ANGLE_X f*ROTATION_PER_FRAME
#define ANGLE_Y f*ROTATION_PER_FRAME
#define ANGLE_Z f*ROTATION_PER_FRAME

// box metrics
#define OBJECT_LENGTH 50

// screen center
#define POS_X SIZE_X/2
#define POS_Y SIZE_Y/2

// for memory alloc
#define MAX_NO_OF_POINTS_IN_OBJECT 12*OBJECT_LENGTH

static void write_pixel_to_file(FILE * f, uint8_t r, uint8_t g, uint8_t b)
{
    fwrite(&r, sizeof(uint8_t), 1, f);
    fwrite(&g, sizeof(uint8_t), 1, f);
    fwrite(&b, sizeof(uint8_t), 1, f);
}

int main()
{
    FILE * file = fopen("video.rgb", "wb");
    if (file == NULL)
    {
        printf("could not open file for writing.\n");
        return -1;
    }

    /* current frame buffer */
    uint8_t frame[SIZE_X][SIZE_Y];

    /* create points for 3d box */
    struct point3d object3d[MAX_NO_OF_POINTS_IN_OBJECT];
    struct point2d object2d[MAX_NO_OF_POINTS_IN_OBJECT];
    unsigned no_of_points = make_box(object3d, OBJECT_LENGTH);

    /* viewer position relative to screen */
    struct point3d screen = { SCREEN_X, SCREEN_Y, SCREEN_Z };

    /* vars for time-variant color scheme */
    double intensity_step = 256.0f/(DURATION/2);
    double intensity = 0.0f;

    /* rasterize, render and write to file */
    for (unsigned f = 0; f < DURATION; ++f)
    {
        struct angle a = { ANGLE_X, ANGLE_Y, ANGLE_Z }; // angle for rotation transform

        /* rasterize */
        for (unsigned i = 0; i < no_of_points; ++i)
        {
            struct point3d r = rotate(object3d[i], a);
            struct point2d u = project(r, screen);
            object2d[i] = shift(u, POS_X, POS_Y);
        }

        /* render frame as boolean array */
        render_frame(object2d, no_of_points, SIZE_X, SIZE_Y, frame);

        /* write 4:4:4 RGB interleaved video to file */
        for (unsigned y = 0; y < SIZE_Y; ++y)
        {
            for (unsigned x = 0; x < SIZE_X; ++x)
            {
                unsigned v = frame[x][y];

                /* some time-variant color scheme */
                uint8_t r = v ? ((uint8_t) intensity) : 0;
                uint8_t g = -r/2;
                uint8_t b = r/2;

                write_pixel_to_file(file, r, g, b);
            }
        }

        /* alter intensity */
        intensity += intensity_step;
        if (intensity >= 255.0f || intensity <= 0.0f)
            intensity_step = -intensity_step;
    }

    fclose(file);

    return 0;
}

