#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    QPushButton * loadButton=new QPushButton("Load Image");
    ui->setupUi(this);
    this->setLayout(new QVBoxLayout());
    this->layout()->addWidget(loadButton);
    connect(loadButton,SIGNAL(clicked()),this,SLOT(loadImage()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadImage()
{
    QString filePath=QFileDialog::getOpenFileName();
    qDebug() << filePath;
    QImage image(filePath);
    QDir dir=QFileDialog::getExistingDirectory();
    ImageHandler::globalThresholdProcessing(image).save(dir.path()+"/GlobalThresholdProcessing.jpg");
    ImageHandler::adaptiveThresholdProcessing(image).save(dir.path()+"/AdaptiveThresholdProcessing.jpg");
    ImageHandler::linearContrast(image).save(dir.path()+"/LinearContrast.jpg");
}

