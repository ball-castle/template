param([Parameter(Mandatory=$true)][string]$Docx,
      [Parameter(Mandatory=$true)][string]$Pdf)
$ErrorActionPreference = 'Stop'
$inputPath = (Resolve-Path -LiteralPath $Docx).Path
$outputPath = [System.IO.Path]::GetFullPath($Pdf)
$wordApp = $null
$document = $null
$exportSucceeded = $false
try {
    $wordApp = New-Object -ComObject Word.Application
    $wordApp.Visible = $false
    $wordApp.DisplayAlerts = 0
    $document = $wordApp.Documents.Open($inputPath, $false, $false)
    $document.Repaginate()
    $null = $document.Fields.Update()
    $document.Save()
    $document.ExportAsFixedFormat($outputPath, 17)
    $pageCount = $document.ComputeStatistics(2)
    $exportSucceeded = $true
} finally {
    if ($null -ne $document) {
        $document.Close(0)
        [void][System.Runtime.InteropServices.Marshal]::FinalReleaseComObject($document)
    }
    if ($null -ne $wordApp) {
        $wordApp.Quit()
        [void][System.Runtime.InteropServices.Marshal]::FinalReleaseComObject($wordApp)
    }
    [GC]::Collect()
    [GC]::WaitForPendingFinalizers()
}
if ($exportSucceeded) {
    $binding = @{
        engine = 'Microsoft Word ExportAsFixedFormat'
        docx_sha256 = (Get-FileHash -LiteralPath $inputPath -Algorithm SHA256).Hash.ToLowerInvariant()
        pdf_sha256 = (Get-FileHash -LiteralPath $outputPath -Algorithm SHA256).Hash.ToLowerInvariant()
    }
    $binding | ConvertTo-Json | Set-Content -LiteralPath ($outputPath + '.binding.json') -Encoding utf8
    Write-Output "Exported PDF ($pageCount pages): $outputPath"
}
