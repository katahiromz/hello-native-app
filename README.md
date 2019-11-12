# hello-native-app

This is a Windows NT native application.

## How to use

1. Copy this folder to `$(REACTOS)/base/setup`.
2. Modify `$(REACTOS)/base/setup/CMakeLists.txt` as follows:

```txt
add_subdirectory(lib)
add_subdirectory(reactos)
add_subdirectory(setup)
add_subdirectory(usetup)
add_subdirectory(welcome)
add_subdirectory(hello-native-app)  # add this line
```

3. Build the modified ReactOS.
4. You will get the program files in output folder.
5. Execute "`00-native-starter.exe 02-tick-count.exe`" on Command Prompt of Windows 2003 or ReactOS.
6. You'll get a bitmap file `02-tick-count.txt` in `C:\Windows` or `C:\ReactOS`.

## Reference

- Wikipedia: https://en.wikipedia.org/wiki/Native_API
- The Undocumented Functions: https://undocumented.ntinternals.net/
