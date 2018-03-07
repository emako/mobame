#include "savemobame.h"
#include "ui_savemobame.h"
#include <QDebug>

SaveMobame::SaveMobame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SaveMobame)
{
    ui->setupUi(this);
    this->setWindowOpacity(0.99);
    LoadIni();
    board = QApplication::clipboard();
    connect(board, SIGNAL(dataChanged()), SLOT(StartBoard()));
    RunBoard();
    index = 1;

    ui->actionSave->setShortcuts(QKeySequence::Save);
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save()));
    ui->actionExit->setShortcuts(QKeySequence::Close);
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(Exit()));
    ui->actionImage->setShortcuts(QKeySequence::UnknownKey);
    connect(ui->actionImage, SIGNAL(triggered()), this, SLOT(SetImageName()));
}

SaveMobame::~SaveMobame()
{
    delete ui;
}

void SaveMobame::ChangeLineFormat(QString & text)
{
    while(text.indexOf("\n") != -1) {
    text.replace(text.indexOf("\n"), 1, "^M");
    }
    while(text.indexOf("^M") != -1) {
    text.replace(text.indexOf("^M"), 2, "\r\n");
    }
}

QString SaveMobame::GetFileFormat(QString &path)
{
    QString format;
    if(path.indexOf(".") != -1){
        int i = 1;
        while(1){
            QString tmp = path.right(i);
            if(tmp.indexOf(".") != -1)
                break;
            i++;
        }
        format = path.right(i);
    }
    return format;
}

void SaveMobame::LoadIni()
{
    QSettings *configIniRead = new QSettings("configure.ini", QSettings::IniFormat);
    QString dirStr = configIniRead->value("/SaveMobame/dir").toString();
    QString wordStr = configIniRead->value("/SaveMobame/word").toString();

    if(configIniRead->value("/SaveMobame/image").toString().isEmpty()){
        QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
        configIniWrite->setValue("/SaveMobame/image", "img2");
        delete configIniWrite;
    }
    delete configIniRead;
    if(!wordStr.isEmpty()){
        ui->lockedcheckBox->setChecked(true);
    }
    if(dirStr.isEmpty()){
        dirStr = "C:/Users/Administrator/Desktop";
    }
    ui->saveEdit->setText(dirStr);
    ui->wordEdit->setText(wordStr);
}

void SaveMobame::on_exitBt_clicked()
{
    Exit();
}

void SaveMobame::Exit()
{
    this->close();
}

void SaveMobame::on_saveBt_clicked()
{
    Save();
}

void SaveMobame::Save()
{
    QString mail = board->text();
    if(!ui->wordEdit->text().isEmpty()){
        if(mail.indexOf(ui->wordEdit->text()) == -1)
            return;
    }
    ui->mailtextEdit->setText(mail);

    maildate = AnalyzeMailDate(mail);

    QString path = ui->saveEdit->text();
    if(path.isEmpty())
        return;
    if(path.right(1) != "/")
        path += "/";
    path += maildate + ".txt";
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    QString text = ui->mailtextEdit->toPlainText();
    ChangeLineFormat(text);
    out << text;
    file.close();
    mtimer = new QTimer(this);
    connect(mtimer,SIGNAL(timeout()),this,SLOT(RenameImage()));
    mtimer->start(500);
}

void SaveMobame::RenameImage()
{
    QSettings *configIniRead = new QSettings("configure.ini", QSettings::IniFormat);
    QString image = configIniRead->value("/SaveMobame/image").toString();
    QDir *dir = new QDir(ui->saveEdit->text());
    QStringList filter;
    filter << image + "*.jpg" << image + "*.png" << image + "*.jpeg";
    dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));

    if(fileInfo->count() == 0){
        mtimer->stop();
        disconnect(mtimer, 0, 0, 0);
    }

    for(int i=0; i < fileInfo->count(); i++){
        QString oldname = fileInfo->at(i).filePath();//fileName()
        QString newname = oldname.replace(oldname.indexOf(image), image.length(), maildate);
        oldname = fileInfo->at(i).filePath();
        bool state = QFile::rename(oldname, newname);
        if(state == false){
            QString format = GetFileFormat(newname);
            newname.remove(format);
            newname += " (" + QString::number(++index) + ")" + format;
            QFile::rename(oldname, newname);
        }
        qDebug()<<"oldname="+oldname;
        qDebug()<<"newname="+newname;
    }
    delete configIniRead;
    delete dir;
    delete fileInfo;
}

void SaveMobame::on_saveEdit_editingFinished()
{
    QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
    configIniWrite->setValue("/SaveMobame/dir", ui->saveEdit->text());
    delete configIniWrite;
}

void SaveMobame::on_wordEdit_editingFinished()
{
    QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
    configIniWrite->setValue("/SaveMobame/word", ui->wordEdit->text());
    delete configIniWrite;
}

void SaveMobame::on_lockedcheckBox_stateChanged(int arg1)
{
    if(arg1){
        ui->wordEdit->setEnabled(false);
    }
    else{
        ui->wordEdit->setEnabled(true);
    }
}

void SaveMobame::StartBoard()
{
    index = 1;
    Save();
}

void SaveMobame::RunBoard()
{
    ui->autosavecheckBox->setChecked(true);
}

void SaveMobame::on_autosavecheckBox_stateChanged(int arg1)
{
    if(arg1){
        connect(board, SIGNAL(dataChanged()), SLOT(StartBoard()));
        ui->saveBt->setText("Running");
        ui->saveBt->setEnabled(false);
    }
    else{
        disconnect(board, 0, 0, 0);
        ui->saveBt->setText("Save");
        ui->saveBt->setEnabled(true);
    }
}

QString SaveMobame::AnalyzeMailDate(QString &mail)
{
    if(mail.indexOf("\u65E5\u6642") == -1){
        QString ymd = QDateTime::currentDateTime().toString("yyMMdd");
        QString hms = QTime::currentTime().toString("hhmmss");
        return ymd + " " + hms + " New";
    }
    else{
        while(mail.indexOf("\n\n") != -1) {
            mail.replace(mail.indexOf("\n\n"), 2, "\n");
        }
        //qDebug()<<mail;
        //日時: 2017年1月25日 21:17:05 JST
        //日時: 2017年9月12日 GMT+921:44:28
        QString date = mail.mid(mail.indexOf("\u65E5\u6642"));
        if(mail.indexOf("JST\n") != -1){
            date = mail.mid(mail.indexOf("\u65E5\u6642"), mail.indexOf("JST\n")-mail.indexOf("\u65E5\u6642"));
        }
        else if(mail.indexOf("GMT") != -1){
            date = mail.mid(mail.indexOf("\u65E5\u6642"), mail.indexOf("\n\u4EF6\u540D")-mail.indexOf("\u65E5\u6642"));
            date.remove(date.indexOf("GMT"), QString("GMT").length() + 2);
            qDebug()<<date;
        }
        date.remove(":");
        date = date.simplified();
        date.remove(0, 5);
        if(date.indexOf("\u5E74") != -1) {
            date.replace(date.indexOf("\u5E74"), QString("\u5E74").size(), ".");
        }
        if(date.indexOf("\u6708") != -1) {
            date.replace(date.indexOf("\u6708"), QString("\u6708").size(), ".");
        }
        if(date.indexOf("\u65E5") != -1) {
            date.replace(date.indexOf("\u65E5"), QString("\u65E5").size(), ".");
        }
        QString y = date.mid(0, date.indexOf("."));
        date.remove(0, date.indexOf(".")+1);
        if(y.size() == 1)
            y = "0" + y;
        QString m = date.mid(0, date.indexOf("."));
        date.remove(0, date.indexOf(".")+1);
        if(m.size() == 1)
            m = "0" + m;
        QString d = date.mid(0, date.indexOf("."));
        date.remove(0, date.indexOf(".")+1);
        if(d.size() == 1)
            d = "0" + d;
        QString hms = date.simplified();
        if(hms.size() == 5)
            hms = "0" + hms;
        return y + m + d + " " + hms;
    }
}

void SaveMobame::on_dirBt_clicked()
{
    QString path = QFileDialog::getExistingDirectory(NULL, tr("Select a Dir"), "", QFileDialog::ShowDirsOnly);
    if(path.isEmpty()){
        return;
    }
    ui->saveEdit->setText(path);
    QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
    configIniWrite->setValue("/SaveMobame/dir", path);
    delete configIniWrite;
}

void SaveMobame::SetImageName()
{
    Image *image_win = new Image();
    image_win->show();
}

void SaveMobame::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasText())
        event->acceptProposedAction();
}

void SaveMobame::dropEvent(QDropEvent* event)
{
    const QMimeData* mime = event->mimeData();
    if(mime->hasUrls()){
        foreach(QUrl url, event->mimeData()->urls()){
            QString path = url.toString();
            path.remove("file:///");
            ui->saveEdit->setText(path);
            QSettings *configIniWrite = new QSettings("configure.ini", QSettings::IniFormat);
            configIniWrite->setValue("/SaveMobame/dir", path);
            delete configIniWrite;
        }
    }
    event->acceptProposedAction();
}
