/*
 * Copyright (C) 2019 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "FingerprintInscreenService"
//#define FOD_DEBUG

#include "FingerprintInscreen.h"
#include <hidl/HidlTransportSupport.h>
#include <android-base/logging.h>
#include <fstream>
#include <cmath>
#include <thread>

/* Hardcoded stuffs */
#define FP_PRESS_PATH "/sys/kernel/oppo_display/notify_fppress"
#define DIMLAYER_PATH "/sys/kernel/oppo_display/dimlayer_hbm"
#define HBM_PATH "/sys/kernel/oppo_display/hbm"
#define X_POS 445
#define Y_POS 1988
#define FP_SIZE 190
#define FP_BEGIN 1
#define FP_ENDIT 0

namespace {

template <typename T>
inline static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value;
#ifdef FOD_DEBUG
    LOG(INFO) << "wrote path: " << path << ", value: " << value << "\n";
#endif
}

template <typename T>
inline static T get(const std::string& path, const T& def) {
    std::ifstream file(path);
    T result;

    file >> result;
    return file.fail() ? def : result;
}

} // anonymous namespace

namespace vendor {
namespace lineage {
namespace biometrics {
namespace fingerprint {
namespace inscreen {
namespace V1_1 {
namespace implementation {

FingerprintInscreen::FingerprintInscreen():mFingerPressed{false} {
}

Return<int32_t> FingerprintInscreen::getPositionX() {
    return X_POS;
}

Return<int32_t> FingerprintInscreen::getPositionY() {
    return Y_POS;
}

Return<int32_t> FingerprintInscreen::getSize() {
    return FP_SIZE;
}

Return<void> FingerprintInscreen::onStartEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::onFinishEnroll() {
    return Void();
}

Return<void> FingerprintInscreen::switchHbm(bool enabled) {
    if (enabled) {
        set(HBM_PATH, FP_BEGIN);
    } else {
        set(HBM_PATH, FP_ENDIT);
    }
    return Void();
}

Return<void> FingerprintInscreen::onPress() {
#ifdef FOD_DEBUG
    LOG(INFO) << __func__ << " start";
#endif
    mFingerPressed = true;
    set(DIMLAYER_PATH, FP_BEGIN);
    std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(39));
        if (mFingerPressed) {
#ifdef FOD_DEBUG
            LOG(INFO) << "Enabling sensor node";
#endif
            set(FP_PRESS_PATH, FP_BEGIN);
        }
    }).detach();
#ifdef FOD_DEBUG
    LOG(INFO) << __func__ << " exit";
#endif
    return Void();
}

Return<void> FingerprintInscreen::onRelease() {
#ifdef FOD_DEBUG
    LOG(INFO) << __func__ << " start";
#endif
    mFingerPressed = false;
    set(FP_PRESS_PATH, FP_ENDIT);
    set(DIMLAYER_PATH, FP_ENDIT);
#ifdef FOD_DEBUG
    LOG(INFO) << __func__ << " exit";
#endif
    return Void();
}

Return<void> FingerprintInscreen::onShowFODView() {
    return Void();
}

Return<void> FingerprintInscreen::onHideFODView() {
#ifdef FOD_DEBUG
    LOG(INFO) << __func__ << " start";
#endif
    set(DIMLAYER_PATH, FP_ENDIT);
    set(FP_PRESS_PATH, FP_ENDIT);
#ifdef FOD_DEBUG
    LOG(INFO) << __func__ << " exit";
#endif
    return Void();
}

Return<bool> FingerprintInscreen::handleAcquired(int32_t acquiredInfo, int32_t vendorCode) {
#ifdef FOD_DEBUG
    LOG(ERROR) << "acquiredInfo: " << acquiredInfo << ", vendorCode: " << vendorCode << "\n";
#else
    (void)acquiredInfo;
    (void)vendorCode;
#endif
    return false;
}

Return<bool> FingerprintInscreen::handleError(int32_t error, int32_t vendorCode) {
#ifdef FOD_DEBUG
    LOG(ERROR) << "error: " << error << ", vendorCode: " << vendorCode << "\n";
#else
    (void)error;
    (void)vendorCode;
#endif
    return false;
}

Return<void> FingerprintInscreen::setLongPressEnabled(bool) {
    return Void();
}

Return<int32_t> FingerprintInscreen::getDimAmount(int32_t brightness) {
    double dim = (255 + ((-12.08071) * pow((double)brightness, 0.4)));
#ifdef FOD_DEBUG
    LOG(INFO) << "Calculated dim amount was: " << dim;
#endif
    return (int32_t)dim;
}

Return<bool> FingerprintInscreen::shouldBoostBrightness() {
    return false;
}

Return<void> FingerprintInscreen::setCallback(const sp<::vendor::lineage::biometrics::fingerprint::inscreen::V1_0::IFingerprintInscreenCallback>& callback) {
    {
        std::lock_guard<std::mutex> _lock(mCallbackLock);
        mCallback = callback;
    }
    return Void();
}

}  // namespace implementation
}  // namespace V1_1
}  // namespace inscreen
}  // namespace fingerprint
}  // namespace biometrics
}  // namespace lineage
}  // namespace vendor