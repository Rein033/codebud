# Tiny static file server for the CodeBud web flasher.
# Run with: powershell -ExecutionPolicy Bypass -File serve.ps1
# Then open http://localhost:8000 in Chrome or Edge.

$port = 8000
$root = $PSScriptRoot

$mime = @{
    ".html" = "text/html"
    ".json" = "application/json"
    ".js"   = "application/javascript"
    ".bin"  = "application/octet-stream"
}

$listener = New-Object System.Net.HttpListener
$listener.Prefixes.Add("http://localhost:$port/")
$listener.Start()
Write-Host "Serving $root at http://localhost:$port/  (Ctrl+C to stop)"

try {
    while ($listener.IsListening) {
        $context = $listener.GetContext()
        $request = $context.Request
        $response = $context.Response

        $path = $request.Url.LocalPath.TrimStart("/")
        if ([string]::IsNullOrEmpty($path)) { $path = "index.html" }
        $file = Join-Path $root $path

        if (Test-Path $file -PathType Leaf) {
            $ext = [System.IO.Path]::GetExtension($file)
            $contentType = $mime[$ext]
            if (-not $contentType) { $contentType = "application/octet-stream" }

            $bytes = [System.IO.File]::ReadAllBytes($file)
            $response.ContentType = $contentType
            $response.ContentLength64 = $bytes.Length
            $response.OutputStream.Write($bytes, 0, $bytes.Length)
        } else {
            $response.StatusCode = 404
        }

        $response.OutputStream.Close()
    }
} finally {
    $listener.Stop()
}
