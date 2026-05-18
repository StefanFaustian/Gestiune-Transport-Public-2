cmake_minimum_required(VERSION 4.1)
project(Tema2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Tema2
        main.cpp
        Autobuz.h
        Tramvai.h
        Vehicul.h
        Vehicul.cpp
        Autobuz.cpp
        Tramvai.cpp
        Troleibuz.h
        Troleibuz.cpp
        Depou.cpp
        Depou.h
        Exceptii.h
        Manager.h
        Manager.cpp
        Utilitare.h
        Linie.cpp
        Linie.h
        Meniu.cpp
        Meniu.h
        VehiculFactory.h)

# Preia toate fisierele cu extensia .txt
file(GLOB DATE_TEXT "${CMAKE_CURRENT_SOURCE_DIR}/*.txt")
file(COPY ${DATE_TEXT} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Copiaza automat fișierele .txt în folderul de build
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Depouri.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Obor.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})