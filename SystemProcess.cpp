#include "systemProcess.h"

void systemProcess() {
    Student* allStudent = nullptr;
    loadAllStudentData(allStudent, studentFileName);

    Course* allCourse = nullptr;
    loadAllCourseData(allCourse, courseFileName, allStudent);

    Class* allClass = nullptr;
    loadAllClassData(allClass, classFileName);

    staffaddStudentByManual();
    exit(0);
    Account* curAccount = loginWindow();

    if(curAccount == nullptr) {
        return;
    }

    if(curAccount->type == 0) {

    }
    else {
        Student* allStudent = nullptr;
        loadAllStudentData(allStudent, studentFileName);

        Student* curStudent = findStudentByID(allStudent, curAccount->username);

        studentWindow(allStudent);
    }
}
