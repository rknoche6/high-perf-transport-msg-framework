add_executable(timestamp_test base/TimestampTest.cpp)
target_link_libraries(timestamp_test PRIVATE highperf::core)
add_test(NAME timestamp_test COMMAND timestamp_test)

add_executable(buffer_test base/BufferTest.cpp)
target_link_libraries(buffer_test PRIVATE highperf::core)
add_test(NAME buffer_test COMMAND buffer_test)
