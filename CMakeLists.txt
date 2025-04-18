cmake_minimum_required(VERSION 3.17)

project(Attendance)

set(CMAKE_AUTOMOC ON)

set(project_name Attendance)

set(SrcDir src)
set(MainWindowDir src/MainWindow)
set(CamerasTabDir src/CamerasTab)
set(AttendanceTabDir src/AttendanceTab)
set(CustomWidgetsDir src/CustomWidgets)

add_executable(
    ${project_name}
        ${SrcDir}/main.cpp
        ${MainWindowDir}/MainWindow.cpp
        ${MainWindowDir}/MainWindow.h
        ${CamerasTabDir}/CamerasTab.cpp
        ${CamerasTabDir}/CamerasTab.h
        ${AttendanceTabDir}/AttendanceTab.cpp
        ${AttendanceTabDir}/AttendanceTab.h
        ${CustomWidgetsDir}/VTabWidget.cpp
        ${CustomWidgetsDir}/VTabWidget.h
        ${CustomWidgetsDir}/CameraLabel.cpp
        ${CustomWidgetsDir}/CameraLabel.h
        ${SrcDir}/General/General.h
        resources.qrc
)

# qt_add_qml_module(Attendance "resources" resources.qrc)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

find_package(OpenCV REQUIRED)
target_link_libraries(${project_name} PRIVATE ${OpenCV_LIBS})

target_link_libraries(${project_name} PRIVATE Qt6::Widgets)

target_include_directories(
    ${project_name} PUBLIC
    MainWindow
)
