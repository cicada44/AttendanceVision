#include "MainWindow.h"
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QVBoxLayout>

#include "CustomWidgets/VTabWidget.h"
#include "General/General.h"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    cameraManager = new CameraManager();
    cameraController = new CameraController(this);
    cameraController->setModel(cameraManager);

    camerasTab = new CamerasTab(cameraController, this);

    daemonController = new DaemonController(this);
    daemonTab = new DaemonTab(daemonController, this);

    const QString dbPath = QCoreApplication::applicationDirPath() + "/data/attendance.db";
    statsRepo = new StatsRepository(dbPath);
    statsController = new StatisticsController(statsRepo, this);
    statisticsTab = new StatisticsTab(statsController, this);

    auto sideBar = new VTabWidget();
    sideBar->addTab(camerasTab, "Cameras");
    sideBar->addTab(daemonTab, "Daemon");
    sideBar->addTab(statisticsTab, tr("Statistics"));

    // Основной вертикальный layout
    auto mainLayout = new QVBoxLayout(this);

    // Центральное содержимое
    auto centerLayout = new QHBoxLayout();
    centerLayout->addWidget(sideBar);
    mainLayout->addLayout(centerLayout);

    // Статус-бар
    timeLabel = new QLabel(this);
    networkLabel = new QLabel(this);

    auto statusLayout = new QHBoxLayout();
    statusLayout->addWidget(networkLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(timeLabel);

    auto statusBar = new QWidget(this);
    statusBar->setLayout(statusLayout);
    statusBar->setFixedHeight(45);
    statusBar->setStyleSheet("background: #ddd; padding: 2px; font-size: 12px;");

    mainLayout->addWidget(statusBar);
    setLayout(mainLayout);

    // Таймер обновления времени
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);

    // Таймер проверки сети
    networkManager = new QNetworkAccessManager(this);
    QTimer* netTimer = new QTimer(this);
    connect(netTimer, &QTimer::timeout, this, &MainWindow::checkNetworkStatus);
    netTimer->start(10000);

    updateTime();
    checkNetworkStatus();
}

void MainWindow::updateTime() {
    QString currentTime = QTime::currentTime().toString("hh:mm:ss");
    timeLabel->setText("Time: " + currentTime);
}

void MainWindow::checkNetworkStatus() {
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://zm.csc.sibsutis.ru/"));

    QNetworkReply* reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->bytesAvailable() > 0 || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).isValid()) {
            networkLabel->setText("Status: Online");
            networkLabel->setStyleSheet("color: green");
        } else {
            networkLabel->setText("Status: Offline");
            networkLabel->setStyleSheet("color: red");
        }
        reply->deleteLater();
    });
}
