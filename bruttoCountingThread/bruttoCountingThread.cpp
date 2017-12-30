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

  qDebug() << "Updated columns in order size: "
           << addAdditionalColumnsToTrainData();

  qDebug() << "Updated trains number: " << updateGivenTrainsData();

  qDebug() << "Resultant csv created. Lines " << generateResultantCSV();;
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

  columnsNamesInOrder = line.split("|");

  trainsFile.close();

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

  dataFile.close();

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

  //int j = 0;

  while(true)
  {
    if(textStream.atEnd())
    {
      trainsFile.close();
      return trains.size();
    }

    trains.append(std::unordered_map<std::string, std::string>());

    QString line = textStream.readLine();

    trainData = line.split("|");

    //qDebug() << ++j;

    for(int i = 0; i < columnsNamesInOrder.size(); ++i)
      trains.last()[columnsNamesInOrder.at(i).toStdString()] = trainData.at(i).toStdString();
  }

  trainsFile.close();

  return -1;
}

int bruttoCountingThread::addAdditionalColumnsToTrainData()
{
  QStringList additionalColumnsNames =
  {
    "km ROJ", "km RPJ", "km pozostałe", "km ogółem r", "brtkm ROJ", "brtkm RPJ",
    "brtkm pozostałe", "brtkm ogółem"
  };

  // Insert after column km ogółem
  int newColumnsShift = columnsNamesInOrder.indexOf("'Km ogółem'") + 1;

  // Add from the back, so shift doesnt change
  while(additionalColumnsNames.size() > 0)
  {
    columnsNamesInOrder.insert(newColumnsShift, additionalColumnsNames.last());
    additionalColumnsNames.pop_back();
  }

  return columnsNamesInOrder.size();
}

int bruttoCountingThread::updateGivenTrainsData()
{
  int updatedTrainsNumber = 0;

  QStringList givenTrainsList = getGivenTrainsNames();

  for(int i = 0; i < trains.size(); ++i)
  {
    if(givenTrainsList.contains(QString::fromStdString(trains[i]["'Nr pociągu'"])))
    {
      updateTrainData(&(trains[i]));
      ++updatedTrainsNumber;     
    }
  }


  return updatedTrainsNumber;
}

QStringList bruttoCountingThread::getGivenTrainsNames()
{
  QStringList givenTrainsList;

  for(std::unordered_map<std::string, std::string> train: givenData)
    givenTrainsList.append(QString::fromStdString(train["Nr pociągu"]));

  return givenTrainsList;
}

int bruttoCountingThread::updateTrainData(std::unordered_map<std::__cxx11::string, std::__cxx11::string> *train)
{
  std::unordered_map<std::__cxx11::string, std::__cxx11::string>
      trainsGivenData;

  // Find given data for train
  for(std::unordered_map<std::__cxx11::string, std::__cxx11::string> data : givenData)
  {
    if(data["Nr pociągu"] == (*train)["'Nr pociągu'"])
    {
      trainsGivenData = data;
      break;
    }
  }

  //train->insert("km ROJ", trainsGivenData["km ROJ"]);
  (*train)["km ROJ"] = (trainsGivenData)["km ROJ"];
  (*train)["km RPJ"] = (trainsGivenData)["km RPJ"];
  (*train)["km pozostałe"] = "0";

  double km = std::stod((*train)["km ROJ"]) + std::stod((*train)["km RPJ"]);

  (*train)["km ogółem r"] = std::to_string(km);

  if(std::stod((*train)["km ogółem r"]) - std::stod((*train)["'Km ogółem'"]) < 0.0001)
    countBruttos(train);

  return 0;
}

int bruttoCountingThread::countBruttos(std::unordered_map<std::__cxx11::string, std::__cxx11::string> *train)
{

  double  bruttoRzecz = std::stod((*train)["'Brutto Rzecz'"]),
          kmROJ       = std::stod((*train)["km ROJ"]),
          kmRPJ       = std::stod((*train)["km RPJ"]),
          kmTotal     = std::stod((*train)["'Km ogółem'"]);

  (*train)["brtkm ROJ"] = std::to_string(bruttoRzecz * kmROJ);
  (*train)["brtkm RPJ"] = std::to_string(bruttoRzecz * kmRPJ);
  (*train)["brtkm pozostałe"] = "0";
  (*train)["brtkm ogółem"] = std::to_string(bruttoRzecz * kmTotal);

  return 0;
}

int bruttoCountingThread::generateResultantCSV()
{
  QFile result(dataDirPath + "/result.csv");

  if(!result.open(QIODevice::WriteOnly))
  {
    qDebug() << "Couldn't open file for writing.";
    return -1;
  }

  int linesNumber = 0;
  QString line = generateHeaderLine();
  QTextStream textStream(&result);

  textStream << line << "\n";

  for(std::unordered_map<std::__cxx11::string, std::__cxx11::string> train: trains)
  {
    line = generateLineFromTrain(&train);
    textStream << line << "\n";
    ++linesNumber;
  }

  return linesNumber;
}

QString bruttoCountingThread::generateHeaderLine()
{
  QString result = "";

  for(QString column: columnsNamesInOrder)
    result += column + "|";

  result.remove(result.length()-1, 1);

  return result;
}

QString bruttoCountingThread::generateLineFromTrain(std::unordered_map<std::__cxx11::string, std::__cxx11::string> *train)
{
  QString line = "";

  for(QString header: columnsNamesInOrder)
  {
    line += QString::fromStdString((*train)[header.toStdString()]) + "|";
  }

  line.remove(line.length()-1, 1);

  return line;
}

void bruttoCountingThread::trainsUpdate(std::unordered_map<std::__cxx11::string, std::__cxx11::string> *train)
{
  (*train)["dupa"] = "biskupa";
}

