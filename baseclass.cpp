#include "baseclass.hpp"

std::vector <double> Base::vec_normalize(const std::vector <double> &v){
	double scale = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
	scale=1.0/(sqrt(scale));
	std::vector <double> out;
	std::transform(v.begin(), v.end(), std::back_inserter(out), [=](double x){ return x * scale; });
	return out;
}

std::vector <double> Base::ray_point_at(const Ray & ray0, const double t){
	std::vector <double> out;
	std::transform(ray0.first.begin(), ray0.first.end(), ray0.second.begin(), std::back_inserter(out), 
	[=](double x, double y)-> double { return x+t*y; });
	return out;
}

std::vector <double> Base::reflect(const std::vector <double> vec, const std::vector <double> normal){
	double scale = std::inner_product(vec.begin(), vec.end(), normal.begin(), 0.0);
	std::vector <double> out;
	std::transform(normal.begin(), normal.end(), std::back_inserter(out), [=](double x){ return x * 2.0 * scale; });
	std::transform(vec.begin(), vec.end(), out.begin(), out.begin(), 
	[](double x, double y)-> double { return x-y; });
	return out;
}

std::vector<double> Base::normal_at(std::vector<double> pnt){
		std::cout << "Inside Base: I should be overloaded and called from elsewhere." << std::endl;
		//This is only to suppress warnings of "no return in a non-void function"
		std::vector <double> out{0,0,0};
		return out;
}

std::pair < std::string, double > Base::intersection_time(Ray r0){
		std::cout << "Inside Base: I should be overloaded and called from elsewhere." << std::endl;
		//This is only to suppress warnings of "no return in a non-void function"
		std::pair < std::string, double > out_pair;
		return out_pair=std::make_pair("false",0);
}

std::vector<double> Base::surface_color(std::vector<double> pnt){
		std::cout << "Inside Base: I should be overloaded and called from elsewhere." << std::endl;
		//This is only to suppress warnings of "no return in a non-void function"
		std::vector <double> out{0,0,0};
		return out;
}

std::vector<double> Base::surface_mprop(std::vector<double> pnt){
		std::cout << "Inside Base: I should be overloaded and called from elsewhere." << std::endl;
		//This is only to suppress warnings of "no return in a non-void function"
		std::vector <double> out{0,0,0};
		return out;
}

void Base::print(void){ 
		std::cout << "Inside Base: I should be overloaded and called from elsewhere." << std::endl;
}
