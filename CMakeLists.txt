cmake_minimum_required(VERSION 3.16)
project(piqi VERSION 0.1)

set(KF6_MIN_VERSION 6.13.0)
set(QT6_MIN_VERSION 6.9.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Qml Network)
find_package(QCoro6 REQUIRED COMPONENTS Core Network DBus Qml)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

# qt_standard_project_setup(REQUIRES 6.9)
qt_policy(SET QTP0001 NEW)
qcoro_enable_coroutines()

include(FeatureSummary)
include(ECMQmlModule)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMGenerateExportHeader)

remove_definitions(-DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# qt_add_library(piqi STATIC)
ecm_add_qml_module(piqi
    GENERATE_PLUGIN_SOURCE
    URI io.github.micro.piqi
    VERSION 1.0
    SOURCES
        illustration.cpp
        user.cpp
        imageurls.cpp
        series.cpp
        tag.cpp
        illusts.cpp
        profile.cpp
        userdetails.cpp
        workspace.cpp

        piqi.cpp

        bookmarkdetails.h
        illusts.h
        qepr.h
        privacypolicy.h
        comments.h
        comment.h
        stamp.h
        searchrequest.h
    # OUTPUT_DIRECTORY
        # "${CMAKE_BINARY_DIR}/io/github/micro/piqi"
)
add_library(Piqi::piqi ALIAS piqi)
add_compile_definitions(QCORO_NO_WARN_DEPRECATED_TASK_H)
target_compile_options (piqi PUBLIC -fexceptions)

ecm_generate_headers(piqi_HEADERS
    HEADER_NAMES
        Illustration
        User
        ImageUrls
        Series
        Tag
        Illusts
        Profile
        UserDetails
        Workspace
        Piqi
        BookmarkDetails
        Illusts
        PrivacyPolicy
        Comments
        Comment
        Stamp
        SearchRequest
    REQUIRED_HEADERS
        piqi_HEADERS
)

install(FILES
    ${piqi_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/piqi"
    COMPONENT Devel
)


target_link_libraries(piqi PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::Network
    QCoro::Core
    QCoro::Network
    QCoro::DBus
    QCoro::Qml
)

ecm_finalize_qml_module(piqi DESTINATION ${KDE_INSTALL_QMLDIR})
