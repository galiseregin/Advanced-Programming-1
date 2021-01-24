/*
 * minCircle.h
 *
 * Author: 322060187 Gali Seregin
 */
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include "anomaly_detection_util.h"

using namespace std;

//class Point;

// ------------ DO NOT CHANGE -----------

class Circle {
public:
	Point center;
	float radius;
	Circle(Point c,float r) : center(c), radius(r) {}
};
// --------------------------------------

float dist(Point a, Point b);
bool is_inside(const Circle& c, const Point& p);
Circle from2points(Point a,Point b);
Circle from3Points(Point a, Point b, Point c);
Circle trivial(vector<Point>& P);

Circle welzl(Point** P,vector<Point> R, size_t n);

Circle findMinCircle(Point** points,size_t size);


#endif// MINCIRCLE_H_
