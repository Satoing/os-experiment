@REM g++ -S thread_student.cpp
@REM as -o thread_student.o thread_student.s

@REM g++ -S thread_tester.cpp
@REM as -o thread_tester.o thread_tester.s

@REM g++ -o thread_tester.o thread_student.o
@REM delete *.o
@REM delete *.s

g++ thread_tester.cpp
a.exe
del a.exe