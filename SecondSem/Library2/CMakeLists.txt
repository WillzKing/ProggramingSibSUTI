cmake_minimum_required(VERSION 3.10)
project(MatrixOperations)

# Настройка опций для выбора типа библиотеки
option(BUILD_STATIC "Build static library" ON)
option(BUILD_SHARED "Build shared library" OFF)

# Добавляем include директорию
include_directories(include)

# Создаем библиотеку в зависимости от выбранного типа
if(BUILD_STATIC)
    add_library(matrix STATIC src/matrix_operations.c src/matrix_io.c)
    set(LIB_TYPE STATIC)
elseif(BUILD_SHARED)
    add_library(matrix SHARED src/matrix_operations.c src/matrix_io.c)
    set(LIB_TYPE SHARED)
endif()

# Создаем исполняемый файл
add_executable(matrix_program src/main.c)
target_link_libraries(matrix_program matrix)

# Установка пути к библиотеке для динамической версии
if(BUILD_SHARED)
    set_target_properties(matrix_program PROPERTIES
        BUILD_RPATH "$ORIGIN/../lib"
    )
endif()