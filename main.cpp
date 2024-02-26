#include "baseclass.hpp"
#include "halfspace.hpp"
#include "sphere.hpp"
#include "light.hpp"
#include <fstream>  // std::ofstream
#include <chrono>
#include <limits>

const double MAX_DOUBLE = std::numeric_limits<double>::max();
const double PI = atan(1.0)*4.0;
const int    INTR_DEPTH = 20; 

int main(){
	
	//std::cout << "Maximal value of type double is " << MAX_DOUBLE << std::endl;
	
	typedef std::chrono::high_resolution_clock Clock;
        typedef std::chrono::milliseconds milliseconds;
        Clock::time_point t0 = Clock::now();
	
	Base base0;
	
	//Light sources
	std::vector < std::vector<double> > light_source_vv;
	std::vector <double> light_source{30, 30, 10};
	light_source_vv.push_back(light_source);
	light_source.at(0)=-10;
	light_source.at(1)=100;
	light_source.at(2)=30;
	light_source_vv.push_back(light_source);
	Light light0(light_source_vv);
	light0.print();
	
	
	//Vector of shared pointers to geometrical primitives
	typedef std::shared_ptr<Base> shared_ptr_type;
	std::vector<shared_ptr_type> obj_ptr_vec;
	
	std::vector <double> hlf_spoint{0,0,0};
	std::vector <double> hlf_normal{0, 1, 0};
	std::vector <double> hlf_color{0,0,0};
	std::vector <double> hlf_mprop{0.2, 0.6, 0.2};
	obj_ptr_vec.push_back(std::make_shared<HalfSpace>(hlf_spoint,hlf_normal,hlf_color,hlf_mprop));
	//The other way is to explicitly create a shared pointer:
	//HalfSpace hlf01(hlf_spoint,hlf_normal,hlf_color,hlf_mprop);
	//std::shared_ptr<Base> ptr = std::make_shared<HalfSpace>(hlf01);
	//std::shared_ptr<Base> ptr(new HalfSpace(hlf01));
	//ptr->print();
	
	std::vector <double> sph_center{1,3,-10};
	double sph_radius=2;
	std::vector <double> sph_color{1,1,0};
	std::vector <double> sph_mprop{0.2, 0.6, 0.2};
	obj_ptr_vec.push_back(std::make_shared<Sphere>(sph_center,sph_radius,sph_color,sph_mprop));

        for(int i=0; i<6; ++i){
                sph_center[0] = -3 - ((double)i)*0.4;
                sph_center[1] =  2.3;
                sph_center[2] =  -5;
                sph_radius = 0.4;
                sph_color[0] = ((double)i)/6.0, 
                sph_color[1] = 1 - ((double)i)/6.0; 
                sph_color[2] = 0.5;
                obj_ptr_vec.push_back(std::make_shared<Sphere>(sph_center,sph_radius,sph_color,sph_mprop));         
        }
	std::for_each(obj_ptr_vec.begin(), obj_ptr_vec.end(), [](shared_ptr_type x){ x->print();});
	std::cout << std::endl;

	int canvas_width=1920*1;
	int canvas_height=1080*1;
	double fov=45.0;
	double fov_rad = PI*(fov/2.0)/180.0;
        double half_width = tan(fov_rad);
        double half_height = (((double)canvas_height)/((double)canvas_width)) * half_width;
        double width = half_width * 2;
        double height = half_height * 2;
        double pixel_width = width / (double(canvas_width - 1));
        double pixel_height = height / (double(canvas_height - 1));

	std::vector <double> position{0,1.8,10};
	std::vector <double> looking_at{0,3,0};
	
	std::vector <double> diffv;
	std::transform(looking_at.begin(), looking_at.end(), position.begin(), back_inserter(diffv), 
	[](double x, double y){return x-y;});
	
	diffv = base0.vec_normalize(diffv);
	Ray eye=std::make_pair(position, diffv);

	auto cross_prod = [](std::vector <double> v1, std::vector <double> v2) -> std::vector <double> {
		std::vector <double> result;
		result.push_back(v1.at(1) * v2.at(2) - v1.at(2) * v2.at(1));
		result.push_back(v1.at(2) * v2.at(0) - v1.at(0) * v2.at(2));
		result.push_back(v1.at(0) * v2.at(1) - v1.at(1) * v2.at(0));
		return result;
	};

	std::vector <double> vector_up{0,1,0};
	std::vector <double> vp_right=base0.vec_normalize(cross_prod(eye.second, vector_up));
	std::vector <double> vp_up=base0.vec_normalize(cross_prod(vp_right, eye.second));
	
	int id, min_obj_id, ind_glob, ind_loc;
	double scale, min_time;
	
	std::pair<std::string,double> temp_pair;
	std::vector<double> reflected_vec;
	std::vector<double> ray_vec(3); //very important to preallocate sizes when used with std::copy
	std::vector <double> xproj(3), yproj(3); 
	
	std::vector<double> intr_point; 
	std::vector<double> intr_normal;
	std::vector<double> intr_color; 
	std::vector<double> intr_mprop;
	
	std::vector<std::vector<double> > point_vv;
	std::vector<std::vector<double> > normal_vv;
	std::vector<std::vector<double> > color_vv;
	std::vector<std::vector<double> > mprop_vv;

	std::vector<double> color_out;
	Ray ray0;
	int buffer_size=canvas_height*canvas_width*3;
	char* buffer = new char[buffer_size];
	//std::cout << "pixel_width " << pixel_width << ", pixel_height "<< pixel_height << "\n";
	for(int i=0; i<canvas_height; ++i){
		for(int j=0; j<canvas_width; ++j){

			scale = j * pixel_width - half_width;
			std::transform(vp_right.begin(), vp_right.end(), xproj.begin(), 
			[=](double x){ return (x * scale); });
			
			scale = i * pixel_height - half_height;
			std::transform(vp_up.begin(), vp_up.end(), yproj.begin(), 
			[=](double x){ return (x * scale); });

			std::copy(eye.second.begin(), eye.second.end(), ray_vec.begin());
			// or ray_vec.assign(eye.second.begin(), eye.second.end());
			std::transform(ray_vec.begin(), ray_vec.end(), xproj.begin(), ray_vec.begin(), 
			[](double x, double y){return x+y;});
			std::transform(ray_vec.begin(), ray_vec.end(), yproj.begin(), ray_vec.begin(), 
			[](double x, double y){return x+y;});
			
			/*
			//std::cout << "xproj " << xproj << ", yproj "<< yproj << "\n";
			for(int q=0; q<3; ++q){
                               std::cout << xproj[q] << ' ';
                       }
                       std::cout << " ";
                       for(int q=0; q<3; ++q){
                               std::cout << yproj[q] << ' ';
                       }
                       std::cout << " \n";
                       */        
			ray_vec = base0.vec_normalize(ray_vec);
			ray0=std::make_pair(eye.first,ray_vec);
			
			std::vector<std::vector<double> > ().swap(point_vv);
			std::vector<std::vector<double> > ().swap(normal_vv);
			std::vector<std::vector<double> > ().swap(color_vv);
			std::vector<std::vector<double> > ().swap(mprop_vv);
			for( int depth=0; depth < INTR_DEPTH; ++depth){

				id=0;
				min_obj_id=-1;
				min_time=MAX_DOUBLE;
				std::for_each(obj_ptr_vec.begin(), obj_ptr_vec.end(), 
					[&](shared_ptr_type x){ 
						temp_pair = x->intersection_time(ray0);
						if(temp_pair.first =="true"){
							if((temp_pair.second < min_time) && (temp_pair.second > EPSILON)){
								min_time=temp_pair.second;
								min_obj_id=id;
							}
						}
						++id;
					}
				);

				if(min_time != MAX_DOUBLE){
					//At the intersection: Point, normal, color, material properties
					intr_point=base0.ray_point_at(ray0, min_time);
					intr_normal=obj_ptr_vec.at(min_obj_id)->normal_at(intr_point);
					intr_color=obj_ptr_vec.at(min_obj_id)->surface_color(intr_point);
					intr_mprop=obj_ptr_vec.at(min_obj_id)->surface_mprop(intr_point);
					//Save
					point_vv.push_back(intr_point);
					normal_vv.push_back(intr_normal);
					color_vv.push_back(intr_color);
					mprop_vv.push_back(intr_mprop);
					//New ray
					reflected_vec=base0.reflect(ray0.second, intr_normal);
					reflected_vec = base0.vec_normalize(reflected_vec);
					ray0=std::make_pair(intr_point,reflected_vec);
				}
				else break;
			}

			color_out=light0.propagate(point_vv, normal_vv, color_vv, mprop_vv, obj_ptr_vec);
			ind_glob=((canvas_height-i-1)*canvas_width+j)*3;
			ind_loc=0;
			std::for_each(color_out.begin(), color_out.end(), [&](double element){
				buffer[ind_glob+ind_loc]=char(std::max(0, std::min(255, int(element*255))));
				++ind_loc;
			}); 
		}
	}
	
	std::ofstream fout;
	fout.open("out_cpp.ppm", std::ios::out | std::ios::binary);
	if(!fout.is_open()) std::cout << "Failed to open the output file.\n";
	fout << "P6" << " " << canvas_width << " " << canvas_height << " 255" << std::endl;
	fout.write (buffer, buffer_size);
	fout.close();
	delete[] buffer;
	
	Clock::time_point t1 = Clock::now();
        milliseconds ms = std::chrono::duration_cast<milliseconds>(t1 - t0);
        std::cout << ms.count() << " ms.\n";
}
