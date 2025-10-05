cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_TOOLCHAIN_FILE=/Users/admin/vcpkg/scripts/buildsystems/vcpkg.cmake .
make -j10 -B
sudo ./output/main
