#
cmake_minimum_required (VERSION 3.8)


#
project ("as-smpp-demo")


#
##
set(LIBS
	as-smpp-esme
	as-smpp-core
)

##
link_directories(
)

set(LIBS
	${LIBS}
)

##
if(NOT WIN32)
	set(LIBS
		${LIBS}
		pthread
	)
endif()

##
if(WIN32)
	set(LIBS
		${LIBS}
		ws2_32.lib
	)
endif()


#
add_executable(${PROJECT_NAME} 
	_as-smpp-demo.c
)


#
target_link_libraries(${PROJECT_NAME} ${LIBS})


#
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)


#
install(TARGETS ${PROJECT_NAME} DESTINATION ./bin)
