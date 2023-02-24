cmake_minimum_required(VERSION 3.21)
project(BakaMsvc)

# Dependencies
find_package(wil CONFIG REQUIRED)


add_library(${PROJECT_NAME} SHARED "address_table.ixx" "native_dll.cpp")
# Macros
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "UNICODE=1"
    "_UNICODE=1"
    "WIN32_LEAN_AND_MEAN=1"
    "WINVER=_WIN32_WINNT_WIN7" # 把最低支持的操作系统设为 Windows 7
    "_WIN32_WINNT=_WIN32_WINNT_WIN7" # 把最低支持的操作系统设为 Windows 7
)
# UTF-8 and debug
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:/ZI>>"
)
target_link_options(${PROJECT_NAME} PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:$<$<CONFIG:Debug>:/SAFESEH:NO>>"
)
# C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# Static link and LTO
set_target_properties(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    INTERPROCEDURAL_OPTIMIZATION "$<$<CONFIG:Debug>:FALSE:TRUE>"
)
# Dependencies
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CPP_HTTPLIB_INCLUDE_DIRS}
    ${DETOURS_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    WIL::WIL
)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ".")
