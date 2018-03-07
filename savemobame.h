#ifndef SAVEMOBAME_H
#define SAVEMOBAME_H

#include "image.h"

#include <QMainWindow>
#include <QClipboard>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QString>
#include <QDir>
#include <QUrl>
#include <QRect>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QDragEnterEvent>
#include <QList>
#include <QMimeData>
#include <QAbstractItemView>
#include <QTableView>
#include <QTextStream>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QDir>

namespace Ui {
class SaveMobame;
}

class SaveMobame : public QMainWindow
{
    Q_OBJECT

public:
    explicit SaveMobame(QWidget *parent = 0);
    ~SaveMobame();
    void ChangeLineFormat(QString & text);
    QString GetFileFormat(QString &path);
    void LoadIni();
    void RunBoard();
    QString AnalyzeMailDate(QString &mail);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private slots:
    void Exit();
    void Save();
    void SetImageName();
    void StartBoard();
    void RenameImage();
    void on_exitBt_clicked();
    void on_saveBt_clicked();
    void on_saveEdit_editingFinished();
    void on_wordEdit_editingFinished();
    void on_lockedcheckBox_stateChanged(int arg1);
    void on_autosavecheckBox_stateChanged(int arg1);
    void on_dirBt_clicked();

private:
    Ui::SaveMobame *ui;
    QClipboard *board;
    QString maildate;
    QTimer *mtimer;
    unsigned int index;
};

#endif // SAVEMOBAME_H
