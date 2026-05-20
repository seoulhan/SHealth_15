# Regenerates tests/golden/shealth_bmi_stdout.golden.txt from the current SHealthBMI build.
# Run from repository root after building SHealthBMI (e.g. build-gcc or build).

param(
    [string]$BuildDir = "build-gcc",
    [string]$OutFile = "tests/golden/shealth_bmi_stdout.golden.txt"
)

$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
Set-Location $Root

$Exe = Join-Path $BuildDir "SHealthBMI.exe"
if (-not (Test-Path $Exe)) {
    $Exe = Join-Path $BuildDir "SHealthBMI"
}
if (-not (Test-Path $Exe)) {
    throw "SHealthBMI not found under '$BuildDir'. Build the project first."
}

$text = & $Exe | Out-String
$text = $text -replace "`r`n", "`n"
$text = $text.TrimEnd("`n")
$normalized = ($text -split "`n" | ForEach-Object { $_.TrimEnd() }) -join "`n"
if ($normalized.Length -gt 0) { $normalized += "`n" }

$outPath = Join-Path $Root $OutFile
$utf8NoBom = New-Object System.Text.UTF8Encoding $false
[System.IO.File]::WriteAllText($outPath, $normalized, $utf8NoBom)
Write-Host "Updated golden baseline: $outPath"
