# target
file(GLOB_RECURSE Config_Test_SOURCES "*.cpp")
file(GLOB_RECURSE Config_Test_HEADERS "*.hpp")

add_executable(ConfigTest 
	${Config_Test_SOURCES} 
	${Common_HEADERS}
	${Config_HEADERS}
)

target_include_directories(ConfigTest PUBLIC
	${Common_INCLUDES}
	${Config_INCLUDES}
)

target_link_libraries(ConfigTest 
	${Boost_LIBRARIES}
	${TBB_LIBS}
	${POCO_LIBS}
	${GTEST_LIBS}
	Common
	Config
)

add_test(ConfigTest, ConfigTest)

add_custom_command(
     TARGET ConfigTest
     COMMENT "Run tests"
     POST_BUILD 
#     COMMAND ${CMAKE_COMMAND} -E copy ConfigTest UnitTest
     COMMAND ConfigTest
)