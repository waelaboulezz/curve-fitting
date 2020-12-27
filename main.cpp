//
//  main.cpp
//  Interpolation
//
//  Created by Wael Aboulezz on 23/12/2020.
//

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <algorithm> 
#include "Coursework1/NewtonInterpolator.hpp"
#include "Coursework1/CubicSpline.hpp"
#include <random>

using namespace std;

void write_csv(std::string filename, std::vector<string> colnames, std::vector<vector<double>> vals){

    ofstream outputFile;
    outputFile.open(filename);
    for (int i = 0; i < colnames.size(); i++) {
        outputFile << colnames[i] << ",";
        for(int j = 0; j < vals[i].size(); j++)
        {
            outputFile << vals[i].at(j) << ",";
        }
        outputFile << std::endl;
    }
    outputFile.close();

}

std::vector<std::pair<std::string, std::vector<double>>> read_csv(std::string filename){
    std::vector<std::pair<std::string, std::vector<double>>> result;
    std::ifstream myFile(filename);
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");
    std::string line, colname;
    double val;
    if(myFile.good())
    {
        std::getline(myFile, line);
        std::stringstream ss(line);
        while(std::getline(ss, colname, ',')){
            result.push_back({colname, std::vector<double> {}});
        }
    }
    while(std::getline(myFile, line))
    {
        std::stringstream ss(line);
        int colIdx = 0;
        while(ss >> val){
            result.at(colIdx).second.push_back(val);
            if(ss.peek() == ',') ss.ignore();
            colIdx++;
        }
    }
    myFile.close();
    return result;
}
std::vector<vector<double>> getPointsNewton(const vector<double> &x, vector<double> f, int n, NewtonInterpolator Newton, vector<double> coeff)
{
    double lower_bound = *min_element(x.begin(), x.end());
    double upper_bound = *max_element(x.begin(), x.end());
    
    vector<double> newX = x;
    vector<double> newY = f;
    
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    for (int i = 0; i < n; i++)
    {
        double a_random_double = unif(re);
        newX.push_back(a_random_double);
        newY.push_back(Newton.interpolate(coeff,a_random_double));
        cout << "x = " << a_random_double << '\n';
        cout << "Newton Interpolation: " << Newton.interpolate(coeff,a_random_double) << '\n';
    }
    
    std::vector<vector<double>> newPoints(2);
    newPoints[0] = newX;
    newPoints[1] = newY;
    
    return newPoints;
}

std::vector<vector<double>> getPointsSpline(const vector<double> &x, vector<double> f, int n, Spline cs)
{
    double lower_bound = *min_element(x.begin(), x.end());
    double upper_bound = *max_element(x.begin(), x.end());
    
    vector<double> newX = x;
    vector<double> newY = f;
    
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    for (int i = 0; i < n; i++)
    {
        double a_random_double = unif(re);
        newX.push_back(a_random_double);
        newY.push_back(cs.interpolateSpline(a_random_double));
        cout << "x = " << a_random_double << '\n';
        cout << "Cubic Spline Interpolation: " << cs.interpolateSpline(a_random_double) << '\n';
    }
    
    std::vector<vector<double>> newPoints(2);
    newPoints[0] = newX;
    newPoints[1] = newY;
    
    return newPoints;
}

int main()
{
    //vector<double> x = {1, 2, 3, 4};
    //vector<double> y = {6, 9, 2, 5};
    std::vector<std::pair<std::string, std::vector<double>>> dataset_1 = read_csv("/Users/waelaboulezz/Documents/MSc Informatics/coursework_1 2/datasets/part_3/3_dataset_1.csv");
    std::vector<std::pair<std::string, std::vector<double>>> dataset_2 = read_csv("/Users/waelaboulezz/Documents/MSc Informatics/coursework_1 2/datasets/part_3/3_dataset_2.csv");

    vector<double> x1 = dataset_1.at(0).second;
    vector<double> y1 = dataset_1.at(1).second;
    vector<double> x2 = dataset_2.at(0).second;
    vector<double> y2 = dataset_2.at(1).second;

    NewtonInterpolator Newton;

    vector<double> a1 = Newton.fit(x1,y1);
    cout << "Newton Interpolation Polynomial: "; Newton.writeNewtonPolynomial(a1);
//    double test = 2.5;
//    cout << "Interpolate for x = " << test << '\n';
//    cout << "Interpolated value: " << Newton.interpolate(a1,test) << '\n';

    vector<double> a2 = Newton.fit(x2,y2);
    cout << "Newton Interpolation Polynomial: "; Newton.writeNewtonPolynomial(a2);
//    cout << "Interpolate for x = " << test << '\n';
//    cout << "Interpolated value: " << Newton.interpolate(a2,test) << '\n';
//
    std::vector<string> colnames(2);
    colnames[0] = "Newton Coefficients Dataset 1";
    colnames[1] = "Newton Coefficients Dataset 2";
    std::vector<vector<double>> coeff(2);
    coeff[0] = a1;
    coeff[1] = a2;
    write_csv("Newton Polynomial Coefficients.csv",colnames,coeff);
    
    std::vector<string> colnamesnewp(2);
    colnamesnewp[0] = "X";
    colnamesnewp[1] = "Y";
    
    vector<vector<double>> newPointsNewton2D1 = getPointsNewton(x1,y1,x1.size(),Newton,a1);
    write_csv("Newton 2 Times Dataset1.csv",colnamesnewp,newPointsNewton2D1);
    vector<vector<double>> newPointsNewton4D1 = getPointsNewton(x1,y1,x1.size()*3,Newton,a1);
    write_csv("Newton 4 Times Dataset1.csv",colnamesnewp,newPointsNewton4D1);
    vector<vector<double>> newPointsNewton2D2 = getPointsNewton(x2,y2,x2.size(),Newton,a2);
    write_csv("Newton 2 Times Dataset2.csv",colnamesnewp,newPointsNewton2D2);
    vector<vector<double>> newPointsNewton4D2 = getPointsNewton(x2,y2,x2.size()*3,Newton,a2);
    write_csv("Newton 4 Times Dataset2.csv",colnamesnewp,newPointsNewton4D2);
//    write_csv("newtonPolynomial_dataset2.csv","Newton Coefficients",a2);

//   std::vector<double> X(5), Y(5);
//   X[0]=0.1; X[1]=0.4; X[2]=1.2; X[3]=1.8; X[4]=2.0;
//   Y[0]=0.1; Y[1]=0.7; Y[2]=0.6; Y[3]=1.1; Y[4]=0.9;


    Spline s;
    vector<vector<double>> cSpline1 = s.fitSpline(x1,y1);
//    printf("Spline_D1 at %f is %f\n", test, s.interpolateSpline(test));
    
    vector<vector<double>> newPointsSpline2D1 = getPointsSpline(x1,y1,x1.size(),s);
    write_csv("Spline 2 Times Dataset1.csv",colnamesnewp,newPointsSpline2D1);
    vector<vector<double>> newPointsSpline4D1 = getPointsSpline(x1,y1,x1.size()*3,s);
    write_csv("Spline 4 Times Dataset1.csv",colnamesnewp,newPointsSpline4D1);
    vector<vector<double>> newPointsSpline2D2 = getPointsSpline(x2,y2,x2.size(),s);
    write_csv("Spline 2 Times Dataset2.csv",colnamesnewp,newPointsSpline2D2);
    vector<vector<double>> newPointsSpline4D2 = getPointsSpline(x2,y2,x2.size()*3,s);
    write_csv("Spline 4 Times Dataset2.csv",colnamesnewp,newPointsSpline4D2);
//    for (int j = 0; j < cSpline1.size(); j++){
//        for (int i = 0; i < cSpline1[j].size(); i++) {
//            cout << cSpline1[j][i] << " ";
//        }
//        cout << endl << "This vector size is " << cSpline1[j].size() << endl;
//    }
    

    std::vector<string> colnamesSpline1(5);
    colnamesSpline1[0] = "Spline x Dataset 1";
    colnamesSpline1[1] = "Spline a Dataset 1";
    colnamesSpline1[2] = "Spline b Dataset 1";
    colnamesSpline1[3] = "Spline c Dataset 1";
    colnamesSpline1[4] = "Spline y Dataset 1";

    
    write_csv("Cubic Spline Interpolator Dataset 1.csv",colnamesSpline1,cSpline1);
    
    vector<vector<double>> cSpline2 = s.fitSpline(x2,y2);
//    printf("Spline_D2 at %f is %f\n", test, s.interpolateSpline(test));
    std::vector<string> colnamesSpline2(5);
    colnamesSpline2[0] = "Spline x Dataset 2";
    colnamesSpline2[1] = "Spline a Dataset 2";
    colnamesSpline2[2] = "Spline b Dataset 2";
    colnamesSpline2[3] = "Spline c Dataset 2";
    colnamesSpline2[4] = "Spline y Dataset 2";
    
    write_csv("Cubic Spline Interpolator Dataset 2.csv",colnamesSpline2,cSpline2);
    
    return 0;
}


