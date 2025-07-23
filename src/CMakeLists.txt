qt_policy(SET QTP0001 NEW)
qcoro_enable_coroutines()
remove_definitions(-DQT_NO_CAST_TO_ASCII -DQT_NO_CAST_FROM_ASCII)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

qt_add_library(piqi SHARED
    bookmarkdetails.cpp
    comment.cpp
    comments.cpp
    illustration.cpp
    illusts.cpp
    imageurls.cpp
    novel.cpp
    novels.cpp
    piqi.cpp
    privacypolicy.cpp
    profile.cpp
    qepr.h
    searchrequest.cpp
    series.cpp
    stamp.cpp
    tag.cpp
    tags.cpp
    user.cpp
    userdetails.cpp
    work.cpp
    workspace.cpp
)

add_library(Piqi::piqi ALIAS piqi)
ecm_add_qml_module(piqi
    GENERATE_PLUGIN_SOURCE
    URI io.github.micro.piqi
    VERSION 1.0
)
add_compile_definitions(QCORO_NO_WARN_DEPRECATED_TASK_H)
target_compile_options (piqi PUBLIC -fexceptions)

ecm_generate_headers(piqi_HEADERS
    HEADER_NAMES
        BookmarkDetails
        Comment
        Comments
        Illustration
        Illusts
        ImageUrls
        Novel
        Novels
        Piqi
        PrivacyPolicy
        Profile
        QEPR
        SearchRequest
        Series
        Stamp
        Tag
        Tags
        User
        UserDetails
        Work
        Workspace
    REQUIRED_HEADERS
        piqi_HEADERS
)

ecm_generate_export_header(piqi
    BASE_NAME Piqi
    VERSION ${PROJECT_VERSION}
    # USE_VERSION_HEADER
    # DEPRECATED_BASE_VERSION 0
    # DEPRECATION_VERSIONS
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

install(FILES
    ${piqi_HEADERS}
    DESTINATION "${KDE_INSTALL_INCLUDEDIR}/piqi"
    COMPONENT Devel
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/piqi_export.h
    ${piqi_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/piqi
    COMPONENT Devel)

ecm_finalize_qml_module(piqi DESTINATION ${KDE_INSTALL_QMLDIR})

target_link_libraries(piqi PUBLIC
    Qt6::Core
    Qt6::Qml
    Qt6::Network
    QCoro::Core
    QCoro::Network
    QCoro::DBus
    QCoro::Qml
)

install(TARGETS piqi EXPORT piqi-targets)
install(
  EXPORT piqi-targets
  NAMESPACE Piqi::
  FILE PiqiTargets.cmake
  DESTINATION "${PIQI_INSTALL_CMAKEDIR}"
)

export(
  EXPORT piqi-targets
  NAMESPACE Piqi::
  FILE "${PIQI_INSTALL_CMAKEDIR}/PiqiTargets.cmake"
)
