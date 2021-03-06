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

struct Course;

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

void exportStudentInASameYear(string schoolYear, string fileName);

bool addStudentByCSV(string fileName);

//---------------------------------------------------------------------------------//

// Front End
void staffEditProfile(Staff* curStaff);

void staffImportDataWindow(int Type, string courseID);

void staffaddStudentByManual();

void staffaddScoreByManual(Course* curCourse);

void staffViewAllCourse();

void staffViewStudentInAClass(Class* curClass, int page);

void staffViewStudentInASameYear(string schoolYear, int page);

void staffViewCourseScore(Course* tmpCourse);

void staffWindow(Staff* tmpStaff);

void staffViewStudentWindow();

void staffViewListClassWindow();

void staffEnterSchoolYearWindow();

void staffChooseOptionWindow(int Type, string courseID);

void staffChooseScoreOption();

void staffFindStudentWindow(int Type);
