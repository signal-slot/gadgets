cmake_minimum_required(VERSION 3.16)

project(gadgets LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Test)
find_package(Qt6 REQUIRED COMPONENTS Core Test)

add_executable(gadgets
  main.cpp
  abstractgadget.h
  abstractgadget.cpp
)

target_link_libraries(gadgets Qt6::Core Qt6::Test)

include(GNUInstallDirs)
install(TARGETS gadgets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
