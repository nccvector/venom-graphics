cmake_minimum_required(VERSION 3.0)
project(mugen)

# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED)

# Add pybind 11
add_subdirectory(pybind11)

# Adding sdl2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# # add the source files as usual
# # add_executable(${PROJECT_NAME} OgreApp.cpp)
# add_executable(${PROJECT_NAME} OgreApp.cpp)
# target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES})

# add the source files as usual
# add_executable(${PROJECT_NAME} OgreApp.cpp)
add_library(${PROJECT_NAME} STATIC OgreApp.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)


# this also sets the includes and pulls third party dependencies
target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES} ${SDL2_LIBRARIES})

# Building python bindings
pybind11_add_module(PyVenom pythonbinding.cpp)
target_link_libraries(PyVenom PRIVATE ${PROJECT_NAME})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/main.py
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})