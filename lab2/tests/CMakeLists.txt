# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(Tests)

# Добавляем тестируемый код
add_executable(tests tests.cpp ../src/my_threads.h ../src/my_threads.cpp ../src/ArrayData.h)

target_include_directories(tests PRIVATE ./src) # Подключаем заголовочные файлы

add_test(NAME MyTest COMMAND tests)