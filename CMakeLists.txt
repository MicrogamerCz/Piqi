cmake_minimum_required(VERSION 3.25)
project(piqi VERSION 0.1.0)

set(KF6_MIN_VERSION 6.13.0)
set(QT6_MIN_VERSION 6.9.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Core Qml Network)
find_package(QCoro6 REQUIRED COMPONENTS Core Network DBus Qml)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(FeatureSummary)
include(ECMQmlModule)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMGenerateExportHeader)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)

set(PIQI_INSTALL_CMAKEDIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/Piqi"
    CACHE STRING "Install path for example CMake package files")

add_subdirectory(src)

configure_package_config_file(
  PiqiConfig.cmake.in "${PIQI_INSTALL_CMAKEDIR}/PiqiConfig.cmake"
  INSTALL_DESTINATION "${PIQI_INSTALL_CMAKEDIR}"
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
  "${PIQI_INSTALL_CMAKEDIR}/PiqiConfigVersion.cmake"
  VERSION ${example_VERSION}
  COMPATIBILITY SameMajorVersion
)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PIQI_INSTALL_CMAKEDIR}/PiqiConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PIQI_INSTALL_CMAKEDIR}/PiqiConfigVersion.cmake"
  DESTINATION "${PIQI_INSTALL_CMAKEDIR}"
)
