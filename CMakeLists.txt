cmake_minimum_required(VERSION 3.5)
project(vision_app) # Project Name Here

find_package(wpilib REQUIRED)

add_executable(my_vision_app src/cpp/robot.cpp) # exectuable name as first parameter
target_link_libraries(my_vision_app cameraserver ntcore cscore wpiutil)
