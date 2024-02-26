#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "baseclass.hpp"

class Sphere : public Base{
	public:
		Sphere(std::vector<double> cntr0, double r0, std::vector<double> c0, std::vector<double> m0)
		:center(cntr0), radius(r0), color(c0), mprop(m0) {} 
		~Sphere(){}
		void print(void);
		std::pair < std::string, double > intersection_time(Ray r0);
		std::vector<double> normal_at(std::vector<double> pnt);
		std::vector<double> surface_color(std::vector<double> pnt);
		std::vector<double> surface_mprop(std::vector<double> pnt);
	private:
		std::vector<double> center;
		double              radius;
		std::vector<double>    color;
		std::vector<double> mprop; //Material properties: specular, lambert and ambient coeffs, should sum to 1.
};

#endif
