#include "sphere.hpp"

void Sphere::print(void){
	std::cout << std::endl << "The sphere object:" << std::endl;
	std::cout << "Center: ";
	std::for_each(center.begin(), center.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;
	std::cout << "Radius: ";
	std::cout << radius << std::endl;	
	std::cout << "The RGB vector: ";
	std::for_each(color.begin(), color.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;
	std::cout << "Material properties: ";
	std::for_each(mprop.begin(), mprop.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;
}

std::pair<std::string,double> Sphere::intersection_time(Ray ray0){ 
	double v, sqnormcp, discriminant;
	std::pair<std::string, double> out_pair;
	
	std::vector <double> cp;
	std::transform(center.begin(), center.end(), ray0.first.begin(), std::back_inserter(cp), 
			[](double x, double y){return x-y;});
	sqnormcp = std::inner_product(cp.begin(), cp.end(), cp.begin(), 0.0);
	v = std::inner_product(cp.begin(), cp.end(), ray0.second.begin(), 0.0);
	
	discriminant = radius*radius-(sqnormcp-v*v);
	//std::cout << "Sphere Intersection time:" << v-sqrt(discriminant) << std::endl;
	if(discriminant<0){
		return out_pair=std::make_pair("false",0);
	}
	else{
		return out_pair=std::make_pair("true",v-sqrt(discriminant));
	}
}

std::vector<double> Sphere::normal_at(std::vector<double> pnt){
	std::vector <double> out;
	std::transform(pnt.begin(), pnt.end(), center.begin(), std::back_inserter(out), 
			[](double x, double y){return x-y;});
	return vec_normalize(out);
}

std::vector<double> Sphere::surface_color(std::vector<double> pnt){
	return color;
}

std::vector<double> Sphere::surface_mprop(std::vector<double> pnt){
	return mprop;
}
