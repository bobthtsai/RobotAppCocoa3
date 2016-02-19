set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -std=c++0x")

#Directory algorithmUtil
set (AlgorithmUtilSrc
      AlgorithmUtil.cpp
      normal.cpp
      AlgorithmConfig.cc
      lodepng.cpp
      mapper.cc
      commands.cc
      HazardNotifier.hpp
      EventNotifier.hpp
      WallFollow.cpp
)
set (linkLibs
      robot
      config++
      boost_filesystem
      boost_system
)
#Lib algorithmUtil
PLAYER_ADD_PLUGIN_DRIVER (
	AlgorithmUtil
		SOURCES
			${AlgorithmUtilSrc}
    LINKLIBS
      ${linkLibs}
)
set_target_properties(AlgorithmUtil PROPERTIES LINKER_LANGUAGE CXX)


install(TARGETS  AlgorithmUtil
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION  lib
        ARCHIVE DESTINATION bin
        )