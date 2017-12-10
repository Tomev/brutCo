#ifndef BRUTTOCOUNTINGTHREAD_H
#define BRUTTOCOUNTINGTHREAD_H

#include <QThread>

class bruttoCountingThread : public QThread
{
  public:
    bruttoCountingThread();

    void run();
};

#endif // BRUTTOCOUNTINGTHREAD_H
