cmake_minimum_required(VERSION 3.17)

project(AttendanceVision)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Network)
# OpenCV
find_package(OpenCV REQUIRED)

set(SrcDir src)

add_executable(${PROJECT_NAME}
    # entry point
    ${SrcDir}/main.cpp

    # MainWindow
    ${SrcDir}/MainWindow/MainWindow.h
    ${SrcDir}/MainWindow/MainWindow.cpp

    # Views
    ${SrcDir}/View/CamerasTab.h
    ${SrcDir}/View/CamerasTab.cpp
    ${SrcDir}/View/CameraDialog.h
    ${SrcDir}/View/CameraDialog.cpp
    ${SrcDir}/View/DaemonTab.h
    ${SrcDir}/View/DaemonTab.cpp
    ${SrcDir}/View/ScheduleTab.h
    ${SrcDir}/View/ScheduleTab.cpp
    ${SrcDir}/View/StatisticsTab.h
    ${SrcDir}/View/StatisticsTab.cpp

    # Controllers
    ${SrcDir}/Controller/CameraController.h
    ${SrcDir}/Controller/CameraController.cpp
    ${SrcDir}/Controller/DaemonController.h
    ${SrcDir}/Controller/DaemonController.cpp
    ${SrcDir}/Controller/ScheduleController.h
    ${SrcDir}/Controller/ScheduleController.cpp
    ${SrcDir}/Controller/StatisticsController.h
    ${SrcDir}/Controller/StatisticsController.cpp

    # Models
    ${SrcDir}/Model/CameraManager.h
    ${SrcDir}/Model/CameraManager.cpp
    ${SrcDir}/Model/ScheduleModel.h
    ${SrcDir}/Model/ScheduleModel.cpp
    ${SrcDir}/Model/StatisticsModel.h
    ${SrcDir}/Model/StatisticsModel.cpp
    ${SrcDir}/Model/AttendanceAnalyzer.h
    ${SrcDir}/Model/AttendanceAnalyzer.cpp
    ${SrcDir}/Model/StatsRepository.h
    ${SrcDir}/Model/StatsRepository.cpp

    # Custom Widgets
    ${SrcDir}/CustomWidgets/CameraLabel.h
    ${SrcDir}/CustomWidgets/CameraLabel.cpp
    ${SrcDir}/CustomWidgets/VTabWidget.h
    ${SrcDir}/CustomWidgets/VTabWidget.cpp

    # General constants
    ${SrcDir}/General/General.h

    # Resources
    resources.qrc
)

# link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Widgets
        Qt6::Sql
        Qt6::Network
        ${OpenCV_LIBS}
)

# include paths
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${SrcDir}
        ${SrcDir}/MainWindow
        ${SrcDir}/View
        ${SrcDir}/Controller
        ${SrcDir}/Model
        ${SrcDir}/CustomWidgets
        ${SrcDir}/General
)

file(COPY ${CMAKE_SOURCE_DIR}/data/attendance.db DESTINATION ${CMAKE_BINARY_DIR}/data)
