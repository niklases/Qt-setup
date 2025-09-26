$qtBaseDir = "C:\Qt"
$qtSearchPattern = "6.*"
$msvcSubdir = "msvc2022_64"
$msvcGenerator = "Visual Studio 17 2022"


$qtVersions = Get-ChildItem -Path $qtBaseDir -Directory | Where-Object { $_.Name -like $qtSearchPattern } | Sort-Object Name -Descending

Write-Host "Found Qt version: $qtVersions" 

$qtDir = $null
foreach ($version in $qtVersions) {
    $candidate = Join-Path -Path $version.FullName -ChildPath $msvcSubdir
    if (Test-Path "$candidate\bin\windeployqt.exe") {
        $qtDir = $candidate
        break
    }
}

if (-not $qtDir) {
    Write-Error "Could not find a suitable Qt installation under $qtBaseDir"
    exit 1
}

Write-Host "Found Qt: $qtDir"


$buildDir = "build"
$config = "Debug"
$exePath = "$buildDir\$config\qt_app.exe"

cmake -B $buildDir -G $msvcGenerator -A x64 -DCMAKE_PREFIX_PATH="$qtDir"
cmake --build $buildDir --config $config

& "$qtDir\bin\windeployqt.exe" "$exePath"
Start-Process "$exePath"

