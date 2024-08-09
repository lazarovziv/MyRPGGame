# escape=`
FROM mcr.microsoft.com/windows/servercore:ltsc2022

SHELL [ "powershell.exe" ]

RUN powershell -Command Add-WindowsFeature Web-Server

# RUN Invoke-WebRequest -Uri "https://aka.ms/vs/17/release/vs_buildtools.exe" -OutFile \vs_buildtools.exe `
#  -UseBasicParsing -MaximumRedirection 10

# RUN Start-Process -FilePath "./vs_buildtools.exe" -ArgumentList `
# "--quiet", "--wait", "--norestart", "--nocache", "--includeOptional", "Microsoft.VisualStudio.Workload.VCTools" `
# -Wait

# install chocolatey
RUN Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

RUN choco install git -y

# Restore the default Windows shell for correct batch processing.
SHELL ["cmd", "/S", "/C"]

RUN `
    # Download the Build Tools bootstrapper.
    curl -SL --output vs_buildtools.exe https://aka.ms/vs/17/release/vs_buildtools.exe `
    `
    # Install Build Tools with the Microsoft.VisualStudio.Workload.AzureBuildTools workload, excluding workloads and components with known issues.
    && (start /w vs_buildtools.exe --quiet --wait --norestart --nocache `
        --installPath "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools" `
        --add Microsoft.VisualStudio.Workload.VCTools `
        --includeRecommended `
        --remove Microsoft.VisualStudio.Component.Windows10SDK.10240 `
        --remove Microsoft.VisualStudio.Component.Windows10SDK.10586 `
        --remove Microsoft.VisualStudio.Component.Windows10SDK.14393 `
        --remove Microsoft.VisualStudio.Component.Windows81SDK `
        || IF "%ERRORLEVEL%"=="3010" EXIT 0) `
    `
    # Cleanup
    && del /q vs_buildtools.exe

SHELL [ "powershell.exe" ]

RUN Invoke-WebRequest -Uri "https://sdk.lunarg.com/sdk/download/1.3.290.0/windows/VulkanSDK-1.3.290.0-Installer.exe" `
-OutFile \vulkansdk-installer.exe -UseBasicParsing -MaximumRedirection 10

RUN Start-Process -FilePath "./vulkansdk-installer.exe" -ArgumentList "--accept-licenses", "--default-answer", `
 "--confirm-command", "install"

#  ttps://sdk.lunarg.com/sdk/download/1.3.290.0/windows/VulkanRT-1.3.290.0-Installer.exe

# RUN Invoke-WebRequest -Uri "https://github.com/Kitware/CMake/releases/download/v3.30.2/cmake-3.30.2-windows-x86_64.msi" `
# -OutFile \cmake-installer.msi -UseBasicParsing -MaximumRedirection 10

# RUN Start-Process -FilePath "./cmake-installer.msi" -ArgumentList "/quiet" -Wait

# RUN Invoke-WebRequest -Uri "https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip" `
# -OutFile \ninja.zip -UseBasicParsing -MaximumRedirection 10

# RUN mkdir bin
# RUN Expand-Archive -LiteralPath $pwd/ninja.zip -DestinationPath $pwd/bin/

# RUN $path = $env:path + ';C:\bin'; Set-ItemProperty -Path 'HKLM:\SYSTEM\CurrentControlSet\Control\Session Manager\Environment\' -Name Path -Value $path

# RUN Remove-Item \vs_buildtools.exe
# RUN Remove-Item \cmake-installer.msi
# RUN Remove-Item \ninja.zip

SHELL ["cmd", "/S", "/C"]

# CMD "& 'C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\Launch-VsDevShell.ps1' %*"

ENTRYPOINT ["C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat", "-arch=amd64", "&&", "powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]
