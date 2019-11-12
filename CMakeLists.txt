# 00-native-starter.exe
add_executable(00-native-starter 00-native-starter.c)
set_module_type(00-native-starter win32cui)
target_link_libraries(00-native-starter wine)
add_importlibs(00-native-starter user32 msvcrt kernel32 ntdll)
add_cd_file(TARGET 00-native-starter DESTINATION reactos FOR all)

# 01-read-write-file.exe
add_executable(01-read-write-file WIN32 01-read-write-file.c common.c)
set_module_type(01-read-write-file nativecui)
target_link_libraries(01-read-write-file nt)
add_importlibs(01-read-write-file ntdll)
add_cd_file(TARGET 01-read-write-file DESTINATION reactos FOR all)

# 02-tick-count.exe
add_executable(02-tick-count WIN32 02-tick-count.c common.c)
set_module_type(02-tick-count nativecui)
target_link_libraries(02-tick-count nt)
add_importlibs(02-tick-count ntdll)
add_cd_file(TARGET 02-tick-count DESTINATION reactos FOR all)

# 03-alloc-memory.exe
add_executable(03-alloc-memory WIN32 03-alloc-memory.c common.c)
set_module_type(03-alloc-memory nativecui)
target_link_libraries(03-alloc-memory nt)
add_importlibs(03-alloc-memory ntdll)
add_cd_file(TARGET 03-alloc-memory DESTINATION reactos FOR all)

# 04-write-bitmap.exe
add_executable(04-write-bitmap WIN32 04-write-bitmap.c common.c)
set_module_type(04-write-bitmap nativecui)
target_link_libraries(04-write-bitmap nt)
add_importlibs(04-write-bitmap ntdll)
add_cd_file(TARGET 04-write-bitmap DESTINATION reactos FOR all)
