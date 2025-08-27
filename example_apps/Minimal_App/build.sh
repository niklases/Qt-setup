cmake -B build
cmake --build build
if [[ -f ./build/qt_app ]]; then
  ./build/qt_app
else
  echo "Build failed. Binary not found."
fi
