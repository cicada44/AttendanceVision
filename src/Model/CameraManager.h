#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <QSqlQuery>
#include <QString>

class CameraManager {
public:
    CameraManager();
    ~CameraManager();

    bool addCamera(const QString& url, const QString& room);
    bool removeCamera(const QString& url);

    const std::unordered_set<std::string>& getCameraUrls() const;

    QMap<QString, QString> getCameras() const;

    bool openStream(const std::string& url);
    cv::VideoCapture* getCapture(const std::string& url);

private:
    std::unordered_set<std::string> cameraUrls;
    std::unordered_map<std::string, cv::VideoCapture*> streams;
};
