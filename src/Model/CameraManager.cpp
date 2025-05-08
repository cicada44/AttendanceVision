#include "CameraManager.h"

#include "../General/General.h"

CameraManager::CameraManager() {}

CameraManager::~CameraManager() {
    for (auto& [url, cap] : streams) {
        if (cap) {
            cap->release();
            delete cap;
        }
    }
}

bool CameraManager::addCamera(const QString& url, const QString& name) {
    QSqlQuery query;
    query.prepare("INSERT INTO cameras (url, name) VALUES (:url, :name)");
    query.bindValue(":url", url);
    query.bindValue(":name", name);
    if (!query.exec()) return false;

    cameraUrls.insert(url.toStdString());
    CameraRoomMap[url] = name;
    return true;
}

bool CameraManager::removeCamera(const QString& url) {
    QSqlQuery query;
    query.prepare("DELETE FROM cameras WHERE url = :url");
    query.bindValue(":url", url);
    if (!query.exec()) return false;

    std::string stdUrl = url.toStdString();
    cameraUrls.erase(stdUrl);

    auto it = streams.find(stdUrl);
    if (it != streams.end()) {
        it->second->release();
        delete it->second;
        streams.erase(it);
    }

    return true;
}

QMap<QString, QString> CameraManager::getCameras() const {
    QMap<QString, QString> cameraMap;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Ошибка: база данных не открыта в getCameras";
        return cameraMap;
    }

    // SELECT name, url FROM cameras
    QSqlQuery query("SELECT name, url FROM cameras", db);
    if (query.lastError().isValid()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return cameraMap;
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        QString url = query.value(1).toString();
        cameraMap.insert(url, name);
    }

    return cameraMap;
}

const std::unordered_set<std::string>& CameraManager::getCameraUrls() const { return cameraUrls; }

bool CameraManager::openStream(const std::string& url) {
    if (streams.find(url) == streams.end()) {
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
