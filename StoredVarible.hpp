/*
    Author: Nikolay Zakirov
    Email: nikolayzakirov31@gmail.com
    Date Created: 2024-06-20
    Last Modified: 2024-06-20

    Description: StoredVariable.hpp
    Uses a local variable across launches
*/
#ifndef STOREDVARIABLE
#define STOREDVARIABLE

// Dependencies
#include <Windows.h>
#include <string>
#include <vector>

int readStoredInteger(); // reads a stored integer (defaults to 0 on first read)
void updateStoredInteger(int newValue); // stores the int
std::string readStoredString(); // reads a stored string (defaults to "" on first read)
void updateStoredString(const std::string& newValue); // stores the string

#endif