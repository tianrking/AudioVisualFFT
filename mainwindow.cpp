#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化场景和视图
    scene = new QGraphicsScene(this);
    auto *view = new QGraphicsView(scene, this); // 确保这里使用了scene初始化view
    setCentralWidget(view); // 将视图设置为中心部件

    // 调整场景大小
    scene->setSceneRect(0, 0, 800, 600);

    // 初始化定时器并连接到更新槽函数
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSpectrum);
    timer->start(100); // 每100毫秒更新一次

    // 确保视图能够显示整个场景
    view->setSceneRect(scene->sceneRect());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateSpectrum()
{
    scene->clear(); // 清除旧的柱状图以便绘制新的

    const int spectrumSize = 30; // 假设有30个频率成分
    for (int i = 0; i < spectrumSize; ++i) {
        int amplitude = QRandomGenerator::global()->bounded(100) + 1;

        // 计算当前柱状图的颜色
        double hue = (double)i / spectrumSize; // 将色调映射到0.0 - 1.0之间
        QColor color = QColor::fromHsvF(hue, 1.0, 1.0); // 饱和度和亮度都设为最大值

        // 使用计算出的颜色绘制柱状图
        QGraphicsRectItem *rect = scene->addRect(i * 26, 600 - amplitude, 25, amplitude, QPen(Qt::NoPen), color);
        rect->setBrush(color);
    }
}

