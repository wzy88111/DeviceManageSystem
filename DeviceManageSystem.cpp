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

    init();
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

    db.setDatabaseName(file_name);
    if (!db.open())
    {
        return ;
    }

    if(not_exsist)
    {
        QString create_table_sql = "CREATE TABLE devices (" \
                                   "id VARCHAR(40) PRIMARY KEY," \
                                   "type VARCHAR(40) NOT NULL," \
                                   "name VARCHAR(40) NOT NULL," \
                                   "buy_time VARCHAR(40) NOT NULL," \
                                   "is_scrap INTEGER NOT NULL," \
                                   "scrap_time VARCHAR(40) NOT NULL)" ;

        QSqlQuery query;
        bool ret = query.exec(create_table_sql);
        if( !ret ){
            return ;
        }
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
