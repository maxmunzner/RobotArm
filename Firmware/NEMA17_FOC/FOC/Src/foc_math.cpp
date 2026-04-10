//
// Created by Max Münzner on 11.03.26.
//

#include "../Inc/foc_math.h"


// Konstante für Q31 Skalierung (2^31)
#define Q31_SCALE 2147483648.0f
#define PI_FLOAT  3.1415926535f

FocMath::FocMath() {
    LL_CORDIC_Config(CORDIC,
LL_CORDIC_FUNCTION_SINE,
LL_CORDIC_PRECISION_6CYCLES,
LL_CORDIC_SCALE_0,
LL_CORDIC_NBWRITE_1,
LL_CORDIC_NBREAD_2,
LL_CORDIC_INSIZE_32BITS,
LL_CORDIC_OUTSIZE_32BITS);
}

void FocMath::updateSinCos(float angle_rad) {
    // 1. Winkel normalisieren (CORDIC erwartet Werte im Bereich -1 bis 1, was -PI bis PI entspricht)
    // Wir skalieren Radiant direkt in das Q31 Format für CORDIC
    int32_t angle_q31 = (int32_t)(angle_rad * (Q31_SCALE / PI_FLOAT));

    LL_CORDIC_WriteData(CORDIC, angle_q31);

    _sinVal = (float)(int32_t)LL_CORDIC_ReadData(CORDIC) / Q31_SCALE;
    _cosVal = (float)(int32_t)LL_CORDIC_ReadData(CORDIC) / Q31_SCALE;
}

ab_t FocMath::inversePark(dq_t dq) {
    ab_t ab;
    // Standard Inverse Park Gleichungen
    ab.alpha = dq.d * _cosVal - dq.q * _sinVal;
    ab.beta  = dq.d * _sinVal + dq.q * _cosVal;
    return ab;
}

dq_t FocMath::park(ab_t ab) {
    dq_t dq;
    // Standard Park Gleichungen
    dq.d =  ab.alpha * _cosVal + ab.beta * _sinVal;
    dq.q = -ab.alpha * _sinVal + ab.beta * _cosVal;
    return dq;
}