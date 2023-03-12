#
cmake_minimum_required (VERSION 3.8)


#
project ("as-smpp-esme")


#
add_library (${PROJECT_NAME} 
	src/esme.c
)


#
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
