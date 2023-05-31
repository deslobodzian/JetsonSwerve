//
// Created by prometheus on 4/27/23.
//

#pragma once

#include <frc/EigenCore.h>
#include <frc/system/LinearSystemLoop.h>
#include <OutliersTalon.hpp>


class DiffSwerveModule {
public:
private:
    OutliersTalon left_falcon_;
    OutliersTalon right_falcon_;

    frc::LinearSystemLoop<3, 2, 3> module_control_loop_;
    frc::Vectord<3> reference_;
    frc::Vectord<2> u_;

};