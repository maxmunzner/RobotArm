//
// Created by Max Münzner on 11.03.26.
//
#include "../Inc/driver.h"

void Driver::init() {
    HAL_TIM_PWM_Start(_timer, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(_timer, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(_timer, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(_timer, TIM_CHANNEL_4);
    __HAL_TIM_MOE_ENABLE(_timer);
    disable();
}

void Driver::enable() {
    // Hier die EN-Pins für den DRV8844 auf High setzen
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
    _enabled = true;
}

void Driver::disable() {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
    _timer->Instance->CCR1 = 0;
    _timer->Instance->CCR2 = 0;
    _timer->Instance->CCR3 = 0;
    _timer->Instance->CCR4 = 0;
    _enabled = false;
}

void Driver::setPhases(float ua, float ub) {
    if(!_enabled) return;

    // Begrenzung (Sättigung) auf -1.0 bis 1.0
    if (ua > 1.0f) ua = 1.0f; if (ua < -1.0f) ua = -1.0f;
    if (ub > 1.0f) ub = 1.0f; if (ub < -1.0f) ub = -1.0f;

    // Umrechnung: -1.0 -> 0 | 0.0 -> range/2 | 1.0 -> range
    uint32_t ccr1 = (uint32_t)((ua * 0.5f + 0.5f) * _range);
    uint32_t ccr3 = (uint32_t)((ub * 0.5f + 0.5f) * _range);

    // Komplementäre Ansteuerung für die H-Brücken
    _timer->Instance->CCR1 = ccr1;
    _timer->Instance->CCR2 = _range - ccr1;
    _timer->Instance->CCR3 = ccr3;
    _timer->Instance->CCR4 = _range - ccr3;
}