#include "halfspace.hpp"

void HalfSpace::print(void){
	std::cout << std::endl << "The halfspace object:" << std::endl;
	std::cout << "Starting point of the normal: ";
	std::for_each(spoint.begin(), spoint.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;
	std::cout << "Surface normal: ";
	std::for_each(normal.begin(), normal.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;	
	std::cout << "The RGB vector: ";
	std::for_each(color.begin(), color.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;
	std::cout << "Material properties: ";
	std::for_each(mprop.begin(), mprop.end(), [](double x){std::cout << x << " ";});
	std::cout << std::endl;
}

std::pair<std::string,double> HalfSpace::intersection_time(Ray ray0){ 
	double wn, un;
	std::pair<std::string,double> out_pair;
	
	un = std::inner_product(ray0.second.begin(), ray0.second.end(), normal.begin(), 0.0);
	if(fabs(un) < EPSILON)
		return out_pair=std::make_pair("false",0);		

	std::vector <double> wvec;
	std::transform(ray0.first.begin(), ray0.first.end(), spoint.begin(), std::back_inserter(wvec), 
			[](double x, double y){return x-y;});
	wn = std::inner_product(wvec.begin(), wvec.end(), normal.begin(), 0.0);

	return out_pair=std::make_pair("true",-wn/un);
	//std::cout << "HalfSpace Intersection time:" << -1.0/un << std::endl;
	//return out_pair=std::make_pair("true",-1.0/un);
}

std::vector<double> HalfSpace::normal_at(std::vector<double> pnt){
	return normal;
}

std::vector<double> HalfSpace::surface_color(std::vector<double> pnt){
        /*
        //std::cout << "\n";
        for(int q=0; q<3; ++q){
                std::cout << pnt[q] << ' ';
        }
        std::cout << " \n";
	*/
	std::vector<double> c0{0,0,0};
	std::vector<double> c1{1,1,1};
	std::vector<double> surf_clr;
	int count = std::accumulate(pnt.begin(), pnt.end(), 0, 
	[](int current_sum, double element) { return current_sum + int(fabs(element)+0.5); });
	surf_clr= ((count % 2) ? c0 : c1);
	return surf_clr;
}

std::vector<double> HalfSpace::surface_mprop(std::vector<double> pnt){
	return mprop;
}
