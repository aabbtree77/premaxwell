#ifndef BASECLASS_HPP
#define BASECLASS_HPP
#include <iostream>
#include <string>
#include <memory> //std::shared_ptr
#include <utility> // std::pair, std::make_pair
#include <vector>
#include <cmath> // tan, atan, fabs
#include <algorithm> // std::for_each, std::transform, std::min, std::copy, std::find_if
#include <numeric> // std::inner_product

//#define EPSILON 0.00001
const double EPSILON = 0.00001;

typedef std::pair <std::vector <double>, std::vector <double> > Ray;

// A fictitious parent class of HalfPlane and Sphere.
// Its main purpose is to keep objects in an STL container inside main()
// and to access their functions via inheritance-based polymorphism

class Base {
	public:
	std::vector <double> vec_normalize(const std::vector <double> &v);
	std::vector <double> ray_point_at(const Ray & ray0, const double t);
	std::vector <double> reflect(const std::vector <double> vec, const std::vector <double> normal);
	virtual std::vector<double> normal_at(std::vector<double> pnt);
	virtual std::pair < std::string, double > intersection_time(Ray r0);
	virtual std::vector<double> surface_color(std::vector<double> pnt);
	virtual std::vector<double> surface_mprop(std::vector<double> pnt);
	virtual void print(void);
	virtual ~Base(){}
}; 

#endif
