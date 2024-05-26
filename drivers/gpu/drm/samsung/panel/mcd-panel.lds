SECTIONS {
	__patchable_function_entries : { *(__patchable_function_entries) }
	.text : {
		__generate_irc_v2.cfi_jt = . + 0x4bfe0;
	}
}
