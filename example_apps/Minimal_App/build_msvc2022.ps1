cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/msvc2022_64"

cmake --build build --config Debug

$buildDir = ".\build\Debug"

& "C:\Qt\6.9.1\msvc2022_64\bin\windeployqt.exe" "$buildDir\qt_app.exe"

Start-Process "$buildDir\qt_app.exe"
