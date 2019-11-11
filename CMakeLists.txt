# native-starter.exe
add_executable(native-starter native-starter.c)
set_module_type(native-starter win32cui)
target_link_libraries(native-starter wine)
add_importlibs(native-starter user32 msvcrt kernel32 ntdll)
add_cd_file(TARGET native-starter DESTINATION reactos FOR all)

# hello-native-app.exe
add_executable(hello-native-app WIN32 hello-native-app.c)
set_module_type(hello-native-app nativecui)
target_link_libraries(hello-native-app nt)
add_importlibs(hello-native-app ntdll)
add_cd_file(TARGET hello-native-app DESTINATION reactos FOR all)

# memory.exe
add_executable(memory WIN32 memory.c)
set_module_type(memory nativecui)
target_link_libraries(memory nt)
add_importlibs(memory ntdll)
add_cd_file(TARGET memory DESTINATION reactos FOR all)

# tick-count.exe
add_executable(tick-count WIN32 tick-count.c)
set_module_type(tick-count nativecui)
target_link_libraries(tick-count nt)
add_importlibs(tick-count ntdll)
add_cd_file(TARGET tick-count DESTINATION reactos FOR all)
