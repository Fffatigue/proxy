cmake_minimum_required(VERSION 3.10)
project(proxy)

set(CMAKE_CXX_STANDARD 98)

add_executable(proxy main.cpp Utils.cpp Utils.h Proxy.cpp Proxy.h Server.cpp Server.h ConnectionsHandler.cpp ConnectionsHandler.h Connection/Connection.h Connection/DirectConnection.cpp Connection/DirectConnection.h Cache.cpp Cache.h third-party/picohttpparser.c third-party/picohttpparser.h Connection/ErrorConnection.cpp Connection/ErrorConnection.h)