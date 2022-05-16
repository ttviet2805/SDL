#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;

#include "Const.h"
#include "Score.h"

struct CourseInfo {
    string courseID = "", courseName = "";
};

#include "Student.h"
struct Student;

struct CourseScore {
    string StudentID = "";
    Score studentScore;

    CourseScore* Next = nullptr;
};

struct Course {
    CourseInfo Info;
    CourseScore* courseScoreHead = nullptr;

    Course* Next = nullptr;

    void loadCourseScoreData(string fileName, Student* &allStudent);

    void saveCourseScoreData(string fileName);

    void addCourseScore(CourseScore* newScore);

    void viewStudentScore();
};

void loadAllCourseData(Course* &allCourse, string fileName, Student* &allStudent);

void saveAllCourseData(Course* allCourse, string fileName);

void showAllCourseData(Course* allCourse);

bool addCourseScoreByCSV(string courseID, string fileName);

Course* findCourseByID(Course* allCourse, string ID);

CourseScore* createACourseScore(string studentID, float Midterm, float Final, float Other);
