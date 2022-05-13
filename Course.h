#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;

#include "Score.h"

struct CourseInfo {
    string courseID, courseName;
};

#include "Student.h"
struct Student;

struct CourseScore {
    string StudentID;
    Score* studentScore = new Score;

    CourseScore* Next = nullptr;
};

struct Course {
    CourseInfo* Info = new CourseInfo;
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

Course* findCourseByID(Course* allCourse, string ID);

CourseScore* createACourseScore(string studentID, float Midterm, float Final, float Other);
