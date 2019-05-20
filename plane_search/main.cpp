#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>

#define enable_output true

using namespace std;

struct coords {
    double x;
    double y;
    double z;
    // Расстояние от точки до плоскости
    double error;
    int sign;
    // Флаг того, что выбираем эту точку для следующей итерации MSE;
    int isValid;
};

typedef struct coords Coords;

void calc_averages(const vector<Coords> &data, Coords &avg) {
    int n = data.size();
    avg.x = avg.y = avg.z = 0;
    for(int i = 0; i < n ; ++i) {
        avg.x+=data[i].x;
        avg.y+=data[i].y;
        avg.z+=data[i].z;
    }
    avg.x /= n;
    avg.y /= n;
    avg.z /= n;
}

void calc_dispersion(const vector<Coords> &data,
                     const Coords &avg,
                     Coords &s2) {
    int n = data.size();
    s2.x = s2.y = s2.z = 0;
    for(int i = 0; i < n ; ++i) {
        s2.x+=pow(data[i].x - avg.x, 2);
        s2.y+=pow(data[i].y - avg.y, 2);
        s2.z+=pow(data[i].z - avg.z, 2);
    }
    s2.x /= (n - 1);
    s2.y /= (n - 1);
    s2.z /= (n - 1);
}

void calc_covariance(const vector<Coords> &data,
                     const Coords &avg,
                     Coords &cov) {
    int n = data.size();
    // x = cov(x,y), y = cov(у,x), z = cov(z,x)
    cov.x = cov.y = cov.z = 0;
    for(int i = 0; i < n ; ++i) {
        cov.x+=(data[i].x - avg.x)*(data[i].y - avg.y);
        cov.y+=(data[i].y - avg.y)*(data[i].z - avg.z);
        cov.z+=(data[i].z - avg.z)*(data[i].x - avg.x);
    }
    cov.x /= (n - 1);
    cov.y /= (n - 1);
    cov.z /= (n - 1);
}

void regression(
                vector<Coords> &data,
                double &b2, double &b1, double &b0
                ) {
    Coords avg, disp, cov;
    calc_averages(data, avg);
    calc_dispersion(data, avg, disp);
    calc_covariance(data, avg, cov);

    if (enable_output) {
        cout << endl << "Averages:" << endl;
        cout << avg.x << " " << avg.y << " " << avg.z << endl;
        cout << endl << "Dispersion" << endl;
        cout << disp.x << " " << disp.y << " " << disp.z << endl;
        cout << endl << "Covariation:" << endl;
        cout << cov.x << " " << cov.y << " " << cov.z << endl;
    }
    //Regression coefficients

    b1 = (cov.z*disp.y - cov.y*cov.x)/(disp.x*disp.y - cov.x*cov.x);
    b2 = (disp.x*cov.y - cov.x*cov.z)/(disp.x*disp.y - cov.x*cov.x);
    b0 = (avg.z - b1*avg.x - b2*avg.y);
}

double calc_errors(vector<Coords> &data, double b2, double b1, double b0, double p) {
    int n = data.size();
    // Вводим знак, чтобы понимать в какой части пространства находится точка и снабжаем расстояние этим знаком
    int sign = 1;
    int validPoints = 0;
    for (int i = 0; i < n ; ++i) {
        if (data[i].z >= (b2*data[i].x + b1*data[i].y + b0)) {
            sign = 1;
        } else {
            sign = -1;
        }
        data[i].error = abs((b1*data[i].x + b2*data[i].y + (-1)*data[i].z + b0))/sqrt(b2*b2 + b1*b1 + 1);
        data[i].sign = sign;
        if (data[i].error <= p) {
            data[i].isValid = 1;
            ++validPoints;
        } else {
            data[i].isValid = 0;
        }
    }
    return static_cast<double>(validPoints)/n;
}

bool compareErrors(Coords c1 , Coords c2) {
    return  c1.error < c2.error;
}



int main() {

    double p = 0;
    int size = 0;
    int idx = 0; //Индекс массива точек
    vector<Coords> data;

    //Reading source file

    //cloud.open("/Users/arseniy/data/plane_search/sdc_point_cloud.txt", ios::in);
    //cloud.open("/Users/arseniy/data/plane_search/test1.txt", ios::in);



    ifstream cloud;
    ofstream result("./output.txt");

    cloud.open("./input.txt", ios::in);

    if (!cloud.is_open()) {
        cout << "File is not found" << endl;
        return EXIT_FAILURE;
    }



    cout.setf(ios::fixed);
    cloud.setf(ios::fixed);
    cloud >> p >> size;

    data.clear();
    data.resize(size);

    idx = 0;
    do {
        cloud >> data[idx].x >> data[idx].y >> data[idx].z;
        ++idx;
    } while (!cloud.eof() && idx < size);

    cloud.close();

     /*

    cin.setf(ios::fixed);
    cin >> p >> size;

    data.clear();
    data.resize(size);

    idx = 0;
    do {
        cin >> data[idx].x >> data[idx].y >> data[idx].z;
        ++idx;
    } while (idx < size);

    */

    // Проверяем корректность вывода
    if (enable_output) {
        cout << "Accuracy: " << p << endl;
        cout << "Array length: " << data.size() << endl;
        cout << "Data:" << endl;
        for (int i = 0; i < 5; i++) {
            cout << data[i].x << " " << data[i].y << " " << data[i].z << endl;
        }
    }

    // Multivariate linear regression using MSE
    double b0 = 0, b1 = 0, b2 = 0;
    double error, error2;
    int reduction;
    int n = data.size();
    vector<Coords> td;

    td = data;
    reduction = (n/100 > 1) ? (n/50):1;

    do {
        regression(td, b2, b1, b0);
        if (enable_output) {
            cout << endl << "Coefficients:" << endl;
            cout << "A: " << -b1 << endl;
            cout << "B: " << -b2 << endl;
            cout << "C: " << 1 << endl;
            cout << "D: " << -b0 << endl;
        }
        error = calc_errors(data, b2, b1, b0, p);
        calc_errors(td, b2, b1, b0, p);

        if (enable_output) {
            cout << endl << "Array length: " << td.size() << endl;
            cout << "Errors: " <<  (1-error)*100 << "%"<<endl;
        }

        if (error > 0.5) break;

        sort(td.begin(), td.end(), compareErrors);

        if (enable_output) {
            cout << endl << "Sorted data:" << endl;
            for (int i = td.size() - 10; i < td.size(); ++i) {
                cout << td[i].x << "\t" << td[i].y << "\t" << td[i].z << "\t" << td[i].error * td[i].sign << endl;
            }
        }
        td.resize(td.size() - reduction);

    } while(td.size() > n/2);

    if (enable_output) {
        cout << endl << "First data:" << endl;
        int k = (td.size() > 10) ? 5 : 0;
        for (int i = 0; i < 5; ++i) {
            cout << td[i].x << "\t" << td[i].y << "\t" << td[i].z << "\t" << td[i].error * td[i].sign << endl;
        }

        cout << endl << "Last data:" << endl;

        for (int i = td.size() - 5; i < td.size(); ++i) {
            cout << td[i].x << "\t" << td[i].y << "\t" << td[i].z << "\t" << td[i].error * td[i].sign << endl;
        }
    }

    if(!enable_output) {
        result.setf(ios::fixed);
        result << -b1 << " " << -b2 << " "<< -1.0 << " "<<-b0 << endl;
        result.close();
    }

    if(enable_output) {
        cout.setf(ios::fixed);
        cout.precision(6);
        cout << endl << "Final coefficient" << endl;
        cout << -b1 << " " << -b2 << " "<< 1.0 << " "<<-b0 << endl;
    }

    return EXIT_SUCCESS;
}