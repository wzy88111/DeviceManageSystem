#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTableWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

typedef struct DeviceInfTag
{
    DeviceInfTag()
        : id("")
        , type("")
        , name("")
        , buy_time("")
        , is_scrap(false)
        , scrap_time("")
        {}
    QString id;
    QString type;
    QString name;
    QString buy_time;
    bool is_scrap;
    QString scrap_time;
}DeviceInf;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void init();

private slots:
    void on_m_btn_query_clicked();

    void on_m_btn_add_clicked();

    void on_m_btn_update_clicked();

    void on_m_btn_del_clicked();

    void on_m_ok_clicked();

    void on_m_table_widget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
