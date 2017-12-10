#include "bruttoCountingThread.h"

#include <QThread>
#include <QDebug>
#include <QFile>
#include <QTextStream>

bruttoCountingThread::bruttoCountingThread(QString dataDirPath)
{
  this->dataDirPath = dataDirPath;
}

void bruttoCountingThread::run()
{
  getColumnsNamesInOrder();

  qDebug() << getGivenTrainsKilometersData();

  // Parse trains data

}

int bruttoCountingThread::getColumnsNamesInOrder()
{
  QFile trainsFile(dataDirPath + "/trains.csv");

  if(!trainsFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Data file cannot be open for reading.";
    return -1;
  }

  columnsNamesInOrder.clear();

  QTextStream textStream(&trainsFile);

  QString line = textStream.readLine();

  columnsNamesInOrder = line.split(",");

  return columnsNamesInOrder.size();
}

int bruttoCountingThread::getGivenTrainsKilometersData()
{
  QFile dataFile(dataDirPath + "/data.csv");

  if(!dataFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Data file cannot be open for reading.";
    return -1;
  }

  QTextStream textStream(&dataFile);
  QStringList givenDatum;
  QStringList dataHeadersInOrder = {"Nr pociągu", "km ROJ", "km RPJ"};

  givenData.clear();

  while(!textStream.atEnd())
  {
    givenData.append(std::unordered_map<std::string, std::string>());
    givenDatum = textStream.readLine().split(",");

    for(int i = 0; i < givenDatum.size(); ++i)
      givenData.last()[dataHeadersInOrder.at(i).toStdString()] = givenDatum.at(i).toStdString();
  }

  return givenData.size();
}

int bruttoCountingThread::getTrainsData()
{

}

