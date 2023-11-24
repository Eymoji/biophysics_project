#pragma once

double rejetgaussien(double moyenne, double variance);
double uniform_distribution(double min, double max);
double titled_distribution(double Xmin, double Xmax, double x0, double ratio, double lambda);
double norm(double x, double y);
double norm2(double x, double y);
double periodic_dx(double x1, double x2, double Lx);
double theta(double x, double y);
std::vector<double> random_landscape_initializer();
std::vector<double> random_landscape_distribution(double Lx, double Ly, const std::vector<double>& W);
