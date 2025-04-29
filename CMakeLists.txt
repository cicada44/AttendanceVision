cmake_minimum_required(VERSION 3.17)

project(AttendanceVision)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# OpenCV
find_package(OpenCV REQUIRED)

set(SrcDir src)

add_executable(${PROJECT_NAME}
    ${SrcDir}/main.cpp

    # MainWindow
    ${SrcDir}/MainWindow/MainWindow.cpp
    ${SrcDir}/MainWindow/MainWindow.h

    # View
    ${SrcDir}/View/CamerasTab.cpp
    ${SrcDir}/View/CamerasTab.h
    ${SrcDir}/View/CameraDialog.cpp
    ${SrcDir}/View/CameraDialog.h

    # Controller
    ${SrcDir}/Controller/CameraController.cpp
    ${SrcDir}/Controller/CameraController.h

    # Model
    ${SrcDir}/Model/CameraManager.cpp
    ${SrcDir}/Model/CameraManager.h

    # Custom Widgets
    ${SrcDir}/CustomWidgets/CameraLabel.cpp
    ${SrcDir}/CustomWidgets/CameraLabel.h
    ${SrcDir}/CustomWidgets/VTabWidget.cpp
    ${SrcDir}/CustomWidgets/VTabWidget.h

    # General
    ${SrcDir}/General/General.h

    # Resources (если есть .qrc файл ресурсов)
    resources.qrc
)

# Подключаем библиотеки
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Указываем пути для #include
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
