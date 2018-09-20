cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRYPTO")

add_library( push
             SHARED
             src/main/cpp/push.cpp
             src/main/cpp/Queue.cpp
             src/main/cpp/RtmpPush.cpp
             src/main/cpp/CallJava.cpp

             src/main/cpp/librtmp/amf.c
             src/main/cpp/librtmp/hashswf.c
             src/main/cpp/librtmp/log.c
             src/main/cpp/librtmp/parseurl.c
             src/main/cpp/librtmp/rtmp.c
              )


target_link_libraries( push
                       log)