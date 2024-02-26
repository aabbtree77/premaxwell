#ifndef HALFSPACE_HPP
#define HALFSPACE_HPP

#include "baseclass.hpp"

class HalfSpace : public Base{
	public:
		HalfSpace(std::vector<double> p0,std::vector<double> v0,std::vector<double> c0,std::vector<double> m0) 
		:spoint(p0), normal(v0), color(c0), mprop(m0) {} 
		~HalfSpace(){}
		void print(void);
		std::pair < std::string, double > intersection_time(Ray r0);
		std::vector<double> normal_at(std::vector<double> pnt);
		std::vector<double> surface_color(std::vector<double> pnt);
		std::vector<double> surface_mprop(std::vector<double> pnt);
	private:
		std::vector<double> spoint;
		std::vector<double> normal;
		std::vector<double> color;
		std::vector<double> mprop; //Material properties: specular, lambert and ambient coeffs, should sum to 1.
};

#endif
