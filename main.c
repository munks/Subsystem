#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define HEADER_SIZE 0x40
#define DOS_HEADER_OFFSET 0x3C

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
int main (int argc, char* argv[]) {
	FILE *lv_file = fopen(argv[1], "r+b");
	
	//Open File
	printf("Open File: %s\n", argv[1]);
	
	if (lv_file == NULL) {
		perror("Could Not Open File. Error Code");
		goto END;
	}
	
	//Get Header Offset
	DWORD lv_offset;
	fseek(lv_file, DOS_HEADER_OFFSET, SEEK_SET);
	fread(&lv_offset, sizeof(DWORD), 1, lv_file);
	lv_offset -= HEADER_SIZE;
	
	//Get Process Machine
	puts("Getting Process Machine");
	
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
	
	fseek(lv_file, HEADER_SIZE + lv_offset + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), SEEK_SET);
	if (Is32Processer(lv_machine)) {
		fread(&lv_optional_header32, sizeof(IMAGE_OPTIONAL_HEADER32), 1, lv_file);
	} else {
		fread(&lv_optional_header64, sizeof(IMAGE_OPTIONAL_HEADER64), 1, lv_file);
	}
	printf("Subsystem: ");
	switch (Is32Processer(lv_machine) ? lv_optional_header32.Subsystem : lv_optional_header64.Subsystem) {
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
	}

	//Write File
	if (Is32Processer(lv_machine)) {
		lv_optional_header32.Subsystem = 3;
	} else {
		lv_optional_header64.Subsystem = 3;
	}
	fseek(lv_file, HEADER_SIZE + lv_offset + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), SEEK_SET);
	if (fwrite(Is32Processer(lv_machine) ? (void*)&lv_optional_header32 : (void*)&lv_optional_header64,
					Is32Processer(lv_machine) ? sizeof(IMAGE_OPTIONAL_HEADER32) : sizeof(IMAGE_OPTIONAL_HEADER64),
					1, lv_file) == 0) {
		perror("Could Not Write File. Error Code");
		goto END;
	}
	puts("Overwrite Success - Subsystem: IMAGE_SUBSYSTEM_WINDOWS_CUI");
	
	END:
	fclose(lv_file);
	system("pause");
	return 0;
}