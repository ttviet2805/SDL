#pragma once

#include <string.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <windows.h>

using namespace std;

#include "Student.h"
#include "Course.h"

struct StudentInClass {
    string StudentID;
    StudentInClass* Next = nullptr;
};

struct Class {
    string className;
    StudentInClass* studentHead = nullptr;

    Class* Next = nullptr;

    void loadStudentInClass();
};

void loadAllClassData(Class* &allClass, string fileName);

void saveAllClassData(Class* allClass, string fileName);

void showAllClassData(Class* allClass);

Class* findClassByID(Class* allClass, string ID);
