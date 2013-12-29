 SOURCES = main.cpp \
    usereventshandler.cpp \
    GCSConnecton.cpp \
    GCSCommunicator.cpp \
    CommObject.cpp

 # install
 target.path = $$[QT_INSTALL_EXAMPLES]/tutorials/widgets/RemoteUserSteering
 sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS RemoteUserSteering.pro
 sources.path = $$[QT_INSTALL_EXAMPLES]/tutorials/widgets/RemoteUserSteering
 INSTALLS += target sources

 symbian: include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
 maemo5: include($$QT_SOURCE_TREE/examples/maemo5pkgrules.pri)

 symbian: warning(This example might not fully work on Symbian platform)
 maemo5: warning(This example might not fully work on Maemo platform)
 simulator: warning(This example might not fully work on Simulator platform)

HEADERS += \
    usereventshandler.h \
    GCSConnecton.h \
    GCSCommunicator.h \
    defines.h \
    GCSConnecton.h \
    GCSCommunicator.h \
    CommObject.h
