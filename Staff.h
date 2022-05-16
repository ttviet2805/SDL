#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;

#include "Class.h"
#include "Student.h"
#include "Const.h"
#include "Course.h";
#include "Score.h"
#include "Login.h"
#include "SystemProcess.h"

struct Student;

struct Class;

struct StaffInfo {
    string ID = "", fullName = "";
};

struct Staff {
    StaffInfo Info;

    Staff* Next = nullptr;
};

Staff* findStaffByID(Staff* allStaff, string StaffID);

// Back End
void loadAllStaffData(Staff* &allStaff, string fileName);

void saveAllStaffData(Staff* allStaff, string fileName);

void showAllStaffData(Staff* allStaff);

void addANewStudent(Student* &allStudent, Student* newStudent);

void deleteAStudent(Student* &allStudent, Student* delStudent);

void viewStudentInAClass(Class* curClass);

void exportStudentInAClass(Class* curClass, string fileName);

void viewStudentInASameYear(string schoolYear);

void addStudentByCSV(string fileName);

//---------------------------------------------------------------------------------//

// Front End
void staffEditProfile(Staff* curStaff);

void staffaddStudentByManual();

void staffViewStudentInAClass(Class* curClass);

void staffWindow(Staff* tmpStaff);
