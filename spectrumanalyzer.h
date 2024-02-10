#ifndef SPECTRUMANALYZER_H
#define SPECTRUMANALYZER_H

#include <QWidget>
#include <QAudioInput>
#include <QByteArray>
#include <QGraphicsScene>
#include <fftw3.h>
#include <QDebug>
//class SpectrumAnalyzer : public QWidget
//{
//    Q_OBJECT

//public:
//    explicit SpectrumAnalyzer(QWidget *parent = nullptr);
//    ~SpectrumAnalyzer();

//    void analyze(const QByteArray& buffer);

//private:
//    QGraphicsScene* scene;
//    fftw_plan plan;
//    fftw_complex *in, *out;
//    int N;

//    void executeFFT();
//    void displaySpectrum();
//};


class SpectrumAnalyzer : public QWidget
{
    Q_OBJECT

public:
    explicit SpectrumAnalyzer(QWidget *parent = nullptr);
    ~SpectrumAnalyzer();

public slots:
    void analyze(const QByteArray& buffer);

private:
    QAudioInput* audioInput;
    QGraphicsScene* scene;
    fftw_plan plan;
    fftw_complex *in, *out;
    int N;

    void executeFFT();
    void displaySpectrum();
};


#endif // SPECTRUMANALYZER_H
