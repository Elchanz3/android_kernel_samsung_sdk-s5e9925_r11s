SECTIONS {
	__patchable_function_entries : { *(__patchable_function_entries) }
	.text : {
		__policy_heavy$cc08729119029b35b8e3c7a53e7f948e.cfi_jt = . + 0x13f48;
		__sysfs_qd_max_show$cc08729119029b35b8e3c7a53e7f948e.cfi_jt = . + 0x13fa0;
		__sysfs_duration_store$cc08729119029b35b8e3c7a53e7f948e.cfi_jt = . + 0x14000;
		__sysfs_pm_qos_cluster2_store$f707e7f56ddfb6b43d1bcefb66701e39.cfi_jt = . + 0x140e0;
		__sysfs_pm_qos_mif_show$f707e7f56ddfb6b43d1bcefb66701e39.cfi_jt = . + 0x14138;
		__ctrl_gear$5e6dc7a848cc678417d05ab311d7e4f7.cfi_jt = . + 0x14258;
	}
}
