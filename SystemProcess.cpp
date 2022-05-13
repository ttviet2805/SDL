#include "systemProcess.h"

void systemProcess() {
//    Account* curAccount = loginWindow();
//
//    if(curAccount == nullptr) {
//        return;
//    }

//    if(curAccount->type == 0) {
//
//    }
//    else {
        Student* allStudent = nullptr;
        loadAllStudentData(allStudent, studentFileName);

//        Student* curStudent = findStudentByID(allStudent, curAccount->username);

//        studentWindow(allStudent);

        studentEditProfileWindow(allStudent);
//    }
}
