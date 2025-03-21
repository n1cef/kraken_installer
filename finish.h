#ifndef FINISH_H
#define FINISH_H

#include <QWidget>

namespace Ui {
class Finish;
}

class Finish : public QWidget
{
    Q_OBJECT

public:
    explicit Finish(QWidget *parent = nullptr);
    ~Finish();

private:
    Ui::Finish *ui;
};

#endif // FINISH_H
