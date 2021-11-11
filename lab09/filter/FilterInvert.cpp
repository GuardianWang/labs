#include "FilterInvert.h"

FilterInvert::~FilterInvert()
{
}

void FilterInvert::apply(Canvas2D *canvas) {
    int width = canvas->width();

    RGBA* current_pixel = nullptr;
    RGBA* current_row = canvas->data();

    // TODO: Task 8 [Optional] Use bitwise complement to set inversion

    for (int r = 0; r < canvas->height(); r++) {
        current_pixel = current_row;

        for (int c = 0; c < canvas->width(); c++) {
            // TODO: Task 7 Update the color with its inversion
            current_pixel->r = 255 - current_pixel->r;
            current_pixel->g = 255 - current_pixel->g;
            current_pixel->b = 255 - current_pixel->b;


            // CONTINUED: Task 8 [Optional]

            current_pixel++;
        }
        current_row += width;
    }
}
