target_sources(${PROJECT_NAME}
        PRIVATE
        namedPipe.namedPipeServer.cpp
        NamedPipeServerTask.h
        NamedPipeServerTask.cpp
        PRIVATE
        FILE_SET CXX_MODULES
        FILES
        namedPipe.dllMain.ixx
)
