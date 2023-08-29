# specify source files
set(VM_SRCS )

# specify header files
set(VM_HDRS 
    vm.h
    vm_impl.h
)

# add the files to the target
target_sources(${PROJECT_NAME} PRIVATE ${VM_SRCS})
target_sources(${PROJECT_NAME} PUBLIC ${VM_HDRS})