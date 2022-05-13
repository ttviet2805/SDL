#include "Score.h"

void Score::setScore(float __MidTerm, float __Final, float __Other) {
    MidTerm = __MidTerm, Final = __Final, Other = __Other;
}

void Score::calTotal() {
    Total = (MidTerm * 30 + Final * 50 + Other * 20) * 1.0 / 100;
}
