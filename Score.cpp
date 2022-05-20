#include "Score.h"

void Score::setScore(float __MidTerm, float __Final, float __Other) {
    MidTerm = __MidTerm, Final = __Final, Other = __Other;
}

void Score::calTotal() {
    Total = (MidTerm * 30 + Final * 50 + Other * 20) * 1.0 / 100;

    if(Total >= 8.5) GPA = 'A';
    else {
        if(Total >= 7.5) GPA = 'B';
        else {
            if(Total >= 6.5) GPA = 'C';
            else {
                if(Total >= 5) GPA = 'D';
                else {
                    GPA = 'E';
                }
            }
        }
    }
}
