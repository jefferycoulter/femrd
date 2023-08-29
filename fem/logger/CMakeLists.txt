# specify source files
set(LOGGER_SRCS )

# specify header files
set(LOGGER_HDRS 
    logger.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${LOGGER_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${LOGGER_HDRS})