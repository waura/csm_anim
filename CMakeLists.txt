cmake_minimum_required(VERSION 2.8)

project(csm_anim)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/nmc/include")
include_directories("${PROJECT_SOURCE_DIR}/SkinMeshGL/include")

add_executable(Main
  Csm.cpp
  CsmAnimApp.cpp
  CsmAnimGL.cpp
  CsmAreaCtrlPanel.cpp
  CsmCassiniArea.cpp
  CsmCassinixArea.cpp
  CsmCassiniyArea.cpp
  CsmCircleArea.cpp
  CsmCtrlPanel.cpp
  CsmEllipseArea.cpp
  GLCanvas.cpp
  TopFrame.cpp
  bessel.cpp
)

# Setting for wxWidgets
find_package(wxWidgets REQUIRED gl core base)
include(${wxWidgets_USE_FILE})
target_link_libraries(Main ${wxWidgets_LIBRARIES})

# Setting for OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIRS})
target_link_libraries(Main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

# Setting for libnmc
add_subdirectory(nmc)
target_link_libraries(Main libnmc)

# Setting for SkinMeshGL
add_subdirectory(SkinMeshGL)
target_link_libraries(Main libSkinMeshGL)
