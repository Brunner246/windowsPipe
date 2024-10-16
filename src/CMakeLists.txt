target_sources(${PROJECT_NAME}
        PRIVATE
        windowsPipe.namedPipeServer.cpp
        PRIVATE
        FILE_SET CXX_MODULES
        FILES
        windowsPipe.dllMain.ixx
)
