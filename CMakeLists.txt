cmake_minimum_required(VERSION 3.16)
project(piqi VERSION 0.1)

set(QT6_MIN_VERSION 6.9.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
remove_definitions(-DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII)

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Qml Network)
find_package(QCoro6 REQUIRED COMPONENTS Core Network DBus Qml)

qt_policy(SET QTP0001 NEW)
qcoro_enable_coroutines()

add_library(piqi SHARED
    piqi.cpp
    illusts.h
    illustration.h
    imageurls.h
    series.h
    tag.h
    user.h
    qepr.h
    privacypolicy.h
    comments.h
    comment.h
    stamp.h
    bookmarkdetails.h
    searchrequest.h
)

set_target_properties(piqi PROPERTIES AUTOMOC ON)
set_target_properties(piqi PROPERTIES AUTORCC ON)
set_target_properties(piqi PROPERTIES AUTOUIC ON)

qt_add_qml_module(piqi
    # SHARED
    URI org.kde.piqi
    VERSION 1.0
    # OUTPUT_DIRECTORY
        # "${CMAKE_BINARY_DIR}/org/kde/piqi"
)
add_compile_definitions(QCORO_NO_WARN_DEPRECATED_TASK_H)
target_compile_options (piqi PUBLIC -fexceptions)

target_link_libraries(piqi PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::Network
    QCoro::Core
    QCoro::Network
    QCoro::DBus
    QCoro::Qml
)
