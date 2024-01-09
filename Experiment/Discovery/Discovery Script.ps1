Get-CimInstance -ClassName Win32_ComputerSystem -Property *
Get-CimInstance -ClassName Win32_OperatingSystem -Property *
Get-CimInstance -ClassName Win32_BIOS -Property *
Get-CimInstance -ClassName Win32_TemperatureProbe -Property *
Get-CimInstance -ClassName Win32_Fan -Property *
Get-CimInstance -ClassName Win32_NetworkAdapter -Property *
Get-CimInstance -ClassName Win32_IP4RouteTable
Get-CimInstance -ClassName Win32_Share
Get-CimInstance -ClassName Win32_Service
Get-CimInstance -ClassName Win32_Process
Get-CimInstance -ClassName Win32_UserAccount -Property *
Get-CimInstance -ClassName Win32_Group -Property *
Get-CimInstance -ClassName Win32_GroupInDomain -Property *
Get-CimInstance -ClassName Win32_LoggedOnUser -Property *
Get-CimInstance -ClassName Win32_LogOnSession -Property *

Get-WmiObject -Namespace root\directory\ldap -Class ds_* -List

Get-CimInstance -ClassName Win32_ShadowCopy
Get-CimInstance -ClassName Win32_QuickFixEngineering
Get-CimInstance -Namespace root/SecurityCenter -ClassName AntiVirusProduct
Get-CimInstance -Namespace root/SecurityCenter2 -ClassName AntiVirusProduct
Get-CimInstance -Namespace root/SecurityCenter -ClassName AntiSpywareProduct
Get-CimInstance -Namespace root/SecurityCenter2 -ClassName AntiSpywareProduct
Get-CimInstance -Namespace root/SecurityCenter -ClassName FirewallProduct
Get-CimInstance -Namespace root/SecurityCenter2 -ClassName FirewallProduct

