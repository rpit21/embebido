cmake_minimum_required(VERSION 3.0.2)
project(plataforma_mov_pkg)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  geometry_msgs
  sensor_msgs
)

## System dependencies are found with CMake's conventions
find_library(wiringpi_libraries names wiringpi)


###################################
## catkin specific configuration ##
###################################

catkin_package(
  CATKIN_DEPENDS roscpp rospy geometry_msgs sensor_msgs
)


###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${WIRINGPI_INCLUDE_DIRS}
)



#############
## Install ##
#############


#Topic publisher
add_executable(encoder src/encoder.cpp)
add_dependencies(encoder ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(encoder ${catkin_LIBRARIES} -lwiringPi)

add_executable(joy_car src/joy_teleop.cpp)
add_dependencies(joy_car ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(joy_car ${catkin_LIBRARIES} -lwiringPi)

add_executable(motors src/Motors.cpp)
add_dependencies(motors ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(motors ${catkin_LIBRARIES} -lwiringPi)

add_executable(mostrar src/mostrar.cpp)
add_dependencies(mostrar ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(mostrar ${catkin_LIBRARIES} -lwiringPi)

add_executable(pose_kinematics src/pose_kinematics.cpp)
add_dependencies(pose_kinematics ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(pose_kinematics ${catkin_LIBRARIES} -lwiringPi)

add_executable(link src/pid_link.cpp)
add_dependencies(link ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(link ${catkin_LIBRARIES} -lwiringPi)

add_executable(pos_control src/pos_control.cpp)
add_dependencies(pos_control ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(pos_control ${catkin_LIBRARIES} -lwiringPi)


add_executable(vel_setpoint src/vel_setpoint.cpp)
add_dependencies(vel_setpoint ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(vel_setpoint ${catkin_LIBRARIES} -lwiringPi)



#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_plataforma_mov_pkg.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
