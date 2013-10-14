#ifndef TEXTFINDER_H
#define TEXTFINDER_H

#include <QWidget>

namespace Ui {
    class TextFInder;
}

class TextFInder : public QWidget
{
    Q_OBJECT

public:
    explicit TextFInder(QWidget *parent = 0);
    ~TextFInder();

private:
    Ui::TextFInder *ui;
    void loadTextFile();

private slots:
    void on_pushButton_clicked();
};

#endif // TEXTFINDER_H
