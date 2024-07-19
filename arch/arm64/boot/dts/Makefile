# SPDX-License-Identifier: GPL-2.0
dtbo-y += samsung/r11s/r11s_eur_openx_w01_r00.dtbo
dtbo-y += samsung/r11s/r11s_eur_openx_w01_r01.dtbo
dtbo-y += samsung/r11s/r11s_eur_openx_w01_r03.dtbo
dtbo-y += samsung/r11s/r11s_eur_openx_w01_r02.dtbo
dtbo-y += samsung/r11s/r11s_eur_openx_w01_r05.dtbo
dtb-y += exynos/s5e9925.dtb

targets += dtbs
DTB_LIST  := $(dtb-y) $(dtbo-y)
always    := $(DTB_LIST)

dtbs: $(addprefix $(obj)/, $(DTB_LIST))

clean-files := *.dtb*
