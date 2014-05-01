INCLUDE( LibFindMacros )
# Unix style platforms

FIND_PATH(Portaudio_INCLUDE_DIR
	NAMES
		portaudio.h
	PATHS
		/home/brunner/espeak-1.48.04-source/src

)

# AMD Stream SDK, (should be updated to APPSDK) default location
FIND_LIBRARY(Portaudio_LIBRARY 
		portaudio
	PATHS
		/usr/lib/x86_64-linux-gnu/
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set( Portaudio_PROCESS_INCLUDES Portaudio_INCLUDE_DIR )
set( Portaudio_PROCESS_LIBS Portaudio_LIBRARY )
libfind_process( Portaudio )


