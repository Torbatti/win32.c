if not exist build mkdir build
if not exist local mkdir local

zig cc src/win32wglx.c -o build/win32wglx.exe -Iinclude -luser32 -lgdi32 -lopengl32
zig cc src/win32raw.c -o build/win32raw.exe -Iinclude -luser32 -lgdi32 -lopengl32
