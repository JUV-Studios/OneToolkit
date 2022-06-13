echo "Downloading third-party source files..."
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/mohaps/lrucache11/master/LRUCache11.hpp" -OutFile "OneToolkit\LRUCache11.hpp"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/TheNicker/FileVersionInfo/master/FileVersionInfo.h" -OutFile "OneToolkit\FileVersionInfo.h"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/TheNicker/FileVersionInfo/master/FileVersionInfo.cpp" -OutFile "OneToolkit\FileVersionInfo.cpp"
