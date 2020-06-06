#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTableWidget>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>


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
        , is_scrap("否")
        , scrap_time("")
        {}
    QString id;
    QString type;
    QString name;
    QString buy_time;
    QString is_scrap;
    QString scrap_time;
}DeviceInf;


enum state
{
    state_query,
    state_add,
    state_update,
    state_del
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void init();

    void commit_db(QSqlQuery& query);

    void reset_right_device_inf(bool reset=true, bool enable=true);

    void remove_all_table_data();
    void push_device_inf_to_table(DeviceInf& di);

    void update_device_inf_to_table(DeviceInf& di);

private slots:
    void on_m_btn_query_clicked();

    void on_m_btn_add_clicked();

    void on_m_btn_update_clicked();

    void on_m_btn_del_clicked();

    void on_m_ok_clicked();

    void on_m_table_widget_itemSelectionChanged();

    void on_m_cb_scrap_stateChanged(int arg1);

private:
    Ui::Dialog *ui;
    state m_state;
    QSqlDatabase m_db;
};
#endif // DIALOG_H
