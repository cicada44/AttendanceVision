#include "CamerasTab.h"
#include "../General/General.h"

CamerasTab::CamerasTab(CameraController* controller, QWidget* parent) : QWidget(parent), controller(controller) {
    setupUI();
}

void CamerasTab::setupUI() {
    gridLayout = new QGridLayout(this);
    setLayout(gridLayout);
    fillCamerasLayout(gridLayout);
}

void CamerasTab::fillCamerasLayout(QGridLayout* layout) {
    layout->setSpacing(8);
    layout->setAlignment(Qt::AlignTop);

    int row = 0, col = 0;
    auto cameras = controller->getCameras();  // Получаем карту <url, room>

    for (const auto& [url, room] : cameras.toStdMap()) {
        auto* btn = new QPushButton(room, this);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        btn->setFont(QFont("", 24, QFont::Bold));
        layout->addWidget(btn, row, col);

        connect(btn, &QPushButton::clicked, this, [=]() { controller->handleCameraClicked(url.toStdString()); });

        btn->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(btn, &QPushButton::customContextMenuRequested, this, [=](const QPoint&) {
            QMenu menu;
            menu.addAction("Удалить", [=]() {
                if (controller->removeCamera(url)) {
                    QMessageBox::information(this, "Удалено", "Камера удалена.");
                    refresh();
                } else {
                    QMessageBox::warning(this, "Ошибка", "Не удалось удалить камеру.");
                }
            });
            menu.exec(QCursor::pos());
        });

        if (++col >= 2) {
            col = 0;
            ++row;
        }
    }

    auto* newCameraBtn = new QPushButton("+", this);
    newCameraBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    newCameraBtn->setFont(QFont("", 24, QFont::Bold));
    layout->addWidget(newCameraBtn, row, col);

    connect(newCameraBtn, &QPushButton::clicked, this, [=]() { addNewCamera(); });
}

void CamerasTab::refresh() {
    QLayoutItem* item;
    while ((item = gridLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    fillCamerasLayout(gridLayout);
}

void CamerasTab::addNewCamera() {
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить новую камеру");
    dialog.setFixedSize(400, 300);

    auto* layout = new QVBoxLayout(&dialog);
    auto* nameEdit = new QLineEdit();
    auto* urlEdit = new QLineEdit();
    auto* ratioSpin = new QDoubleSpinBox();

    nameEdit->setPlaceholderText("Аудитория (например, 401)");
    urlEdit->setPlaceholderText("URL (например, rtsp://admin:pass@192.168.1.100/stream)");

    ratioSpin->setRange(0.0, 1.0);
    ratioSpin->setSingleStep(0.01);
    ratioSpin->setDecimals(2);
    ratioSpin->setValue(0.25);  // значение по умолчанию

    auto* nameLabel = new QLabel("Название аудитории:");
    auto* urlLabel = new QLabel("Ссылка на камеру:");
    auto* ratioLabel = new QLabel("line_y_ratio (0.0–1.0):");

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);
    layout->addWidget(urlLabel);
    layout->addWidget(urlEdit);
    layout->addWidget(ratioLabel);
    layout->addWidget(ratioSpin);

    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        QString url = urlEdit->text().trimmed();
        QString room = nameEdit->text().trimmed();
        double ratio = ratioSpin->value();

        if (url.isEmpty() || room.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Поля не должны быть пустыми");
            return;
        }
        if (controller->addCamera(url, room, ratio)) {
            dialog.accept();
            refresh();
        } else {
            QMessageBox::warning(this, "Ошибка", "Камера с таким URL уже существует или добавление не удалось.");
        }
    });

    connect(buttonBox, &QDialogButtonBox::rejected, [&]() { dialog.reject(); });

    dialog.exec();
}
