#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "baseclass.hpp"

class Light {
	public:
		Light(std::vector < std::vector<double> > light_source_vv0) 
		:light_source_vv(light_source_vv0) {} 
		~Light(){}
		void print(void);
		bool visible(std::vector<double> lght_src, std::vector<double> pnt, std::vector<std::shared_ptr<Base> > obj_ptr_vec);
		std::vector<double> propagate(std::vector<std::vector<double> > point_vv, 
		std::vector<std::vector<double> > normal_vv, 
		std::vector<std::vector<double> > color_vv, 
		std::vector<std::vector<double> > mprop_vv,
		std::vector<std::shared_ptr<Base> > obj_ptr_vec);
	private:
		std::vector < std::vector<double> > light_source_vv;
};

#endif
