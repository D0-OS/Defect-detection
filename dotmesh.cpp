#include "dotmesh.h"

double Pi = acos(-1);

double grad_to_rad(double x){
    return x * Pi / 180;
}
double rad_to_grad(double x){
    return x * 180 / Pi;
}

Point polarToDec (Dot a){
    Point point;
    point.x = a.R * cos((a.ang));
    point.y = a.R * sin((a.ang));
    return point;
}

Dot decToPolar (Point point){
    double x = point.x;
    double y = point.y;
    double r = std::sqrt(x * x + y * y);
    double theta = std::atan2(y, x); // Используем atan2 для корректного знака
    return {r, theta};
}


void DotMesh::setData(std::vector<Dot> dots){
    /*std::ofstream ffout("data.txt");
    for (int i = 0; i < 360; i+= shag){
        ffout << 5 << " " << i << "\n";
    }*/

    data = dots;
}

std::vector<Dot> DotMesh::getData(){
    /*std::ifstream fin ("data.txt");
    Dot temp;
    while(fin >> temp.R >> temp.ang){
        data.push_back(temp);
    }
    std::ofstream fout ("out.txt");
    for (auto i : data){
        fout << i.R << " " << i.ang << "\n";
    }
    center.R = 0;
    center.ang = 0;*/

    return data;
}

std::vector<Dot> DotMesh::outData(){
    return data;
}

std::ofstream fout ("dotsout.txt");
std::pair<Dot, Dot> DotMesh::findClosest(double ang, DotMesh& v){
    double left = 361, right = 361;
    std::pair<Dot, Dot> pairDots {};
    for (auto i : v.data){
        if (fabs(fmod((i.ang - ang),360)) < left) {
            right = left;
            left = fabs(i.ang - ang);
            pairDots.second = pairDots.first;
            pairDots.first = i;
        } else if (fabs(fmod((i.ang - ang),360)) < right) {
            right = fabs(i.ang - ang);
            pairDots.second = i;
        }
    }

    fout << pairDots.first.ang << " " << pairDots.second.ang << "\n";
    return pairDots;
}
std::ofstream out("check_vals.txt");
Dot DotMesh::getIntersec(Dot a, Dot b, Dot c){
    Point first = polarToDec(a);
    Point second = polarToDec(b);
    Point third = polarToDec(c);
    double k1 = first.y / first.x;
    double k2 = (third.y - second.y) / (third.x - second.x);
    double x_int = (second.y - k2 * second.x) / (k1 - k2);
    double y_int = k1 * x_int;
    out << x_int << " " << y_int << " - x y\n";
    out << first.x << " " << first.y << "\n";
    out << second.x << " " << second.y << "\n";
    out << third.x << " " << third.y << "\n";
    out << k1 << " " << k2 << " - k\n";
    Dot ans;
    ans.R = sqrt(x_int*x_int + y_int*y_int);
    ans.ang = rad_to_grad(atan(y_int / x_int));
    return ans;
}

void DotMesh::calcDiff(DotMesh& second){

    for (int i = 0; i < data.size(); i++){
        double angle = data[i].ang;
        std::pair<Dot, Dot> res_dots = findClosest(angle, second);
        Dot temp = getIntersec(data[i], res_dots.first, res_dots.second);
        delta.push_back(temp);
        delta_val.push_back(temp.R - data[i].R);
        out << data[i].R << " " << delta_val[i] << "\n";
    }
}

std::vector<double> DotMesh::get_delt(){
    return delta_val;
}

std::pair<DotMesh, DotMesh> solve(){
    DotMesh mesh, mesh2;/*
    mesh.setData(10);
    mesh.getData();
    mesh2.setData(7);
    mesh2.getData();
    mesh.calcDiff(mesh2);
    std::vector<double> diff;
    diff = mesh.get_delt();

    for (auto i : diff){
        std::cout << i << "\n";
    }*/
    return {mesh, mesh2};
}

double distToCircle(Point mid, Point val, double Rad){
    return fabs(Rad - sqrt((mid.x - val.x)*(mid.x - val.x) + (mid.y - val.y)*(mid.y - val.y)));
}

/*std::vector<std::pair<double, double>> void rf609_imitation(double D, double r, double dist, int N){
    std::vector<std::pair<double, double>> vals(N);
    double delta = 360.0 / N;
    int n = 0;
    for (auto i : vals){ i.second = n++ * delta; }
    for (auto i : vals){ std::cout << i.second << "\n"; }
}
*/
