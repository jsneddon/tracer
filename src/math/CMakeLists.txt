add_library(math vector3.cpp vector3.h ray.cpp ray.h)

install (TARGETS math DESTINATION bin)
install (FILES vector3.h ray.h DESTINATION include)