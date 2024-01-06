cmake_minimum_required(VERSION 3.27)
project(GenIITeamBuilder)

set(CMAKE_CXX_STANDARD 23)


add_executable(
        ${PROJECT_NAME}
        src/main.cpp
        src/MyHandler.h
        src/Timer.h
        src/data_class/PokemonType.h
        src/data_class/StatModifiers.h
        src/data_class/Stat.h
        src/data_class/Category.h
        src/ThreadPool.h
        src/ThreadPool.cpp
        src/PokemonDataSource.h
        src/BattleField.h
        src/data_class/PokemonState.h
        src/PokemonDataSource.cpp
        src/data_class/PokemonInformation.h
        src/PokemonType.cpp
        src/data_class/BaseStats.h
        src/data_class/Stats.h
        src/data_class/AllStats.h
        src/data_class/Pokemon.h
        src/data_class/Attack.h
        src/data_class/Category.cpp
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   add_compile_options(-O0 -fno-inline -g)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE )
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

