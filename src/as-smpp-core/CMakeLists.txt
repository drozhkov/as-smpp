#
cmake_minimum_required (VERSION 3.8)


#
project ("as-smpp-core")


#
add_library (${PROJECT_NAME} 
	src/net.c
	src/session.c
	src/threads.c
	src/core.c
	src/smpp.c
	src/serializer.c
	src/converter.c
	src/logger.c
)


#
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
