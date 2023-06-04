/**
 * @file filler.cpp
 * Implementation of functions in the filler namespace.
 *
 */

/**
 * Performs a multi-point flood fill using breadth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillBFS(FillerConfig &config)
{
    return fill<Queue>(config);
}

/**
 * Performs a multi-point flood fill using depth first search.
 *
 * @param config      FillerConfig struct to setup the fill
 * @return animation  object illustrating progression of flood fill algorithm
 */
animation filler::fillDFS(FillerConfig &config)
{
    return fill<Stack>(config);
}

// bool filler::beenVisited(HSLAPixel* pixel, unordered_set<HSLAPixel*> visited) {
//     if (visited.end() != visited.find(pixel)) return true;
    
//     return false;
// }

bool filler::inTolerance(HSLAPixel* pixel1, HSLAPixel* pixel2, float tolerance) {
    if ((*pixel1).dist(*pixel2) <= tolerance) {
        return true;
    }
    return false;
}

bool filler::inBounds(point* p, PNG* img) {
    return (p->x >= 0 && p->y >= 0 && p->x < img->width() && p->y < img->height());
}

/**
 * Run a multi-point flood fill on an image
 *
 * @param  config     FillerConfig struct with data for flood fill of image
 * @return animation  object illustrating progression of flood fill algorithm
 */
template <template <class T> class OrderingStructure> animation filler::fill(FillerConfig &config)
{
    OrderingStructure<point> os;
    
    // change this to a vector of vectors
    int imgWidth = config.img.width();
    int imgHeight = config.img.height();
    vector<vector<bool>> visited = vector<vector<bool>>(imgWidth,vector<bool>(imgHeight, false));
    int counter = 0;

    animation gif;
    vector<point> points = vector<point>(4);
    gif.addFrame(config.img);

    for (int i = 0; i < config.centers.size(); i++) {
        colorPicker* picker = config.pickers[i];
        center center = config.centers[i]; 
        *config.img.getPixel(center.x,center.y) = picker->operator()(point(center));
        visited[center.x][center.y] = true; // .insert(config.img.getPixel(center.x,center.y));
        cout << "center pushed to os" << endl;
        counter++;
        os.add(point(center));
        while (!os.isEmpty()) {
            point top = os.remove();
            point left = point(top.x - 1, top.y, top.c);
            point down = point(top.x, top.y + 1, top.c);
            point right = point(top.x + 1, top.y, top.c);
            point up = point(top.x, top.y - 1, top.c);
            points[0] = left;
            points[1] = down;
            points[2] = right;
            points[3] = up;
            for (int j = 0; j < points.size(); j++) {
                int pointX = points[j].x;
                int pointY = points[j].y;
                if (!inBounds(&points[j], &config.img)) {
                    continue;
                }
                HSLAPixel* ctrPixel = &points[j].c.color;
                HSLAPixel* pPixel = config.img.getPixel(pointX, pointY);
                // !beenVisited(pPixel, visited)
                if (inTolerance(ctrPixel, pPixel, config.tolerance) && visited[pointX][pointY] == false) {
                    // cout << "processing pixel " << pointX << " " << pointY << " pixel #" << counter << endl; 
                    *pPixel = picker->operator()(points[j]);
                    visited[pointX][pointY] = true;
                    counter++;
                    if (counter % config.frameFreq == 0) {
                        cout << "adding frame" << endl;
                        gif.addFrame(config.img);
                    }
                    os.add(points[j]);
                }
            }
            // points.clear();
        } 
    }

    gif.addFrame(config.img);
    cout << "image complete" << endl;
    return gif;
    /**
     * @todo You need to implement this function!
     *
     * This is the basic description of a flood-fill algorithm: Every fill
     * algorithm requires an ordering structure, which is passed to this
     * function via its template parameter. For a breadth-first-search
     * fill, that structure is a Queue, for a depth-first-search, that
     * structure is a Stack. To begin the algorithm, you simply place the
     * given point in the ordering structure, marking it processed
     * (the way you mark it is a design decision you'll make yourself).
     * We have a choice to either change the color, if appropriate, when we
     * add the point to the OS, or when we take it off. In our test cases,
     * we have assumed that you will change the color when a point is added
     * to the structure.
     * Until the structure is empty, you do the following:
     *
     * 1.     Remove a point from the ordering structure, and then...
     *
     *        1.    add its unprocessed neighbors whose color values are
     *              within (or equal to) tolerance distance from the center,
     *              to the ordering structure, and
     *              mark them as processed.
     *        2.    if it is an appropriate frame, send the current PNG to the
     *              animation (as described below).
     *
     * 2.     When implementing your breadth-first-search and
     *        depth-first-search fills, you will need to explore neighboring
     *        pixels in some order.
     *
     *        While the order in which you examine neighbors does not matter
     *        for a proper fill, you must use the same order as we do for
     *        your animations to come out like ours! The order you should put
     *        neighboring pixels **ONTO** the queue or stack is as follows:
     *        ** LEFT(-x), DOWN(+y), RIGHT(+x), UP(-y). IMPORTANT NOTE: *UP*
     *        here means towards the top of the image, so since an image has
     *        smaller y coordinates at the top, this is in the *negative y*
     *        direction. Similarly, *DOWN* means in the *positive y*
     *        direction.** To reiterate, when you are exploring (filling out)
     *        from a given pixel, you must first try to fill the pixel to
     *        it's LEFT, then the one DOWN from it, then to the RIGHT and
     *        finally UP. If you do them in a different order, your fill may
     *        still work correctly, but your animations will be different
     *        from the grading scripts!
     *
     *        Because we are doing a multi-point flood fill you will repeat the flood
     *        fill from several locations stored in the `centers` variable of FillerConfig
     *        struct. Initial fill points should be visited in the order they stored in the
     *        `centers` vector. If a pixel lies in the fill region of multiple start points,
     *        it should only be updated by the first fill. To repeat, pixels should only
     *        be modified the first time they are visited by the filler algorithm.
     *
     * 3.     For every k pixels filled, **starting at the kth pixel**, you
     *        must add a frame to the animation, where k = frameFreq.
     *
     *        For example, if frameFreq is 4, then after the 4th pixel has
     *        been filled you should add a frame to the animation, then again
     *        after the 8th pixel, etc.  You must only add frames for the
     *        number of pixels that have been filled, not the number that
     *        have been checked. So if frameFreq is set to 1, a pixel should
     *        be filled every frame.
     *
     * 4.     Finally, as you leave the function, send one last frame to the
     *        animation. This frame will be the final result of the fill, and
     *        it will be the one we test against.
     *
     */
}
