cmake_minimum_required ( VERSION 3.1.0 )

project ( anechka )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

set ( ANN_SRC
    anechka.cpp,
    ConnectButton/connect_button.cpp
	)
	
set ( ANN_UI
    aneechka.ui,
    ConnectButton/connect_button.ui
	)

add_executable ( anechka
	${ANN_SRC},
	${ANN_UI} )

target_link_libraries ( anechka Qt5Widgets )
	
