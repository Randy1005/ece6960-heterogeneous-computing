enable_testing()

include(${TP_3RD_PARTY_DIR}/doctest/doctest.cmake)

list(APPEND TP_UNITTESTS
  
)

foreach(unittest IN LISTS TP_UNITTESTS)
  add_executable(${unittest} ${unittest}.cpp)
  target_include_directories(${unittest} PRIVATE ${PROJECT_SOURCE_DIR} ${TP_3RD_PARTY_DIR}/doctest)
  target_link_libraries(${unittest} LINK_PUBLIC floorplanner LINK_PUBLIC stdc++fs)
  doctest_discover_tests(${unittest})
endforeach()

