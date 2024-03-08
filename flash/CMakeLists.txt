add_executable(flash 
flash.h
flash.c
main.c)

# pull in common dependencies
target_link_libraries(flash pico_stdlib hardware_flash)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(flash)
