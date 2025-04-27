cmake_minimum_required(VERSION 3.15)
project(RealTimeStockDashboard LANGUAGES CXX)

set(BUILD_SHARED_LIBS OFF)
add_definitions(-DCURL_STATICLIB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add local include directories for all dependencies
include_directories(
    third_party/imgui
    third_party/imgui/backends
    third_party/implot
    third_party/json/single_include
    third_party/boost
    third_party/curl/include
    third_party/glfw/include
    src
)

# Add GLFW as a local dependency
add_subdirectory(third_party/glfw)

# Gather ImGui and ImPlot source files
set(IMGUI_SRC
    third_party/imgui/imgui.cpp
    third_party/imgui/imgui_draw.cpp
    third_party/imgui/imgui_tables.cpp
    third_party/imgui/imgui_widgets.cpp
    third_party/imgui/imgui_demo.cpp
    third_party/imgui/backends/imgui_impl_glfw.cpp
    third_party/imgui/backends/imgui_impl_opengl3.cpp
)
set(IMPLOT_SRC
    third_party/implot/implot.cpp
    third_party/implot/implot_items.cpp
    third_party/implot/implot_demo.cpp
)

# nlohmann/json is header-only
# Boost is header-only for Asio

# CURL - use prebuilt static libraries for MinGW
include_directories(third_party/curl/include)
set(CURL_LIB_PATH "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libcurl.a")
set(CURL_DEP_SSL "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libssl.a")
set(CURL_DEP_CRYPTO "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libcrypto.a")
set(CURL_DEP_Z "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libz.a")
set(CURL_DEP_BROTLICOMMON "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libbrotlicommon.a")
set(CURL_DEP_BROTLIDEC "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libbrotlidec.a")
set(CURL_DEP_BROTIENC "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libbrotlienc.a")
set(CURL_DEP_NGHTTP2 "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libnghttp2.a")
set(CURL_DEP_NGHTTP3 "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libnghttp3.a")
set(CURL_DEP_NGTCP2 "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libngtcp2.a")
set(CURL_DEP_NGTCP2_CRYPTO "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libngtcp2_crypto_quictls.a")
set(CURL_DEP_PSL "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libpsl.a")
set(CURL_DEP_SSH2 "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libssh2.a")
set(CURL_DEP_ZSTD "${CMAKE_SOURCE_DIR}/third_party/curl/lib/libzstd.a")

file(GLOB SOURCES src/*.cpp)

add_executable(RealTimeStockDashboard ${SOURCES} ${IMGUI_SRC} ${IMPLOT_SRC})

# Link libraries
target_link_libraries(RealTimeStockDashboard
    PRIVATE
    glfw
    opengl32
    ${CURL_LIB_PATH}
    ${CURL_DEP_SSL}
    ${CURL_DEP_CRYPTO}
    ${CURL_DEP_Z}
    ${CURL_DEP_BROTIENC}
    ${CURL_DEP_BROTLIDEC}
    ${CURL_DEP_BROTLICOMMON}
    ${CURL_DEP_NGHTTP2}
    ${CURL_DEP_NGHTTP3}
    ${CURL_DEP_NGTCP2}
    ${CURL_DEP_NGTCP2_CRYPTO}
    ${CURL_DEP_PSL}
    ${CURL_DEP_SSH2}
    ${CURL_DEP_Z}
    ${CURL_DEP_ZSTD}
    ws2_32
    wldap32
    crypt32
    bcrypt
    advapi32
    secur32
)

# For Windows: set subsystem to windows for GUI app (optional)
if(WIN32)
    set_target_properties(RealTimeStockDashboard PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
