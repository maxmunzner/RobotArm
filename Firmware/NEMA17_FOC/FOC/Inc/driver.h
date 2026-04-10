//
// Created by Max Münzner on 11.03.26.
//

#ifndef NEMA17_FOC_DRIVER_H
#define NEMA17_FOC_DRIVER_H

#include "main.h"

class Driver {
public:
    Driver(TIM_HandleTypeDef* timer, uint32_t range)
        : _timer(timer), _range(range) {}

    void init();
    void enable();
    void disable();

    void setPhases(float ua, float ub);

private:
    TIM_HandleTypeDef* _timer;
    uint32_t _range;
    bool _enabled = false;
};

#endif //NEMA17_FOC_DRIVER_H