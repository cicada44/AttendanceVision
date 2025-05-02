#include "CamerasTab.h"
#include <QGridLayout>
#include "../General/General.h"

CamerasTab::CamerasTab(CameraController* controller, QWidget* parent) : QWidget(parent), controller(controller) {
    setupUI();
}

void CamerasTab::setupUI() {
    auto layout = new QGridLayout(this);
    fillCamerasLayout(layout);
    setLayout(layout);
}

void CamerasTab::fillCamerasLayout(QGridLayout* layout) {
    int row = 0, col = 0;
    for (const auto& url : CameraRoomMap) {
        QString cameraUrl = url.first;
        QString roomNumber = url.second;

        auto* btn = new QPushButton(roomNumber, this);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setFont(QFont("", 24, QFont::Bold));
        layout->addWidget(btn, row, col);

        connect(btn, &QPushButton::clicked, this,
                [=, this]() { controller->handleCameraClicked(url.first.toStdString()); });

        if (++col >= 2) {
            col = 0;
            ++row;
        }
    }

    auto newCameraBtn = new QPushButton("+", this);
    newCameraBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    newCameraBtn->setFont(QFont("", 24, QFont::Bold));
    connect(newCameraBtn, &QPushButton::clicked, this, [=, this]() { addNewCamera(); });
    layout->addWidget(newCameraBtn, row, col);
}

void CamerasTab::addNewCamera() {
    auto addNewCamDialog = new QDialog(this);

    auto l = new QVBoxLayout(addNewCamDialog);

    auto editName = new QLineEdit();
    addNewCamDialog->accept();
    editName->setPlaceholderText("New camera name");

    auto editUrl = new QLineEdit();
    editUrl->setPlaceholderText("New url (http://admin:0gfhjkm)...");

    auto nameLabel = new QLabel("Name"), urlLabel = new QLabel("URL");
    QFont f = font();
    f.setPixelSize(14);
    nameLabel->setFont(f);
    urlLabel->setFont(f);

    auto apply = new QPushButton("Apply");
    connect(apply, &QPushButton::clicked, [=, this]() {
        if (CameraRoomMap.contains(editUrl->text()) || editUrl->text().isEmpty() || editName->text().isEmpty()) {
            QMessageBox::information(this, "Information", "Camera already added");
        } else {
            CameraRoomMap[editUrl->text()] = editName->text();
            addNewCamDialog->accept();
        }
    });

    auto cancel = new QPushButton("Cancel");
    connect(cancel, &QPushButton::clicked, [=, this]() { addNewCamDialog->reject(); });

    l->setSpacing(0);
    l->addWidget(nameLabel);
    l->addWidget(editName);
    l->addSpacing(4);
    l->addWidget(urlLabel);
    l->addWidget(editUrl);
    l->addSpacing(4);

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(cancel);
    buttonLayout->addWidget(apply);

    l->addSpacing(5);
    l->addLayout(buttonLayout);

    addNewCamDialog->setLayout(l);
    addNewCamDialog->exec();
    addNewCamDialog->setFixedSize(400, 300);
}
