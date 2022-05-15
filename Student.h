#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

#include "Const.h"
#include "Course.h";
#include "Score.h"
#include "Class.h"
#include "SDLProcess.h"
#include "ButtonClass.h"
#include "Login.h"
#include "SystemProcess.h"

struct StudentInfo {
    string ID = "", firstName = "", lastName = "", Gender = "", Dob = "", SocialID = "", Class = "", schoolyear = "";
};

struct StudentScore {
    string courseID = "";
    Score studentScore;

    StudentScore* Next = nullptr;
};

struct Student {
    StudentInfo Info;
    StudentScore* Score = nullptr;

    Student* Next = nullptr;

    void addStudentScore(StudentScore* newScore);

    void viewProfile();

    void editProfile();

    void viewScoreBoard();

    void exportScoreBoard(string fileName);

    void viewStudentInCurrentClass();

//    StudentScore* findCourseScore(string ID);
};

// Back End
void loadAllStudentData(Student* &allStudent, string fileName);

void saveAllStudentData(Student* allStudent, string FileName);

void showAllStudentInfo(Student* allStudent);

Student* findStudentByID(Student* allStudent, string ID);

StudentScore* createAStudentScore(string CourseID, float MidTerm, float Final, float Other);

//-----------------------------------------------------------------------//

//Front End

void studentEditProfileWindow(Student* curStudent);

void studentViewScore(Student* curStudent);

void studentViewStudentInClass(Student* curStudent);

void studentWindow(Student* curStudent);
