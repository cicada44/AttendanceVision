#pragma once

#include <QFont>
#include <QString>
#include <QtSql/QtSql>
#include <unordered_map>

static std::unordered_map<QString, QString> CameraRoomMap{};

inline bool initDatabaseConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\cicada-main\\sibsutis\\graduate\\AttendanceVision\\data\\attendance.db");

    if (!db.open()) {
        qDebug() << "Unable to open DB\n";
        return false;
    }

    return true;
}
