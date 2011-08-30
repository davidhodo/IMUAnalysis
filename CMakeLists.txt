
###########################################################
#   Top level CMakeLists.txt file for the GAVLab gMOOS 
#   Project. This builds the common library and sensor apps
###########################################################


CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(IMUAnalysis)

##########################################################
# No clue what this does. Got it from MOOS
##########################################################

IF(CMAKE_COMPILER_IS_GNUCXX)
    SET( WALL_ON OFF CACHE BOOL "tell me about all compiler warnings (-Wall) ")
    IF(WALL_ON)
	    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    ENDIF( WALL_ON)
ELSE(CMAKE_COMPILER_IS_GNUCXX)
    IF(MSVC)
	    SET( WALL_ON OFF CACHE BOOL "tell me about all compiler warnings (-Wall) ")
	    IF(WALL_ON)
	        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
		ENDIF(WALL_ON)
	ELSE(MSVC)
        # Other compilers go here	
	
	ENDIF(MSVC)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


#####################################################
# Define preprocessor definitions
IF(WIN32)
    ADD_DEFINITIONS(-DWINDOWS_NT -D_CRT_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_DEPRECATE -D_WIN32_WINNT=0x0501)
ENDIF(WIN32)
IF(UNIX)
    ADD_DEFINITIONS(-DUNIX)
	IF("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
		ADD_DEFINITIONS( -DPLATFORM_DARWIN )
		SET( PLATFORM_DARWIN 1 ) 
	ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
	IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
		ADD_DEFINITIONS( -DPLATFORM_LINUX )
		SET( PLATFORM_LINUX 1 ) 
	ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
ENDIF(UNIX)


SET( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE PATH
     "Output directory for the gMOOS libraries" )
SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE PATH
     "Output directory for the gMOOS executables" )

# set directory to install built files to
IF(WIN32)
	SET(CMAKE_INSTALL_PREFIX "C:/MOOS/MOOSBin" CACHE STRING "Path to install MOOS binaries to")
ENDIF(WIN32)
IF(UNIX)
	SET(CMAKE_INSTALL_PREFIX "/MOOS/MOOSBin" CACHE STRING "Path to install MOOS binaries to")
ENDIF(UNIX)

###############################################################################
# Tell CMake where to look for find moduleXYZ files
SET( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules" )

#what files are needed?
SET(IMUANALYSIS_SRCS main.cpp allanVariance.cpp allanVariance.h 
   ${RESOURCES})

# Add executable called gUSARSim
ADD_EXECUTABLE( allanvariance ${IMUANALYSIS_SRCS})

INSTALL(TARGETS
allanvariance
RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
)


