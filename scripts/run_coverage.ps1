# Mission 2: generate domain line coverage (gcov + parse_domain_coverage.py)
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Build = Join-Path $Root "build"
$Python = "C:\Users\user\AppData\Local\Programs\Python\Python312\python.exe"
$CoverageDir = Join-Path $Root "docs\coverage"

# Header .gcov is last-writer-wins; process these gcda files after others.
$GcovPriorityLast = @("filters_test.cpp.gcda", "text_analyzer_test.cpp.gcda")

Push-Location $Build
try {
    & ".\feedback_analyzer_tests.exe" | Out-Null
    $gcdaDirs = @(
        "CMakeFiles\feedback_analyzer_tests.dir\src\cpp",
        "CMakeFiles\feedback_analyzer_tests.dir\tests"
    )
    foreach ($rel in $gcdaDirs) {
        $dir = Join-Path $Build $rel
        if (-not (Test-Path $dir)) { continue }
        Push-Location $dir
        $gcdaFiles = Get-ChildItem -Filter "*.gcda" | Sort-Object Name
        $ordered = @(
            $gcdaFiles | Where-Object { $GcovPriorityLast -notcontains $_.Name }
            $gcdaFiles | Where-Object { $GcovPriorityLast -contains $_.Name }
        )
        foreach ($g in $ordered) {
            if ($g) { gcov $g.Name 2>$null | Out-Null }
        }
        Pop-Location
    }
} finally {
    Pop-Location
}

& $Python (Join-Path $Root "scripts\parse_domain_coverage.py")

# Optional: gcovr HTML (often needs extra Windows path tuning; failures are non-fatal)
$HtmlOut = Join-Path $CoverageDir "coverage.html"
New-Item -ItemType Directory -Force -Path $CoverageDir | Out-Null
if (Get-Command gcovr -ErrorAction SilentlyContinue) {
    try {
        Push-Location $Build
        gcovr -r $Root --gcov-filter "feedback_analyzer_tests" -f "src/cpp" `
            --exclude "tests/" --html $HtmlOut --html-details 2>$null
        Write-Host "HTML report: $HtmlOut"
    } catch {
        Write-Host "gcovr HTML skipped (see docs/coverage.md for manual gcov)."
    } finally {
        Pop-Location
    }
}
