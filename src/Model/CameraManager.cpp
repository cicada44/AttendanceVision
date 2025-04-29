#include "CameraManager.h"

CameraManager::CameraManager() {}

CameraManager::~CameraManager() {
    for (auto& [url, cap] : streams) {
        if (cap) {
            cap->release();
            delete cap;
        }
    }
}

void CameraManager::addCamera(const std::string& url) {
    if (cameraUrls.find(url) != cameraUrls.end()) {
        cameraUrls.insert(url);
    }
}

void CameraManager::removeCamera(const std::string& url) {
    cameraUrls.erase(std::remove(cameraUrls.begin(), cameraUrls.end(), url), cameraUrls.end());
    if (streams.find(url) != streams.end()) {
        streams[url]->release();
        delete streams[url];
        streams.erase(url);
    }
}

const std::unordered_set<std::string>& CameraManager::getCameraUrls() const { return cameraUrls; }

bool CameraManager::openStream(const std::string& url) {
    if (streams.find(url) != streams.end()) {
        auto* cap = new cv::VideoCapture(url);
        if (!cap->isOpened()) {
            delete cap;
            return false;
        }
        streams[url] = cap;
    }
    return true;
}

cv::VideoCapture* CameraManager::getCapture(const std::string& url) {
    if (streams.find(url) != streams.end()) {
        return streams[url];
    }
    return nullptr;
}
