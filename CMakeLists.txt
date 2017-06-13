set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
project(simulation)
add_executable(simulation main.cpp geometry.cpp Model.cpp View.cpp Controller.cpp navigation.cpp Island.cpp Ship.cpp Tanker.cpp Cruiser.cpp Ship_factory.cpp track_base.cpp utility.cpp)

add_subdirectory(test)

add_executable(test_exception test/test_exception.cpp)
add_executable(test_view test/test_view.cpp)
