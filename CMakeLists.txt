cmake_minimum_required(VERSION 2.8)

project(mouthState)

find_package(OpenCV 3.4 REQUIRED PATHS "/home/summer/opencv3.4.0/install")
add_subdirectory(/home/summer/DLIB/dlib-19.13/dlib dlib_build)

include_directories(${OpenCV_INCLUDE_DIRS})
# Create executable with all necessary source files
#add_executable(${PROJECT_NAME}
#  ${project_sources} )
add_executable(${PROJECT_NAME} "main.cpp" "FaceDetector.cpp" "MouthTracker.cpp"
                            "MouthStateClassifier.cpp" "MouthStateDetector.cpp")
target_link_libraries( mouthState ${OpenCV_LIBS} dlib::dlib)
