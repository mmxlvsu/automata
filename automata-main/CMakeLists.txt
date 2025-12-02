cmake_minimum_required(VERSION 3.16)
project(automata)

set(CMAKE_CXX_STANDARD 17)

# Qt install path (change if needed)
set(CMAKE_PREFIX_PATH "D:\\Qt\\6.10.1\\mingw_64\\lib\\cmake\\Qt6")

# Find Qt packages
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Enable AUTOMOC and AUTOUIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Add executable
add_executable(MyQtApp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    ProjectOverviewTab.cpp
    ProjectOverviewTab.h
    SyntaxAnalysisTab.cpp
    SyntaxAnalysisTab.h
    AssignmentParser.h
    DelimiterParser.h
    OperationParser.h
    AssignmentParser.cpp
    DelimiterParser.cpp
    OperationParser.cpp
    NFASimulatorTab.cpp 
    NFASimulatorTab.h
    LexicalAnalysis.cpp
    LexicalAnalysis.h
)

# Link Qt libraries
target_link_libraries(MyQtApp Qt6::Core Qt6::Widgets)
