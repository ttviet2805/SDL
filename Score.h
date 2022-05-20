#pragma once

#include <iostream>
#include <string.h>

struct Score {
    float MidTerm = 0, Final = 0, Other = 0, Total;
    char GPA;

    void setScore(float __MidTerm, float __Final, float __Other);

    void calTotal();
};
