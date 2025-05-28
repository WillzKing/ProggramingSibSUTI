cmake_minimum_required(VERSION 3.10)  # Должно быть ПЕРВОЙ строкой!
project(Phonebook)

set(CMAKE_C_STANDARD 11)

# Опция для выбора типа библиотеки
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Создаем библиотеку из файлов в текущей директории
add_library(phonebook 
    phonebook.c
    sort.c
)

# Указываем где искать заголовочные файлы (текущая директория и include)
target_include_directories(phonebook PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Создаем исполняемый файл
add_executable(phonebook_app main.c)

# Линкуем с библиотекой
target_link_libraries(phonebook_app phonebook)

# Для Windows: копируем DLL рядом с EXE, если это динамическая библиотека
if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET phonebook_app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:phonebook>
        $<TARGET_FILE_DIR:phonebook_app>
    )
endif()