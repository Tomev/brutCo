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
  qDebug() << "Columns in order size: " << getColumnsNamesInOrder();

  qDebug() << "Given trains data rows: " << getGivenTrainsKilometersData();

  qDebug() << "Trains data rows: " << getTrainsData();

  qDebug() << "Updateds columns in order size: "
           << addAdditionalColumnsToTrainData();

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
  QFile trainsFile(dataDirPath + "/trains.csv");

  if(!trainsFile.open(QIODevice::ReadOnly))
  {
    qDebug() << "Data file cannot be open for reading.";
    return -1;
  }

  QTextStream textStream(&trainsFile);

  // First line contains headers, it should be omitted
  textStream.readLine();

  QStringList trainData;

  trains.clear();

  while(true)
  {
    if(textStream.atEnd()) return trains.size();

    trains.append(std::unordered_map<std::string, std::string>());

    QString line = textStream.readLine();

    trainData = line.split(",");

    for(int i = 0; i < columnsNamesInOrder.size(); ++i)
      trains.last()[columnsNamesInOrder.at(i).toStdString()] = trainData.at(i).toStdString();
  }
}

int bruttoCountingThread::addAdditionalColumnsToTrainData()
{
  QStringList additionalColumnsNames =
  {
    "km ROJ", "km RPJ", "km pozostałe", "km ogółem", "brtkm ROJ", "brtkm RPJ",
    "brtkm pozostałe", "brtkm ogółem"
  };

  // Insert after column km ogółem
  int newColumnsShift = columnsNamesInOrder.indexOf("Km ogółem") + 1;

  // Add from the back, so shift doesnt change
  while(additionalColumnsNames.size() > 0)
  {
    columnsNamesInOrder.insert(newColumnsShift, additionalColumnsNames.last());
    additionalColumnsNames.pop_back();
  }

  return columnsNamesInOrder.size();
}

