#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

  private slots:
    void on_actionExit_triggered();

    void on_pushButton_count_clicked();

    void on_actionSelect_data_folder_triggered();

  private:
    Ui::MainWindow *ui;

  protected:
    QString dataDirPath;

    QString getDataDirPath();
};

#endif // MAINWINDOW_H
