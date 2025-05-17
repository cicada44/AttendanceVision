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

bool CameraManager::addCamera(const QString& url, const QString& name, double lineYRatio) {
    QSqlQuery query;
    query.prepare("INSERT INTO cameras (url, name, line_y_ratio) VALUES (:url, :name, :ratio)");
    query.bindValue(":url", url);
    query.bindValue(":name", name);
    query.bindValue(":ratio", lineYRatio);
    if (!query.exec()) return false;

    cameraUrls.insert(url.toStdString());
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

double CameraManager::getLineYRatio(const QString& url) const {
    QSqlQuery query;
    query.prepare("SELECT line_y_ratio FROM cameras WHERE url = :url");
    query.bindValue(":url", url);
    if (!query.exec() || !query.next()) return 0.25;
    return query.value(0).toDouble();
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
