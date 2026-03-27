fp_overflow_checker
Zachary Terry
CS3339 Spring 2026 — HW 2: Binary Representation, Floating-point Math and Bit Manipulation

Description:
I created a program that will automatically detect floating-point overflow in C++
programs.Given a loop bound and a loop counter increment we determine whether the IEEE 754 single-precision floating-point representation has insufficient bits
We also checked whether there was overflow or not. We do this by computing the minimum threshold for when percision is lost. 

How to build and run this program:

If you are on LInux/Mac you would type 
g++ -o fp_overflow_checker fp_overflow_checker.cpp -std=c++17
If you are on Windows you have to type 
g++ -o fp_overflow_checker fp_overflow_checker.cpp -std=c++17 -static-libgcc -static-libstdc++

Then to actuall use the program you would run this and then your values 
./fp_overflow_checker 1e+08 1.0 the values can be whatever you want to test

Limitations:
The limiations that I have considered would be 
That it cant use negative numbers 
We are asssuming 32 bit single percision 
