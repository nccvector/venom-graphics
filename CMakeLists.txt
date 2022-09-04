cmake_minimum_required(VERSION 3.0)
project(mugen)

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED)

# Add pybind 11
add_subdirectory(pybind11)

# # add the source files as usual
# # add_executable(${PROJECT_NAME} OgreApp.cpp)
# add_executable(${PROJECT_NAME} OgreApp.cpp)
# target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES})

# add the source files as usual
# add_executable(${PROJECT_NAME} OgreApp.cpp)
add_library(${PROJECT_NAME} STATIC OgreApp.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)


# this also sets the includes and pulls third party dependencies
target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES})

# Building python bindings
pybind11_add_module(pymugen pythonbinding.cpp)
target_link_libraries(pymugen PRIVATE ${PROJECT_NAME})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/main.py
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})