#include "systemProcess.h"

void systemProcess() {
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
