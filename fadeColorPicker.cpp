#include "fadeColorPicker.h"

fadeColorPicker::fadeColorPicker(double fade)
{
    fadeFactor = fade;
}

HSLAPixel fadeColorPicker::operator()(point p)
{
    int ctrX = p.c.x;
    int ctrY = p.c.y;
    double ctrH = p.c.color.h;
    double ctrS = p.c.color.s;
    double ctrL = p.c.color.l;
    double dist = calcDist(ctrX, ctrY, p.x, p.y);
    double newL = ctrL*pow(fadeFactor, dist);
    return HSLAPixel(ctrH, ctrS, newL);
}

double fadeColorPicker::calcDist(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2-x1,2) + pow(y2-y1,2));
}

