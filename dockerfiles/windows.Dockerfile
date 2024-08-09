# escape=`
FROM mcr.microsoft.com/windows/servercore:ltsc2022

SHELL [ "powershell.exe" ]

RUN powershell -Command Add-WindowsFeature Web-Server

RUN Invoke-WebRequest -Uri "https://aka.ms/vs/17/release/vs_buildtools.exe" -OutFile \vs_buildtools.exe `
 -UseBasicParsing -MaximumRedirection 10

RUN Start-Process -FilePath "./vs_buildtools.exe" -ArgumentList `
"--quiet", "--wait", "--norestart", "--nocache", "--includeRecommended", "Microsoft.VisualStudio.Workload.VCTools" `
-Wait

RUN Remove-Item \vs_buildtools.exe
