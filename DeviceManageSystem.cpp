#include "DeviceManageSystem.h"
#include "ui_DeviceManageSystem.h"
#include <QFile>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //init();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    bool not_exsist = false;
    QString file_name =QApplication::applicationDirPath() + "/config.db";
    QFile file(file_name);
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.close();

        not_exsist = true;
    }

    //db.setDatabaseName(file_name);
    //if (!db.open())
    {
           // return ;
    }


}


void Dialog::on_m_btn_query_clicked()
{

}

void Dialog::on_m_btn_add_clicked()
{

}

void Dialog::on_m_btn_update_clicked()
{

}

void Dialog::on_m_btn_del_clicked()
{

}

void Dialog::on_m_ok_clicked()
{

}

void Dialog::on_m_table_widget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{

}
