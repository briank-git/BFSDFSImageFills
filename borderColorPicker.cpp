#include "borderColorPicker.h"

borderColorPicker::borderColorPicker(unsigned int borderSize, HSLAPixel fillColor, PNG &img, double tolerance)
{
    borderColorPicker::borderSize = borderSize;
    borderColorPicker::fillColor = fillColor;
    borderColorPicker::img = img;
    borderColorPicker::tolerance = tolerance;
}

HSLAPixel borderColorPicker::operator()(point p)
{
    if (isOnImageBorder(p) || isOnEdge(p)) {
        return fillColor;
    } else {
        return *img.getPixel(p.x, p.y);
    }
}

bool borderColorPicker::isOnImageBorder(point p) {
    int xMax = img.width() - 1;
    int yMax = img.height() - 1;
    return (p.x < borderSize || p.y < borderSize || p.x > xMax - borderSize || p.y > yMax - borderSize);
}

double borderColorPicker::calcDist(int x1, int y1, int x2, int y2) {
    return pow(x2-x1,2) + pow(y2-y1,2);
}

bool borderColorPicker::isOnEdge(point p) {
    HSLAPixel ctr = p.c.color;
    for (int i = p.x - borderSize; i <= p.x + borderSize; i++) {
        for (int j = p.y - borderSize; j <= p.y + borderSize; j++) {
            if (ctr.dist(*img.getPixel(i, j)) > tolerance && calcDist(p.x, p.y, i, j) <= pow(borderSize,2)) {
                return true;
            }
        }
    }
    return false;
   
}