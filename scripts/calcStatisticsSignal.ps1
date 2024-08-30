$exePath = "./bin/Signal.exe"
$deletePreviousRecordsPath = "./output/Signal*.txt"

if ($args.Count -lt 4) {
    Write-Host "Usage: $0 <number_of_repetitions> <number_of_threads> <mask_size> <file_path>"
    exit 1
}

$runCount = [int]$args[0]
$threadsNum = [int]$args[1]
$maskSize = [int]$args[2]
$filePath = $args[3]

$minVariant = 0
$maxVariant = 1

$naiveApproach = 0
$smartApproach = 1

if (-not (Test-Path $exePath)) {
    Write-Host "Error: Executable file '$exePath' not found."
    exit 1
}

Remove-Item -Path $deletePreviousRecordsPath -Force

for ($i = 0; $i -lt $runCount; $i++) {
    Write-Host "Running $exePath ($($i + 1)/$runCount)..."
    & $exePath 1 $minVariant $naiveApproach $filePath $maskSize
    & $exePath 1 $maxVariant $naiveApproach $filePath $maskSize
    & $exePath $threadsNum $minVariant $naiveApproach $filePath $maskSize
    & $exePath $threadsNum $maxVariant $naiveApproach $filePath $maskSize
    & $exePath 1 $minVariant $smartApproach $filePath $maskSize
    & $exePath 1 $maxVariant $smartApproach $filePath $maskSize
    & $exePath $threadsNum $minVariant $smartApproach $filePath $maskSize
    & $exePath $threadsNum $maxVariant $smartApproach $filePath $maskSize
}

& ./bin/Statistics.exe