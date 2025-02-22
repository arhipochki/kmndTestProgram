$raylibIncludePath = "C:\raylib\raylib\src"
$raylibLibPath = "C:\raylib\raylib\src"

$cppFiles = Get-ChildItem -Path .\src\*.cpp | ForEach-Object { $_.FullName }

Write-Output $cppFiles

$compileCommand = "g++ -DNDEBUG -s -O3 -I $raylibIncludePath -L $raylibLibPath $cppFiles -lraylib -lwinmm -lgdi32 -lopengl32 -o main.exe"

Invoke-Expression $compileCommand

Invoke-Expression ".\main.exe"