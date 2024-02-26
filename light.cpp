#include "light.hpp"

void Light::print(void){
	std::cout << std::endl << "The Light object:" << std::endl;
	int ind=0;
	std::for_each(light_source_vv.begin(), light_source_vv.end(), 
	[&](std::vector <double> vec){
		std::cout << "Source" << ind << ": ";
		std::for_each(vec.begin(), vec.end(),
		[](double x){std::cout << x << " ";});
		std::cout << std::endl;
		++ind;
	});
}

bool Light::visible(std::vector<double> lght_src, std::vector<double> pnt, std::vector<std::shared_ptr<Base> > obj_ptr_vec){
	
	bool result=false;
	
	std::vector<double> diffv;
	std::transform(lght_src.begin(), lght_src.end(), pnt.begin(), back_inserter(diffv), 
					[](double x, double y){return x-y;});
	Base base0;
	diffv = base0.vec_normalize(diffv);				
	Ray ray0=std::make_pair(pnt, diffv);
	
	std::pair<std::string,double> temp_pair;
	std::vector<std::shared_ptr<Base> >::iterator it;
	it=std::find_if(obj_ptr_vec.begin(), obj_ptr_vec.end(), 
		[&](std::shared_ptr<Base> x)-> bool{ 
			temp_pair = x->intersection_time(ray0);
			return ((temp_pair.first =="true") && (temp_pair.second > EPSILON))? true : false;
		}
	);
	if(it==obj_ptr_vec.end())
		result=true;
	
	return result;
}

std::vector<double> Light::propagate(std::vector<std::vector<double> > point_vv, 
		std::vector<std::vector<double> > normal_vv, 
		std::vector<std::vector<double> > color_vv, 
		std::vector<std::vector<double> > mprop_vv,
		std::vector<std::shared_ptr<Base> > obj_ptr_vec){
		// Includes speculiar, diffuse (lambertian) and ambient light aspects.
		// There is no refraction.
		
		Base base1;
		
		double specular, ambient;
		double lambertian, lambert_amount, lambert_prod, lambert_contrib;
		
		std::vector<double> lght_src;
		std::vector<double> diff_vec(3);
		std::vector<double> intr_point;
		std::vector<double> intr_normal;
		std::vector<double> intr_color;
		std::vector<double> intr_mprop;

		std::vector<double> c0{0,0,0};
		std::vector<double> refl_color{0,0,0};
		
		int num_points=int(point_vv.size());
		if(!num_points){
			//std::cout << "No points to propagate" << std::endl; 
			return c0;
		}
		for(int i=num_points-1; i > -1; --i){
			//Reflection (specular) light
			if(i != (num_points-1)){ 
				//refl_color=c0;
				refl_color.assign(c0.begin(), c0.end());
			}else{
				refl_color.at(0)=0;
				refl_color.at(1)=0;
				refl_color.at(2)=0;
			}
			
			c0.at(0)=0;
		    c0.at(1)=0;
		    c0.at(2)=0;
			
			//Specular light coefficient
			specular=mprop_vv.at(i).at(0);
			std::transform(c0.begin(), c0.end(), refl_color.begin(), c0.begin(), 
			[=](double x, double y){return x+specular*y;});
			
			
			//Diffuse (lambertian) light
			intr_point = point_vv.at(i);
			intr_normal = normal_vv.at(i);
			intr_color = color_vv.at(i);
			if(1){
			lambert_amount=0;
			std::for_each(light_source_vv.begin(), light_source_vv.end(), [&](std::vector<double> lght_src){
				lambert_contrib=0;
				if(visible(lght_src, intr_point, obj_ptr_vec)){
					std::transform(lght_src.begin(), lght_src.end(), intr_point.begin(), diff_vec.begin(), 
					[](double x, double y){return x-y;});
					diff_vec = base1.vec_normalize(diff_vec);
					lambert_contrib = std::inner_product(diff_vec.begin(), diff_vec.end(), intr_normal.begin(), 0.0);
					if(lambert_contrib>0)
						lambert_amount += lambert_contrib;
				}
			});
			lambert_amount = std::min(1.0, lambert_amount);
			
			//Diffuse light coefficient
			lambertian=mprop_vv.at(i).at(1);
			lambert_prod=lambertian*lambert_amount;
			std::transform(c0.begin(), c0.end(), intr_color.begin(), c0.begin(), 
			[=](double x, double y){return x+lambert_prod*y;});
			}

			//Ambient light coefficient
			ambient=mprop_vv.at(i).at(2);
			std::transform(c0.begin(), c0.end(), intr_color.begin(), c0.begin(), 
			[=](double x, double y){return x+ambient*y;});
		}
		
		return c0;	
}
