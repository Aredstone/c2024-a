project(PI)
set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Ofast)

add_executable(
        LongRealNum.cpp
        LongRealNum.h
        main.cpp
        LongRealNum.cpp
        SignedLongRealNum.cpp
        SignedLongRealNum.h
)
