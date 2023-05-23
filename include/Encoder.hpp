#pragma once
#include <cmath>

class Encoder {
public:
    Encoder();
    ~Encoder();
    float get_position_rads_abs();
    float get_position_deg_abs();
    float get_position_rads_quad();
    float get_position_deg_quad();
    float get_velocity();
    float get_rot_count();
private:
    float abs_pos_rad;
    float quad_pos_rad;

};
