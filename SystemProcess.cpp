#include "systemProcess.h"

void systemProcess() {
//    Staff* allStaff = nullptr;
//    loadAllStaffData(allStaff, staffFileName);

//    staffEditProfile(allStaff);
////
//Class* allClass = nullptr;
//loadAllClassData(allClass, classFileName);
//
//    staffViewStudentInAClass(allClass, 0);

    staffViewStudentInASameYear("2021", 0);
//
    exit(0);

    Account* curAccount = loginWindow();

    if(curAccount == nullptr) {
        return;
    }

    if(curAccount->type == 0) {
        Staff* allStaff = nullptr;
        loadAllStaffData(allStaff, staffFileName);

        Staff* curStaff = findStaffByID(allStaff, curAccount->username);

        if(curStaff)
            staffWindow(curStaff);
    }
    else {
        Student* allStudent = nullptr;
        loadAllStudentData(allStudent, studentFileName);

        Student* curStudent = findStudentByID(allStudent, curAccount->username);

        studentWindow(curStudent);
    }
}
