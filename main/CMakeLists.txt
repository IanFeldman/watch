file(GLOB_RECURSE SOURCES "../src/*.c")

idf_component_register(
    SRCS "main.c"
         ${SOURCES}
    INCLUDE_DIRS "."
                 "../inc")
