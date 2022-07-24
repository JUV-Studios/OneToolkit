echo "Downloading third-party source files..."
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/TranslucentTB/TranslucentTB/release/Xaml/dependencyproperty.h" -OutFile "..\OneToolkit.UI.Xaml\Shared\DependencyProperty.h"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/TheNicker/FileVersionInfo/master/FileVersionInfo.h" -OutFile "..\OneToolkit\FileVersionInfo.h"
Invoke-WebRequest -Uri "https://raw.githubusercontent.com/TheNicker/FileVersionInfo/master/FileVersionInfo.cpp" -OutFile "..\OneToolkit\FileVersionInfo.cpp"