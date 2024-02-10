//#include "spectrumanalyzer.h"
//#include <QGraphicsView>
//#include <QVBoxLayout>
//#include <cmath>

//SpectrumAnalyzer::SpectrumAnalyzer(QWidget *parent) : QWidget(parent), N(1024)
//{
//    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
//    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
//    plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

//    auto* view = new QGraphicsView(this);
//    scene = new QGraphicsScene(this);
//    view->setScene(scene);

//    auto* layout = new QVBoxLayout(this);
//    layout->addWidget(view);
//    setLayout(layout);
//}

//SpectrumAnalyzer::~SpectrumAnalyzer()
//{
//    fftw_destroy_plan(plan);
//    fftw_free(in);
//    fftw_free(out);
//}

//void SpectrumAnalyzer::analyze(const QByteArray& buffer)
//{
//    // 填充in数组，简化处理，实际应根据音频格式正确转换
//    for(int i = 0; i < N; ++i) {
//        in[i][0] = (i < buffer.size()) ? (unsigned char)buffer[i] : 0.0;
//        in[i][1] = 0.0;
//    }

//    executeFFT();
//    displaySpectrum();
//}

//void SpectrumAnalyzer::executeFFT()
//{
//    fftw_execute(plan);
//}

//void SpectrumAnalyzer::displaySpectrum()
//{
//    scene->clear();
//    const double sceneWidth = scene->width();
//    double maxAmplitude = 0; // 查找最大幅度以进行归一化

//    // 首先计算最大幅度值
//    for (int i = 0; i < N / 2; ++i) {
//        double amplitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
//        if (amplitude > maxAmplitude) {
//            maxAmplitude = amplitude;
//        }
//    }

//    // 确保maxAmplitude不为零，避免除以零的情况
//    if (maxAmplitude == 0) return;

//    const double binWidth = sceneWidth / (N / 2);

//    for (int i = 0; i < N / 2; ++i) {
//        double amplitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
//        double normalizedAmplitude = amplitude / maxAmplitude;
//        double height = normalizedAmplitude * scene->height();

//        int colorIntensity = static_cast<int>(normalizedAmplitude * 255);
//        QColor barColor = QColor(colorIntensity, 0, 255 - colorIntensity);

//        scene->addRect(QRectF(i * binWidth, scene->height() - height, binWidth - 1, height), QPen(Qt::NoPen), barColor);
//    }
//}

#include "spectrumanalyzer.h"
#include <QAudioFormat>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <cmath>
#include <QAudioDeviceInfo>
#include <QDebug>

SpectrumAnalyzer::SpectrumAnalyzer(QWidget *parent) : QWidget(parent), N(1024)
{
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    auto* view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);

    // Set up audio format
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    // Audio input device
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format, this);
    QIODevice* device = audioInput->start();
    connect(device, &QIODevice::readyRead, [this, device]() {
        QByteArray buffer = device->readAll();
        analyze(buffer);
    });
}

SpectrumAnalyzer::~SpectrumAnalyzer()
{
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
    audioInput->stop();
    delete audioInput;
}

void SpectrumAnalyzer::analyze(const QByteArray& buffer)
{
    // Assuming buffer contains raw audio data according to the format specified in the constructor
    // This is a simplified example, actual implementation needs proper conversion
    int sampleCount = buffer.size() / 2; // 16-bit audio samples
    for(int i = 0; i < N && i < sampleCount; ++i) {
        // Convert bytes to FFTW input format
        in[i][0] = reinterpret_cast<const qint16*>(buffer.constData())[i];
        in[i][1] = 0.0;
    }

    executeFFT();
    displaySpectrum();
}

void SpectrumAnalyzer::executeFFT()
{
    fftw_execute(plan);
}

void SpectrumAnalyzer::displaySpectrum()
{
    scene->clear();
    const double sceneWidth = scene->width();
    const double binWidth = sceneWidth / (N / 2);

    for (int i = 0; i < N / 2; ++i) {
        double amplitude = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
        double normalizedAmplitude = amplitude / (1 << 15); // Normalizing for 16-bit audio
        double height = normalizedAmplitude * scene->height();

//        QColor color = QColor::fromHsvF(0.7 - (normalizedAmplitude * 0.7), 1.0, 1.0);
        double hue = qBound(0.0, 0.7 - (normalizedAmplitude * 0.7), 1.0);
        QColor color = QColor::fromHsvF(hue, 1.0, 1.0);

        scene->addRect(QRectF(i * binWidth, scene->height() - height, binWidth - 1, height), QPen(Qt::NoPen), color);
    }
}



