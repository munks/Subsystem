#ifndef _main
	#define _main
	
	//Include
	#include <stdio.h>
	#include <stdlib.h>
	#include <locale.h>
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
		wchar_t*	lp_fileDir, //_In_
		FILE**		lp_output //_Out_
	);
	int Read_NTHeader_Offset (
		FILE*			lp_file, //_InOut_
		DWORD*	lp_output //_Out_
	);
	int Read_ArchitectureMachine (
		FILE*			lp_file, //_InOut_
		DWORD	lp_nt_offset, //_In_
		WORD*		lp_output //_Out_
	);
	int Read_ImageOptionalHeader (
		FILE*			lp_file, //_InOut_
		DWORD	lp_nt_offset, //_In_
		WORD		lp_machine, //_In_
		IMAGE_OPTIONAL_HEADER64*	lp_output_header, //_Out_
		int*			lp_output_size //_Out_
	);
	int Get_ImageOptionalHeader_Subsystem (
		IMAGE_OPTIONAL_HEADER64*	lp_header //_In_
	);
	int UserInput_SubsystemIndex (
		int*			lp_output //_Out_
	);
	int Set_ImageOptionalHeader_Subsystem (
		IMAGE_OPTIONAL_HEADER64*	lp_header, //_Out_
		int			lp_subsystem //_In_
	);
	int Write_ImageOptionalHeader (
		FILE*			lp_file, //_InOut_
		DWORD	lp_nt_offset, //_In_
		IMAGE_OPTIONAL_HEADER64*	lp_write_header, //_In_
		int			lp_write_size //_In_
	);
#endif