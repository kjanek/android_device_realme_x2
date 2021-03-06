#
# Copyright (C) 2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

$(call inherit-product, device/realme/X2/device.mk)

# Inherit some common RevengeOS stuff.
TARGET_SCREEN_HEIGHT := 2340
TARGET_SCREEN_WIDTH := 1080
TARGET_BOOT_ANIMATION_RES := 1080
$(call inherit-product, vendor/revengeos/config/common.mk)

# Device identifier. This must come after all inclusions.
PRODUCT_NAME := revengeos_X2
PRODUCT_DEVICE := X2
PRODUCT_BRAND := realme
PRODUCT_MODEL := realme X2
PRODUCT_MANUFACTURER := realme

BUILD_FINGERPRINT := "realme/RMX1992/RMX1992L1:10/QKQ1.191201.002/1592194586:user/release-keys"

PRODUCT_GMS_CLIENTID_BASE := android-oppo
