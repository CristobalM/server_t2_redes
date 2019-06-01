cmake_minimum_required(VERSION 3.10)
project(server_t2_redes)

set(CMAKE_CXX_STANDARD 14)

set(Timer Timer.h)
set(Server Server.h Server.cpp)
set(ServerWorker ServerWorker.h ServerWorker.cpp)
set(ServerReceiver ServerReceiver.h ServerReceiver.cpp)
set(ServerSender ServerSender.h ServerSender.cpp)
set(ServerDataGluer ServerDataGluer.h ServerDataGluer.cpp)
set(ServerSubject ServerSubject.h ServerSubject.cpp)
set(InputProcessor InputProccesor.h InputProccesor.cpp)
set(Packet Packet.h)


set(INNER_SOURCE_FILES main.cpp
        ${Server}
        ${ServerWorker}
        ${ServerThread}
        ${ServerReceiver}
        ${ServerSender}
        ${ServerDataGluer}
        ${InputProcessor}
        ${Timer}
        ${Packet}
        )

set(RES_LIST "")
foreach (SF IN LISTS INNER_SOURCE_FILES)
    list(APPEND RES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/${SF}")
endforeach ()

set(SOURCEFILES ${RES_LIST} PARENT_SCOPE)
