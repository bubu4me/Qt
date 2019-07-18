#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    media(new QMediaPlayer(this)),
    fd(new QFileDialog(this)),
    dlgGrayscale(new QDialog(this))
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete fd;
    delete ui;
}

void MainWindow::init()
{
    // audio files settings
    fd->setWindowTitle(tr("Select Audio File to Open"));
    fd->setNameFilter("Audios(*.wav *.mp3)");
    fd->setFileMode(QFileDialog::ExistingFiles);
    fd->setViewMode(QFileDialog::Detail);

    // connect slots
    // 监听音乐播放的状态
    connect(media, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(updateState(QMediaPlayer::State)));
    connect(media, SIGNAL(positionChanged(qint64)), this, SLOT(updatePos(qint64)));
    connect(media, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
    connect(ui->playSlider, &QSlider::sliderMoved, media, &QMediaPlayer::setPosition);

    // UI settings
    ui->lineEdit_auidoPath->setReadOnly(true);
    ctrlBtn(false);
    QFile uiFile(":/qss/Qss7.qss");
    uiFile.open(QFile::ReadOnly);
    qApp->setStyleSheet(QString::fromLatin1(uiFile.readAll()));

}

// set action and button enable or not
void MainWindow::ctrlBtn(bool isEnabled)
{
    ui->button_load->setEnabled(isEnabled);
    ui->button_play->setEnabled(isEnabled);
    ui->button_save->setEnabled(isEnabled);
    ui->button_stop->setEnabled(isEnabled);

    ui->actionFFT->setEnabled(isEnabled);
    ui->actionOLA->setEnabled(isEnabled);
    ui->actionResample->setEnabled(isEnabled);
    ui->actionReverse->setEnabled(isEnabled);
    ui->actionFade->setEnabled(isEnabled);
    ui->actionAdjustByJson->setEnabled(isEnabled);
    ui->actionNormalization->setEnabled(isEnabled);
}

void MainWindow::on_button_browse_clicked()
{
    if (fd->exec()) {
        audioList = fd->selectedFiles();
        ui->lineEdit_auidoPath->setText(audioList[0]);
        audioInfo.setFile(audioList[0]);
        ctrlBtn(false);
        ui->label_audioName->setText(audioInfo.fileName());
        ui->button_load->setEnabled(true);
    }
}

void MainWindow::on_button_load_clicked()
{
    audioFile.load(audioList[0].toStdString());
    showLoadLog();
    media->setMedia(QUrl::fromLocalFile(audioList[0]));
    ui->label_audioTime->setText(getAudioTime(0) + "/" + getAudioTime(int(lengthInSeconds)));
    ctrlBtn(true);
}

void MainWindow::on_button_save_clicked()
{
    QString dirPath = fd->getSaveFileName(this, tr("Save Wav"), QString(audioInfo.fileName()), tr("wav"));

    if (!dirPath.isEmpty()) {
        audioFile.save(dirPath.toStdString(), AudioFileFormat::Wave);
        showLog(audioInfo.fileName() + " has been saved to " + dirPath);
    }
}

void MainWindow::on_button_play_clicked()
{
    if (ui->button_play->text() == "Play") {
        media->setMedia(nullptr);
        QString tempPath = QCoreApplication::applicationDirPath();
        tempPath.append("/temp.wav");
        audioFile.save(tempPath.toStdString());
        media->setMedia(QUrl::fromLocalFile(tempPath));
        media->play();
        ui->button_play->setText("Pause");
    }else if (ui->button_play->text() == "Pause") {
        media->pause();
        ui->button_play->setText("Resume");
    }else if (ui->button_play->text() == "Resume") {
        media->play();
        ui->button_play->setText("Pause");
    }
    ui->button_stop->setEnabled(true);
}

void MainWindow::on_button_stop_clicked()
{
    media->stop();
    ui->button_play->setText("Play");
    ui->button_play->setEnabled(true);
}

// 获取JSON的值
QJsonValue MainWindow::getJsonValue(QString key)
{
    QString jsonStr = ui->lineEdit_parameters->text();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonStr.toUtf8());

    return jsonDoc.object().value(key.toLower());
}

// 打印Load加载的详细信息
void MainWindow::showLoadLog()
{
    // get current time
    currentTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss ddd");

    // TODO show details about audio
    sampleRate = audioFile.getSampleRate();                         // 采样率
    channels = audioFile.getNumChannels();                          // 通道数
    bitDepth = audioFile.getBitDepth();                             // 位深
    samplesPerChannel = audioFile.getNumSamplesPerChannel();        // 每个通道的采样点
    lengthInSeconds = audioFile.getLengthInSeconds();               // 长度

    operationLogs
    .append(QString("[********* %1 *********]\n").arg(currentTime))
    .append("File Name           " + audioInfo.fileName() + '\n')
    .append("File Path           " + audioList[0] + '\n')
    .append("Sample Rate         " + QString::number(sampleRate) + '\n')
    .append("Channels            " + QString::number(channels) + '\n')
    .append("Bit Depth           " + QString::number(bitDepth) + '\n')
    .append("Samples Per Channel " + QString::number(samplesPerChannel) + '\n')
    .append("Duration            " + QString::number(lengthInSeconds) + '\n')
    .append("===========================================");
    ui->textEdit_log->append(operationLogs);
    operationLogs.clear();
}

// 打印操作日志
void MainWindow::showLog(QString log)
{
    currentTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss ddd");
    operationLogs
    .append("[********* ").append(currentTime).append(" *********]\n")
    .append(log + '\n');
    ui->textEdit_log->append(operationLogs);
    operationLogs.clear();
}

// Reverse
void MainWindow::on_actionReverse_triggered()
{
    qDebug() << "reverse" << endl;
    uint channels = uint(audioFile.getNumChannels());
    uint samplesPerChannel = uint(audioFile.getNumSamplesPerChannel());
    AudioFile<double>::AudioBuffer buffer;

    buffer.resize(channels);
    for (uint c = 0; c < channels; ++c) {
        buffer[c].resize(samplesPerChannel);
        for (uint s = 0; s < samplesPerChannel; ++s) {
            buffer[c][s] = audioFile.samples[c][samplesPerChannel - 1 - s];
        }
    }
    qDebug() << "size: " << buffer.size();
    if (audioFile.setAudioBuffer(buffer)) {
        showLog("Reverse");
    }
}

// Fade out
void MainWindow::on_actionFade_triggered()
{
    qDebug() << "fade out" << endl;
    double end = audioFile.getLengthInSeconds();
    double start = 0;

    if (end > 2) {
        start = end - 2;
    }

    QJsonValue jValueStart = getJsonValue("start");
    QJsonValue jValueEnd = getJsonValue("end");
    if (!jValueStart.isNull() && bool(jValueStart.toDouble())) {
        start = jValueStart.toDouble();
    }
    if (!jValueEnd.isNull() && bool(jValueEnd.toDouble())) {
        end = jValueEnd.toDouble();
    }
    if (start < 0) {
        start = 0;
    }
    if (end > audioFile.getLengthInSeconds()) {
        end = audioFile.getLengthInSeconds();
    }

    AudioFile<double>::AudioBuffer buffer;
    uint channels = audioFile.getNumChannels();
    uint samplePerChannel = uint(audioFile.getNumSamplesPerChannel());
    uint sampleRate = audioFile.getSampleRate();
    buffer.resize(channels);
    uint startPoint = uint(start * sampleRate);
    uint endPoint = uint(end * sampleRate);
    uint differ = endPoint - startPoint;
    for (uint c = 0; c < channels; ++c) {
        buffer[c].resize(samplePerChannel);
        for (uint s = 0; s < samplePerChannel; ++s) {
            if (s >= startPoint && s <= endPoint) {
                buffer[c][s] = audioFile.samples[c][s] * (endPoint - s) / differ;
            }else {
                buffer[c][s] = audioFile.samples[c][s];
            }
        }
    }
    if (audioFile.setAudioBuffer(buffer)) {
        showLog(QString("Fade out from %1s to %2s").arg(start).arg(end));
    }

}

// Resample
void MainWindow::on_actionResample_triggered()
{
    qDebug() << "resample" << endl;
    double rate = 1;
    QJsonValue jValue = getJsonValue("rate");

    if (!jValue.isNull() && bool(jValue.toDouble())) {
        rate = jValue.toDouble();
    }
    AudioFile<double>::AudioBuffer buffer;
    uint channels = audioFile.getNumChannels();
    int samplePerChannel = audioFile.getNumSamplesPerChannel();
    buffer.resize(channels);
    uint newSamplePerChannel = uint(samplePerChannel * rate);
    for (uint ss = 0; ss < channels; ++ss) {
        buffer[ss].resize(newSamplePerChannel);
        for (uint spc = 0; spc < newSamplePerChannel; ++spc) {
            buffer[ss][spc] = audioFile.samples[ss][uint(spc / rate)];
        }
    }
    if (audioFile.setAudioBuffer(buffer)) {
        showLog(QString("Resample with rate %1").arg(rate));
    }
}

// 获取汉宁窗
vector<double> MainWindow::getHanningWnd(int windowSize)
{
    vector<double> hanWnd;
    double PI = 3.1415926;

    for (int i = 0; i < windowSize; ++i) {
        hanWnd.push_back(0.5 - 0.5 * cos(2 * PI * i / windowSize));
    }
    return hanWnd;
}

// 进度条
QString MainWindow::getAudioTime(qint64 time)
{
    qint64 sec = time / 1000;
    qint64 min = sec / 60;

    sec -= min * 60;
    return QString("%1:%2").arg(min, 2, 10, QLatin1Char('0')).arg(sec, 2, 10, QLatin1Char('0'));
}

// media state
void MainWindow::updateState(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState) {
        ui->button_play->setText("Play");
    }else {
        qDebug() << "playing" << endl;
    }
}

void MainWindow::updatePos(qint64 position)
{
    ui->playSlider->setValue(position);
    ui->label_audioTime->setText(getAudioTime(position) + "/" + getAudioTime(media->duration()));
}

void MainWindow::updateDuration(qint64 duration)
{
    ui->playSlider->setRange(0, duration);
    ui->playSlider->setEnabled(duration > 0);
    ui->playSlider->setPageStep(duration / 10);
}

// fft
void MainWindow::on_actionFFT_triggered()
{
    showLog(QString("FFT\n").append("There are %1 grayscale image(s) to save").arg(channels));
    //计算图像的尺寸
    int N = sqrt(samplesPerChannel);

    if (sqrt(samplesPerChannel) > N) {
        N += 1; //希望图像的尺寸大于等于总点数
    }
    Matrix<double> mat1(N, N, 0);
    Matrix<double> mat2(N, N, 0);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i * N + j < samplesPerChannel) {
                mat1(i, j) = audioFile.samples[0][i * N + j];
                if (channels == 2) {
                    mat2(i, j) = audioFile.samples[1][i * N + j];
                }
            }
        }
    }
    fourierTransform(mat1, N, 0);
    if (channels == 2) {
        fourierTransform(mat2, N, 1);
    }
}

// 傅里叶变换
void MainWindow::fourierTransform(const Matrix<double>& mat, int N, int channel)
{
    Matrix<complex<double> > fftChannelMat = fft2d(mat, N, N);
    Matrix<double> fftRealMat(fftChannelMat.getNRow(), fftChannelMat.getNCol(), 0);

    int nRow = fftRealMat.getNRow();
    int nCol = fftRealMat.getNCol();

    for (int i = 0; i < nRow; i++) {
        for (int j = 0; j < nCol; j++) {
            fftRealMat(i, j) = log(1 + abs(fftChannelMat(i, j)));
        }
    }
    Matrix<int> resMat = nomarlized(fftRealMat, 0, 255);
    resMat.fftshift();
    QImage grayscaleImg = toQImage(resMat);
    saveMyImage(grayscaleImg, channel);
}

// 图像归一化
Matrix<int> MainWindow::nomarlized(Matrix<double> data, double min, double max)
{
    qDebug() << "fft Matrix<double> nomarlized" << endl;
    int maxValue = 0;
    int minValue = 999;

    size_t nRow = data.getNRow();
    size_t nCol = data.getNCol();

    Matrix<int> resData(nRow, nCol, 0);

    for (size_t i = 0; i < nRow; ++i) {
        for (size_t j = 0; j < nCol; ++j) {
            if (maxValue < data(i, j)) {
                maxValue = int(data(i, j));
            }
            if (minValue > data(i, j)) {
                minValue = int(data(i, j));
            }
        }
    }

    for (size_t i = 0; i < nRow; ++i) {
        for (size_t j = 0; j < nCol; ++j) {
            resData(i, j) = int((data(i, j) - minValue) * (max - min) / (maxValue - minValue));
        }
    }
    return resData;
}

// 保存图像
void MainWindow::saveMyImage(QImage img, int channel)
{
    // 保存图像
    QString imgPath = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("*.jpg;;*.png"));

    if (imgPath.isEmpty()) {
        return;
    }else {
        if (!(img.save(imgPath))) {
            showLog(QString("Failed to save grayscale image %1 ").arg(channel + 1).append(imgPath));
            return;
        }
        showLog(QString("Grayscale image %1 has been saved to ").arg(channel + 1).append(imgPath));
    }
}

// OLA
void MainWindow::on_actionOLA_triggered()
{
    qDebug() << "OLA" << endl;
    double factor = 1;
    QJsonValue jValue = getJsonValue("factor");

    if (!jValue.isNull() && bool(jValue.toDouble())) {
        factor = jValue.toDouble();
    }
    if (std::abs(factor - 1) < 0.0001) {
        qDebug() << "factor = 1" << endl;
        return;
    }

    int wndSz = sampleRate / 10;
    int anaStep = wndSz / 2;
    int synStep = int(anaStep * factor);
    int N = int(ceil((samplesPerChannel - wndSz) / anaStep)) + 1;

    double **win1 = new double*[N];
    double **win2 = new double*[N];
    for (int i = 0; i < N; i++) {
        win1[i] = new double[wndSz];
        win2[i] = new double[wndSz];
        for (int j = 0; j < wndSz; j++) {
            win1[i][j] = 0;
            win2[i][j] = 0;
        }
    }

    vector<double> hanningWnd = getHanningWnd(wndSz);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < wndSz; j++) {
            uint num = i * anaStep + j;
            win1[i][j] = audioFile.samples[0][num] * hanningWnd[j];
            if (channels == 2) {
                win2[i][j] = audioFile.samples[1][num] * hanningWnd[j];
            }
        }
    }
    AudioFile<double>::AudioBuffer buffer;
    buffer.resize(channels);
    uint newLen = wndSz + (N - 1) * synStep;
    buffer[0].resize(newLen);
    if (channels == 2) {
        buffer[1].resize(newLen);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < wndSz; j++) {
            uint num = i * synStep + j;
            buffer[0][num] = 0;
            buffer[0][num] += win1[i][j];
            if (channels == 2) {
                buffer[1][num] = 0;
                buffer[1][num] += win2[i][j];
            }
        }
    }
    audioFile.setAudioBuffer(buffer);
    showLog(QString("OLA with factor %1").arg(factor));
}

// 调整音量
void MainWindow::on_actionAdjustByJson_triggered()
{
    vector<double> vol;

    vol.resize(channels);
    for (int c = 0; c < channels; ++c) {
        vol[c] = 1;
    }
    QJsonValue jValue = getJsonValue("vol");

    if (!jValue.isNull() && jValue.toDouble()) {
        vol[0] = jValue.toDouble();
    }

    for (int c = 0; c < channels; ++c) {
        vol[c] = vol[0];
    }
    setVolumeInChannel(vol);
    showLog(QString("Volume is adjusted to %1").arg(int(vol[0] * 100)));
}

// Set Volume
void MainWindow::setVolumeInChannel(vector<double> volume)
{
    AudioFile<double>::AudioBuffer buffer;

    buffer.resize(channels);
    for (int c = 0; c < channels; ++c) {
        buffer[c].resize(samplesPerChannel);
        for (int i = 0; i < samplesPerChannel; ++i) {
            buffer[c][i] = audioFile.samples[c][i] * volume[c];
        }
    }
    audioFile.setAudioBuffer(buffer);
}

// 音量归一化
void MainWindow::on_actionNormalization_triggered()
{
    vector<double> maxVol;

    maxVol.resize(channels);
    for (int c = 0; c < channels; ++c) {
        maxVol[c] = 0;
        for (int i = 0; i < samplesPerChannel; ++i) {
            if (audioFile.samples[c][i] > maxVol[c]) {
                maxVol[c] = audioFile.samples[c][i];
            }
        }
    }
    setVolumeInChannel(maxVol);
    showLog("Volume is normalized");
}
