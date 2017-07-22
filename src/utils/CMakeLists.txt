add_library(utils sceneloader.cpp sceneloader.h)

install (TARGETS utils DESTINATION bin)
install (FILES sceneloader.h DESTINATION include)