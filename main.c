#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define HEADER_SIZE 0x40
#define DOS_HEADER_OFFSET 0x3C
#define SUBSYSTEM_OFFSET 0x22

//32-Bit Checker(Bool)
#define Is32Processer(m) (m == IMAGE_FILE_MACHINE_I386)

//Subsystem Case(Print)
#define SubsystemCase(t) 	case t: { \
											printf(#t"\n"); \
											break; \
										}
//Machine Type Check(Print)
#define MachineCase(t) 	case t: { \
										printf(#t); \
										break; \
									}
//Print Subsystem Number (Print)
#define PrintSubsystem(s, c) printf("%d: %s%s\n", s, #s, c);

//Subsystem Check Case (Print)
#define SubsystemCheck(s) case s: { \
											printf("Selected Subsystem: %s\n", #s); \
											break; \
										}
int main (int argc, char* argv[]) {
	FILE *lv_file = fopen(argv[1], "r+b");
	
	//Open File
	printf("Open File: %s\n", argv[1]);
	
	if (lv_file == NULL) {
		perror("Could Not Open File. Error Code");
		goto END;
	}
	
	//Check PE (File Name)
	if (strstr(argv[1], ".exe") == NULL) {
		puts("File is not PE Format.");
		goto END;
	}
	
	//Check PE (Header Character)
	WORD lv_pe;
	fread(&lv_pe, sizeof(WORD), 1, lv_file);
	if (lv_pe != 0x5A4D) {
		puts("File is not PE Format.");
		goto END;
	}
	
	//Get Header Offset
	DWORD lv_offset;
	fseek(lv_file, DOS_HEADER_OFFSET, SEEK_SET);
	fread(&lv_offset, sizeof(DWORD), 1, lv_file);
	lv_offset -= HEADER_SIZE;
	
	//Get Process Machine
	puts("");
	WORD lv_machine;
	fseek(lv_file, HEADER_SIZE + lv_offset + sizeof(DWORD), SEEK_SET);
	fread(&lv_machine, sizeof(WORD), 1, lv_file);
	
	printf("Process Machine: %X(", lv_machine);
	switch(lv_machine) {
		MachineCase(IMAGE_FILE_MACHINE_I386);
		MachineCase(IMAGE_FILE_MACHINE_IA64);
		MachineCase(IMAGE_FILE_MACHINE_AMD64);
		default: {
			printf("Unknown");
			break;
		}
	}
	puts(")");
	
	//Get Header
	IMAGE_OPTIONAL_HEADER32 lv_optional_header32;
	IMAGE_OPTIONAL_HEADER64 lv_optional_header64;
	
	WORD* lv_optional_header = Is32Processer(lv_machine) ? (WORD*)&lv_optional_header32 : (WORD*)&lv_optional_header64;
	int lv_header_size = Is32Processer(lv_machine) ? sizeof(IMAGE_OPTIONAL_HEADER32) : sizeof(IMAGE_OPTIONAL_HEADER64);
	
	fseek(lv_file, HEADER_SIZE + lv_offset + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), SEEK_SET);
	fread(lv_optional_header, lv_header_size, 1, lv_file);

	printf("Subsystem: ");
	switch (lv_optional_header[SUBSYSTEM_OFFSET]) {
		SubsystemCase(IMAGE_SUBSYSTEM_UNKNOWN);
		SubsystemCase(IMAGE_SUBSYSTEM_NATIVE);
		SubsystemCase(IMAGE_SUBSYSTEM_WINDOWS_GUI);
		SubsystemCase(IMAGE_SUBSYSTEM_WINDOWS_CUI);
		SubsystemCase(IMAGE_SUBSYSTEM_OS2_CUI);
		SubsystemCase(IMAGE_SUBSYSTEM_POSIX_CUI);
		SubsystemCase(IMAGE_SUBSYSTEM_WINDOWS_CE_GUI);
		SubsystemCase(IMAGE_SUBSYSTEM_EFI_APPLICATION);
		SubsystemCase(IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER);
		SubsystemCase(IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER);
		SubsystemCase(IMAGE_SUBSYSTEM_EFI_ROM);
		SubsystemCase(IMAGE_SUBSYSTEM_XBOX);
		default: {
			printf("Not PE format Subsystem.");
		}
	}
	
	//Select Subsystem
	int lv_subsystem;
	
	puts("\nChoose Subsystem Number.");
	PrintSubsystem(IMAGE_SUBSYSTEM_UNKNOWN, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_NATIVE, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_WINDOWS_GUI, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_WINDOWS_CUI, " (Console)");
	PrintSubsystem(IMAGE_SUBSYSTEM_OS2_CUI, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_POSIX_CUI, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_WINDOWS_CE_GUI, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_EFI_APPLICATION, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_EFI_ROM, "");
	PrintSubsystem(IMAGE_SUBSYSTEM_XBOX, "");
	
	CONSOLE_SCREEN_BUFFER_INFO lv_csbi; //For Console Coordinate
	
	printf("Number: ");
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &lv_csbi);
	
	INPUT:
	scanf("%d", &lv_subsystem);
	
	//Check Valid
	switch (lv_subsystem) {
		SubsystemCheck(IMAGE_SUBSYSTEM_UNKNOWN);
		SubsystemCheck(IMAGE_SUBSYSTEM_NATIVE);
		SubsystemCheck(IMAGE_SUBSYSTEM_WINDOWS_GUI);
		SubsystemCheck(IMAGE_SUBSYSTEM_WINDOWS_CUI);
		SubsystemCheck(IMAGE_SUBSYSTEM_OS2_CUI);
		SubsystemCheck(IMAGE_SUBSYSTEM_POSIX_CUI);
		SubsystemCheck(IMAGE_SUBSYSTEM_WINDOWS_CE_GUI);
		SubsystemCheck(IMAGE_SUBSYSTEM_EFI_APPLICATION);
		SubsystemCheck(IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER);
		SubsystemCheck(IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER);
		SubsystemCheck(IMAGE_SUBSYSTEM_EFI_ROM);
		SubsystemCheck(IMAGE_SUBSYSTEM_XBOX);
		default: {
			printf("Invalid Number: %-10d", lv_subsystem);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), lv_csbi.dwCursorPosition);
			printf("                  "); //Empty Text
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), lv_csbi.dwCursorPosition);
			goto INPUT;
		}
	}
	
	//Write File
	lv_optional_header[SUBSYSTEM_OFFSET] = (WORD)lv_subsystem;

	fseek(lv_file, HEADER_SIZE + lv_offset + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), SEEK_SET);
	if (fwrite(lv_optional_header, lv_header_size, 1, lv_file) != 1) {
		perror("Could Not Write File. Error Code");
		goto END;
	}
	puts("\nOverwrite Success.");
	
	END:
	fclose(lv_file);
	system("pause");
	return 0;
}