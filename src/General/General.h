#pragma once

#include <QFont>
#include <QString>
#include <unordered_map>

static std::unordered_map<QString, QString> CameraRoomMap{
    {QStringLiteral("http://admin:0gfhjkm@192.168.6.62/video2.mjpg"), QStringLiteral("Аудитория 408")},
    {QStringLiteral("http://admin:0gfhjkm@192.168.6.61/video2.mjpg"), QStringLiteral("Аудитория 401")}};
