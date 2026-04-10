//
// Created by Max Münzner on 11.03.26.
//

#ifndef NEMA17_FOC_FOC_MATH_H
#define NEMA17_FOC_FOC_MATH_H

#include "main.h"
#include "stm32g4xx_ll_cordic.h"

struct dq_t { float d; float q; };
struct ab_t { float alpha; float beta; };

class FocMath {
public:
    FocMath();

    /**
     * Berechnet Sinus und Cosinus gleichzeitig via CORDIC
     * @param angle_rad Winkel in Radiant (-PI bis PI)
     */
    void updateSinCos(float angle_rad);

    /**
     * Inverse Park Transformation
     * Wandelt d/q Sollwerte in alpha/beta um
     */
    ab_t inversePark(dq_t dq);

    /**
     * Park Transformation (für die Strommessung)
     * Wandelt gemessene alpha/beta Werte in d/q um
     */
    dq_t park(ab_t ab);

    // Zugriff auf die aktuellen Werte
    float getSin() const { return _sinVal; }
    float getCos() const { return _cosVal; }

private:
    float _sinVal = 0.0f;
    float _cosVal = 1.0f;
};
#endif //NEMA17_FOC_FOC_MATH_H