cmake_minimum_required(VERSION 3.10)

# Название вашего проекта
project(IHost)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указание директории для заголовочных файлов
include_directories(include)

# Путь к vcpkg.cmake
#set(CMAKE_TOOLCHAIN_FILE "C:/Projects/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Поиск библиотеки yaml-cpp
find_package(yaml-cpp REQUIRED)

# Создание исполняемого файла
add_executable(ihost src/main.cpp)

# Подключение зависимости
target_link_libraries(ihost PRIVATE yaml-cpp)
