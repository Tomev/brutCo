#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDebug"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionSelect_data_folder_triggered()
{
  QString dirPath = getDataDirPath();

  if(dirPath == "") dataDirPath = dirPath;
}

QString MainWindow::getDataDirPath()
{
  QString baseDir = "C:/",
          dataDirPath;

  // Get dir path of KBs folder
  dataDirPath = QFileDialog::getExistingDirectory
              (
                  this,
                  tr("Select directory"),
                  baseDir,
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
              );

  return dataDirPath;
}

void MainWindow::on_actionExit_triggered()
{
  qApp->exit();
}

void MainWindow::on_pushButton_count_clicked()
{

}
