#ifndef BRUTTOCOUNTINGTHREAD_H
#define BRUTTOCOUNTINGTHREAD_H

#include <memory>
#include <unordered_map>

#include <QThread>
#include <QStringList>
#include <QVector>


class bruttoCountingThread : public QThread
{
  public:
    bruttoCountingThread(QString dataDirPath);

    void run();

  protected:
    QString dataDirPath;
    QStringList columnsNamesInOrder;

    QVector<std::unordered_map<std::string, std::string>> trains;
    QVector<std::unordered_map<std::string, std::string>> givenData;

    int getColumnsNamesInOrder();
    int getGivenTrainsKilometersData();
    int getTrainsData();
    int addAdditionalColumnsToTrainData();
    int updateGivenTrainsData();
      QStringList getGivenTrainsNames();
      int updateTrainData(std::unordered_map<std::string, std::string> *train);
        int countBruttos(std::unordered_map<std::string, std::string> *train);
};

#endif // BRUTTOCOUNTINGTHREAD_H
