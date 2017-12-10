#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "./bruttoCountingThread/bruttoCountingThread.h"

#include <memory>

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

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

  if(containsNecessaryFiles(dirPath)) dataDirPath = dirPath;
}

QString MainWindow::getDataDirPath()
{
  QString baseDir = "C:/test",
          dataDirPath;

  // Get dir path to folder with data
  dataDirPath = QFileDialog::getExistingDirectory
              (
                  this,
                  tr("Select directory"),
                  baseDir,
                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
              );

  return dataDirPath;
}

bool MainWindow::containsNecessaryFiles(QString dirPath)
{
  QStringList necessaryFilesNames = {"trains.csv", "data.csv"};

  std::unique_ptr<QFileInfo> fileToCheck;

  QString filePath;

  for(QString fileName : necessaryFilesNames)
  {
    filePath = dirPath + "/" + fileName;
    fileToCheck.reset(new QFileInfo(filePath));

    if(!fileToCheck->isFile())
    {
      qDebug() << "Folder does not contain " + fileName + ".";
      return false;
    }
  }

  return true;
}

void MainWindow::on_actionExit_triggered()
{
  qApp->exit();
}

void MainWindow::on_pushButton_count_clicked()
{
  std::unique_ptr<bruttoCountingThread>
      bct(new bruttoCountingThread(dataDirPath));

  bct->run();
}
