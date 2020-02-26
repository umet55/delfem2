git submodule update --init --recursive

: ################################
: test cpp

cd 3rd_party/googletest
mkdir buildVS64
cd buildVS64
cmake -A x64 -Dgtest_force_shared_crt=ON .. 
cmake --build . --config Release
cd ../../..

cd 3rd_party/googletest
mkdir buildVS32
cd buildVS32
cmake -A Win32 -Dgtest_force_shared_crt=ON .. 
cmake --build . --config Release
cd ../../..

cd test_cpp
mkdir buildVS64
cd buildVS64
cmake -A x64 ..
cmake --build . --config Release
"Release/runUnitTests.exe"
cd ../../

cd test_cpp
mkdir buildVS32
cd buildVS32
cmake -A Win32 ..
cmake --build . --config Release
"Release/runUnitTests.exe"
cd ../../

goto :eof

: ################################
: glfw

cd 3rd_party/glfw
cmake -A x64 .
cmake --build . --config Release
cd ../..



cd src_cpp/external/glfw
mkdir buildVS64
cd buildVS64
cmake -A x64 ..
cmake --build . --config Release
rem cmake --build . --config Debug
cd ..
mkdir buildVS32
cd buildVS32
cmake -A Win32 ..
cmake --build . --config Release
rem cmake --build . --config Debug
cd ../
cd ../../../


: ##############################
: glfw_oldgl

cd examples_glfw_oldgl
mkdir buildVS32
cd buildVS32
cmake -A Win32 ..
cmake --build . --config Release
cd ../../

cd examples_glfw_oldgl
mkdir buildVS64
cd buildVS64
cmake -A x64 ..
cmake --build . --config Release
cd ../../


: ##############################
: glfw_newgl

cd examples_glfw
mkdir buildVS32
cd buildVS32
cmake -A Win32 ..
cmake --build . --config Release
cd ../../

cd examples_glfw
mkdir buildVS64
cd buildVS64
cmake -A x64 ..
cmake --build . --config Release
cd ../../



: ###############################
: pybind

cd src_pybind/core
mkdir buildVS64
cd buildVS64
cmake -A x64 ..
cmake --build . --config Release
cd ../../../

cd src_pybind/gl
mkdir buildVS64
cd buildVS64
cmake -A x64 ..
cmake --build . --config Release
cd ../../../



: cd examples_cpp
: mkdir buildVS64
: cd buildVS64
: cmake -A x64 -DCMAKE_PREFIX_PATH="C:/Program Files/glew;C:/Program Files/freeglut" ..
: cmake --build .
: cd ../../


: pip uninstall PyDelFEM2 -y
: pip uninstall PyDelFEM2 -y
: python setup.py install
: python setup.py test


