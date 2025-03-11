# Esp-idf component requires this file.
# Even Arduino IDE will ignore it.
file(GLOB READGUY_SRCS
  src/*.cpp
  src/guy_epaper/*.cpp
  src/guy_epaper/guy_154a_290a/*.cpp
  src/guy_epaper/guy_154b_270b_290b/*.cpp
  src/guy_epaper/guy_154C/*.cpp
  src/guy_epaper/guy_213a/*.cpp
  src/guy_epaper/guy_213b_266a/*.cpp
  src/guy_epaper/guy_370a/*.cpp
  src/guy_epaper/guy_370B/*.cpp
  src/guy_epaper/guy_420a/*.cpp
  src/guy_epaper/guy_420b/*.cpp
  src/guy_epaper/guy_426A/*.cpp
  src/guy_epaper/guy_583A/*.cpp
  src/guy_epaper/guy_583B/*.cpp
  src/guy_epaper/guy_750A/*.cpp
  src/guy_epaper/guy_1020A/*.cpp
  src/guy_epaper/lcdDebug/*.cpp
)
set(READGUY_DIRS
    src
    src/guy_epaper
    src/guy_epaper/guy_154a_290a
    src/guy_epaper/guy_154b_270b_290b
    src/guy_epaper/guy_154C
    src/guy_epaper/guy_213a
    src/guy_epaper/guy_213b_266a
    src/guy_epaper/guy_370a
    src/guy_epaper/guy_370B
    src/guy_epaper/guy_420a
    src/guy_epaper/guy_420b
    src/guy_epaper/guy_426A
    src/guy_epaper/guy_583A
    src/guy_epaper/guy_583B
    src/guy_epaper/guy_750A
    src/guy_epaper/guy_1020A
    src/guy_epaper/lcdDebug
    ../arduino-esp32/libraries/HTTPUpdateServer/src
)
idf_component_register(SRCS ${READGUY_SRCS}
                      INCLUDE_DIRS "." ${READGUY_DIRS}
                      REQUIRES arduino-esp32 LovyanGFX spi_flash
                      )
#spi flash for read chip