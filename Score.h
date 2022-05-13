#pragma once

#include <iostream>

struct Score {
    float MidTerm = 0, Final = 0, Other = 0, Total;

    void setScore(float __MidTerm, float __Final, float __Other);

    void calTotal();
};
