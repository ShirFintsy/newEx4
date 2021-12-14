#include <cstdio>
/*
 * Assignment 2
 * Shir Fintsy 206949075
 * Ohad Marmor 207481524
 */
// returns the variance of X and Y
float var(float* x, int size);

// returns the covariance of X and Y
float cov(float* x, float* y, int size);

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size);

class Line{
public:
    float a,b;
    Line():a(0),b(0){}
    Line(float a, float b):a(a),b(b){}
    float f(float x){
        return a*x+b;
    }
    // for testing:
    void printing() {
        printf("%fx+%f", a, b);
    }
};

class Point{
public:
    float x,y;
    Point(float x, float y):x(x),y(y){}
};

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size);

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size);

// returns the deviation between point p and the line
float dev(Point p,Line l);
// find the average of an array in this size
float find_avr(float* x, int size);




