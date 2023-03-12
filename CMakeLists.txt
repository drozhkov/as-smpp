#
cmake_minimum_required(VERSION 3.8)


#
project("as-smpp")

if (MSVC)
	# add_compile_options(-D_WIN32_WINNT=0x0601)
endif()


#
##

##

##
include_directories("include")


#
add_subdirectory("src/as-smpp-core")
add_subdirectory("src/as-smpp-esme")

add_subdirectory("src/_as-smpp-demo")
