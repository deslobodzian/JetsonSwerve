#include "../include/Encoder.hpp"

//******************for absolute positions**********************************************

float Encoder::get_position_rads_abs() {

    
    float position_rads = abs_pos_rad_;

    return position_rads;
}

float Encoder::get_position_deg_abs() {

    
    float position_deg = abs_pos_rad_*180/(M_PI);

    return position_deg;
}

//***********for quad positions**********************************************************

float Encoder::get_position_rads_quad() {

    float calibrated_pos_rad=quad_pos_rad_+abs_pos_rad_;

    return calibrated_pos_rad;
}

float Encoder::get_position_deg_quad() {

    float calibrated_pos_rad=quad_pos_rad_+abs_pos_rad_;

    float calibrated_pos_deg=calibrated_pos_rad*180/(M_PI);

    return calibrated_pos_deg;
}

//*********************for velocitiy***********************************************

float Encoder::get_velocity() {

//gotta do this and rot count later

}

float Encoder::get_rot_count() {

}