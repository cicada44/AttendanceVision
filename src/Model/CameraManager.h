#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class CameraManager {
public:
    CameraManager();
    ~CameraManager();

    void addCamera(const std::string& url);
    void removeCamera(const std::string& url);
    const std::unordered_set<std::string>& getCameraUrls() const;

    bool openStream(const std::string& url);
    cv::VideoCapture* getCapture(const std::string& url);

private:
    std::unordered_set<std::string> cameraUrls;
    std::unordered_map<std::string, cv::VideoCapture*> streams;
};
