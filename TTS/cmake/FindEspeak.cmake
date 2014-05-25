INCLUDE( LibFindMacros )
# Unix style platforms

FIND_PATH(Espeak_INCLUDE_DIR
	NAMES
                espeak/speak_lib.h
	PATHS
		#/home/brunner/Iason/TTS/include
                #/usr/include/espeak

)

# AMD Stream SDK, (should be updated to APPSDK) default location
FIND_LIBRARY(Espeak_LIBRARY 
		espeak
	PATHS
		#/home/brunner/espeak-1.48.04-source/src/
                #/usr/lib/x86_64-linux-gnu/
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set( Espeak_PROCESS_INCLUDES Espeak_INCLUDE_DIR )
set( Espeak_PROCESS_LIBS Espeak_LIBRARY )
libfind_process( Espeak )
