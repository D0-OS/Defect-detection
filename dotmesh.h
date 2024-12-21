#pragma once
#ifndef DOTMESH_H
#define DOTMESH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

extern double Pi;

struct Dot {
    double R;
    double ang;
};

struct Point {
    double x, y;
};

extern double grad_to_rad(double x);

extern double rad_to_grad(double x);

struct Line {
    double A, B, C;
};

class DotMesh
{
    Dot center {0, 0};
    std::vector<Dot> data;
    std::vector<Dot> delta;
    std::vector<double> delta_val;
    std::pair<Dot, Dot> findClosest(double ang, DotMesh& v);
    Dot getIntersec(Dot a, Dot b, Dot c);
public:
    std::vector<Dot> getData();
    int getDataSize() { return data.size(); };
    void setData(std::vector<Dot> dots);
    void calcDiff(DotMesh& b);
    std::vector<Dot> outData();
    std::vector<double> get_delt();
};

extern Point polarToDec (Dot a);

extern Dot decToPolar(Point a);

extern double distToCircle(Point mid, Point val, double Rad);


#endif // DOTMESH_H
