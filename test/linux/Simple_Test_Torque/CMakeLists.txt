
set(SOURCES 
	simple_test_torque.c
	control.c
)
add_executable(simple_test_torque ${SOURCES})
target_link_libraries(simple_test_torque soem)
target_link_libraries(simple_test_torque m)
install(TARGETS simple_test_torque DESTINATION bin)

