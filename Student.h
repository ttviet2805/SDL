#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;

#include "Course.h";
#include "Score.h"
#include "Class.h"

struct StudentInfo {
    string ID, firstName, lastName, Gender, Dob, SocialID, Class, schoolyear;
};

struct StudentScore {
    string courseID;
    Score* studentScore = new Score;

    StudentScore* Next = nullptr;
};

struct Student {
    StudentInfo* Info = new StudentInfo;
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

void loadAllStudentData(Student* &allStudent, string fileName);

void saveAllStudentData(Student* allStudent, string FileName);

void showAllStudentInfo(Student* allStudent);

Student* findStudentByID(Student* allStudent, string ID);

StudentScore* createAStudentScore(string CourseID, float MidTerm, float Final, float Other);
