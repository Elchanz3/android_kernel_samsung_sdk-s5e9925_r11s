// SPDX-License-Identifier: GPL-2.0+
#include <kunit/test.h>

#include "kunit-mock-misc.h"
#include "kunit-mock-mlme.h"
#include "../qsfs.c"

/* unit test function definition */
static void test_slsi_qsf_init(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	slsi_qsf_init(sdev);
}

static void test_slsi_qsf_deinit(void)
{
	slsi_qsf_deinit();
}

static void test_slsi_get_wifi_standard(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	bool he_activate = false;

	sdev->band_6g_supported = false;
	sdev->fw_vht_enabled = false;
	sdev->fw_ht_enabled = false;

	KUNIT_EXPECT_LT(test, SLSI_WIFI_7, slsi_get_wifi_standard(sdev, he_activate));
}

static void test_slsi_get_band_conc(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	bool he_activate = true;

	sdev->dualband_concurrency = true;
	sdev->band_6g_supported = true;
	KUNIT_EXPECT_EQ(test, DUAL_BAND_CONCURRANCY_WITH_6G, slsi_get_band_conc(sdev, he_activate));

	sdev->band_6g_supported = false;
	sdev->fw_vht_enabled = true;
	KUNIT_EXPECT_EQ(test, DUAL_BAND_CONCURRANCY, slsi_get_band_conc(sdev, he_activate));
}

static void test_slsi_get_antenna_from_ht_caps(struct kunit *test)
{
	bool he_activate = true;
	u8 caps[21] = {0};

	caps[12] = 7;

	KUNIT_EXPECT_EQ(test, 2, slsi_get_antenna_from_ht_caps(caps));
}

static void test_slsi_get_nss_from_mcs_nss_map(struct kunit *test)
{
	u16 mcs_map = 0xFFFF;

	KUNIT_EXPECT_EQ(test, 1, slsi_get_nss_from_mcs_nss_map((mcs_map & 0xFFFF)));
}

static void test_slsi_get_antenna_from_vht_caps(struct kunit *test)
{
	u8 caps[12] = {0};

	KUNIT_EXPECT_EQ(test, 8, slsi_get_antenna_from_vht_caps(caps));
}

static void test_slsi_get_antenna_from_he_caps(struct kunit *test)
{
	u8 caps[32] = {0};

	KUNIT_EXPECT_EQ(test, 8, slsi_get_antenna_from_he_caps(caps));
}

static void test_slsi_qsf_encode_hw_feature(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	u8 misc_features_activated[18] = {0};
	bool he_active = true;
	char buf[1024] = {0};
	u32 bytes = 0;
	u32 buf_size = sizeof(buf);
	u8 he_caps[32] = {0};
	u8 vht_caps[12] = {0};
	u8 ht_caps[21] = {0};

	KUNIT_EXPECT_EQ(test, 0,
			slsi_qsf_encode_hw_feature(sdev, misc_features_activated, he_active,
						   buf, bytes, buf_size, he_caps, vht_caps, ht_caps));

	he_active = false;
	sdev->fw_vht_enabled = true;
	KUNIT_EXPECT_EQ(test, 0,
			slsi_qsf_encode_hw_feature(sdev, misc_features_activated, he_active,
						   buf, bytes, buf_size, he_caps, vht_caps, ht_caps));

	sdev->fw_vht_enabled = false;
	sdev->fw_ht_enabled = true;
	KUNIT_EXPECT_EQ(test, 6,
			slsi_qsf_encode_hw_feature(sdev, misc_features_activated, he_active,
						   buf, bytes, buf_size, he_caps, vht_caps, ht_caps));
}

static void test_slsi_qsf_encode_sw_feature_1(struct kunit *test)
{
	struct slsi_dev *sdev = NULL;
	u8 misc_features_activated[18] = {0, 4,};
	u32 twt_control = 13;
	bool twt_active = true;
	char buf[1024] = {0};
	u32 bytes = 0;
	u32 buf_size = sizeof(buf);

	KUNIT_EXPECT_EQ(test, 38,
			slsi_qsf_encode_sw_feature_1(sdev, misc_features_activated, twt_control, twt_active,
						     buf, bytes, buf_size));
}

static void test_slsi_qsf_encode_sw_feature_2(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	u8 misc_features_activated[18] = {0};
	bool he_softap_active = true;
	bool wpa3_active = true;
	u32 appendix_versions = 0x0102;
	char buf[1024] = {0};
	u32 bytes = 1024;
	u32 buf_size = sizeof(buf);

#ifdef SCSC_SEP_VERSION
	sdev->softap_max_client = 10;
#endif

	misc_features_activated[7] = 255;
	misc_features_activated[8] = 15;

	KUNIT_EXPECT_EQ(test, 1024,
			slsi_qsf_encode_sw_feature_2(sdev, misc_features_activated, he_softap_active,
						     wpa3_active, appendix_versions, buf, bytes, buf_size));
}

static void test_slsi_qsf_encode_sw_feature_3(struct kunit *test)
{
	struct slsi_dev *sdev = NULL;
	u8 misc_features_activated[18] = {0};
	u32 appendix_versions = 0x0102;
	char buf[1024] = {0};
	u32 bytes = 0;
	u32 buf_size = sizeof(buf);

	misc_features_activated[10] = 7;

	KUNIT_EXPECT_EQ(test, 28,
			slsi_qsf_encode_sw_feature_3(sdev, misc_features_activated,
						     appendix_versions, buf, bytes, buf_size));
}

static void test_slsi_qsf_encode_sw_feature_4(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	u8 misc_features_activated[18] = {0};
	bool tdls_active = true;
	u16 max_tdls_cli = 5;
	bool tdls_peer_uapsd_active = true;
	bool nan_fast_connect = true;
	char buf[1024] = {0};
	u32 bytes = 0;
	u32 buf_size = sizeof(buf);

	sdev->nan_enabled = true;
#ifdef CONFIG_SCSC_WLAN_SUPPORT_6G
	sdev->band_6g_supported = true;
#endif
	misc_features_activated[14] = 16;
	misc_features_activated[17] = 15;

	KUNIT_EXPECT_EQ(test, 22,
			slsi_qsf_encode_sw_feature_4(sdev, misc_features_activated,
						     tdls_active, max_tdls_cli, tdls_peer_uapsd_active,
						     nan_fast_connect, buf, bytes, buf_size));
}

static void test_slsi_get_qsf_mib_data(struct kunit *test)
{
	struct slsi_dev *sdev = kunit_kzalloc(test, sizeof(struct slsi_dev), GFP_KERNEL);
	struct slsi_qsf_mib_data *qsf_data = kunit_kzalloc(test, sizeof(struct slsi_qsf_mib_data), GFP_KERNEL);

	KUNIT_EXPECT_EQ(test, 0, slsi_get_qsf_mib_data(sdev, qsf_data));
}

static void test_sysfs_show_qsf(struct kunit *test)
{
}

/* Test fictures */
static int qsfs_test_init(struct kunit *test)
{
	kunit_log(KERN_INFO, test, "%s completed.", __func__);
	return 0;
}

static void qsfs_test_exit(struct kunit *test)
{
	kunit_log(KERN_INFO, test, "%s: completed.", __func__);
}

/* KUnit testcase definitions */
static struct kunit_case qsfs_test_cases[] = {
	KUNIT_CASE(test_slsi_qsf_init),
	KUNIT_CASE(test_slsi_qsf_deinit),
	KUNIT_CASE(test_slsi_get_wifi_standard),
	KUNIT_CASE(test_slsi_get_band_conc),
	KUNIT_CASE(test_slsi_get_antenna_from_ht_caps),
	KUNIT_CASE(test_slsi_qsf_encode_hw_feature),
	KUNIT_CASE(test_slsi_get_nss_from_mcs_nss_map),
	KUNIT_CASE(test_slsi_get_antenna_from_vht_caps),
	KUNIT_CASE(test_slsi_get_antenna_from_he_caps),
	KUNIT_CASE(test_slsi_qsf_encode_sw_feature_1),
	KUNIT_CASE(test_slsi_qsf_encode_sw_feature_2),
	KUNIT_CASE(test_slsi_qsf_encode_sw_feature_3),
	KUNIT_CASE(test_slsi_qsf_encode_sw_feature_4),
	KUNIT_CASE(test_slsi_get_qsf_mib_data),
	KUNIT_CASE(test_sysfs_show_qsf),
	{}
};

static struct kunit_suite qsfs_test_suite[] = {
	{
		.name = "qsfs-test",
		.test_cases = qsfs_test_cases,
		.init = qsfs_test_init,
		.exit = qsfs_test_exit,
	}
};

kunit_test_suites(qsfs_test_suite);
