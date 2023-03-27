#ifndef _main
	#define _main
	
	//Include
	#include <stdio.h>
	#include <stdlib.h>
	#include <windows.h>
	
	//Define Const
	#define DOS_HEADER_OFFSET 0x3C
	#define SUBSYSTEM_OFFSET 0x22
	
	//32-Bit Check
	#define Is32Processer(m) (m == IMAGE_FILE_MACHINE_I386)

	//Subsystem Case
	#define SubsystemCase(t) 	case t: { \
												printf(#t"\n"); \
												break; \
											}
	//Machine Type Check
	#define MachineCase(t) 	case t: { \
											printf(#t); \
											break; \
										}
	//Print Subsystem Number
	#define PrintSubsystem(s, c) printf("%d: %s%s\n", s, #s, c);

	//Subsystem Check Case
	#define SubsystemCheck(s) case s: { \
												printf("Selected Subsystem: %s\n", #s); \
												break; \
											}
	
	//Function
	int FileOpen (
		char*		lp_fileDir,
		FILE**		lp_output
	);
	int Read_NTHeader_Offset (
		FILE*			lp_file,
		DWORD*	lp_output
	);
	int Read_ArchitectureMachine (
		FILE*			lp_file,
		DWORD	lp_nt_offset,
		WORD*		lp_output
	);
	int Read_ImageOptionalHeader (
		FILE*			lp_file,
		DWORD	lp_nt_offset,
		WORD		lp_machine,
		IMAGE_OPTIONAL_HEADER64*	lp_output_header,
		int*			lp_output_size
	);
	int Get_ImageOptionalHeader_Subsystem (
		IMAGE_OPTIONAL_HEADER64*	lp_header
	);
	int UserInput_SubsystemIndex (
		int*			lp_output
	);
	int Set_ImageOptionalHeader_Subsystem (
		IMAGE_OPTIONAL_HEADER64*	lp_header,
		int			lp_subsystem
	);
	int Write_ImageOptionalHeader (
		FILE*			lp_file,
		DWORD	lp_nt_offset,
		IMAGE_OPTIONAL_HEADER64*	lp_write_header,
		int			lp_write_size
	);
#endif