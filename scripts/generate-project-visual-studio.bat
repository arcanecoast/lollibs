mkdir ..\build
cd ..\build
cmake -G "Visual Studio 12" -D CXXTEST_INCLUDE_DIR=E:\Projects\Libs\Trunk\C++\CxxTest -D CXXTEST_PYTHON_TESTGEN_EXECUTABLE=E:\Projects\Libs\Trunk\C++\CxxTest\bin\cxxtestgen ..
cd ..