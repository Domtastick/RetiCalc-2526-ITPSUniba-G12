cmake_minimum_required(VERSION 3.0)
project(esonero C)

set(CMAKE_C_STANDARD 11)

add_executable(clientTCP
    SourceTCP_G12/client.c
    SourceTCP_G12/globali.c
    SourceTCP_G12/main.h
    SourceTCP_G12/utility.c
)
add_executable(serverTCP
    SourceTCP_G12/server.c
    SourceTCP_G12/globali.c
    SourceTCP_G12/main.h
    SourceTCP_G12/utility.c
)
set_target_properties(clientTCP serverTCP
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/TCP_G12"
)
add_executable(clientUDP
    SourceUDP_G12/client.c
    SourceUDP_G12/main.h
    SourceUDP_G12/utility.c
)
add_executable(serverUDP
    SourceUDP_G12/server.c
    SourceUDP_G12/main.h
    SourceUDP_G12/utility.c
)
set_target_properties(clientUDP serverUDP
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/UDP_G12"
)
