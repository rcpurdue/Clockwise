#ifndef CLOCKWISE_H
#define CLOCKWISE_H

#include <Watchy.h>

class Clockwise : public Watchy{
    using Watchy::Watchy;
    public:
        void drawWatchFace();
        void drawQuadrant(int up_left_x, int up_left_y, const String &center_text, int num_fill, int total,
                          bool lead_zero);
};

#endif
