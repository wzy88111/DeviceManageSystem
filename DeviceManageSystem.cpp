#include "DeviceManageSystem.h"
#include "ui_DeviceManageSystem.h"

#include <QFile>
#include <QStringList>

#include <QTableWidgetItem>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    init();
}

Dialog::~Dialog()
{
    m_db.close();
    delete ui;
}

void Dialog::init()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    bool not_exsist = false;

    QString file_name =QApplication::applicationDirPath() + "/config.db";
    QFile file(file_name);
    if(!file.exists())
    {
        file.open(QIODevice::WriteOnly);
        file.close();

        not_exsist = true;
    }

    m_db.setDatabaseName(file_name);
    if (!m_db.open())
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
                                   "is_scrap VARCHAR(4) NOT NULL," \
                                   "scrap_time VARCHAR(40) )" ;

        QSqlQuery query(m_db);
        bool ret = query.exec(create_table_sql);
        if( !ret ){
            return ;
        }

        //commit_db(query);
    }


    QStringList headers;
    headers << "编号" << "类型" << "名称" << "购入日期" << "是否报废" << "报废日期";
    ui->m_table_widget->setColumnCount(headers.size());
    ui->m_table_widget->setHorizontalHeaderLabels(headers);

    reset_right_device_inf(false);
    ui->m_ok->setEnabled(false);
    m_state = state_query;

    emit ui->m_btn_query->click();
}

void Dialog::commit_db(QSqlQuery &query)
{
    if(query.isActive())
    {
        m_db.commit();
    }
    else
    {
        m_db.rollback();
    }
}

void Dialog::reset_right_device_inf(bool reset, bool enable)
{
    if(reset)
    {
        ui->m_edit_id->setText("");
        ui->m_edit_type->setText("");
        ui->m_edit_name->setText("");

        QDate dt = QDate::currentDate();
        ui->m_dt_buy_time->setDate(dt);

        ui->m_cb_scrap->setChecked(false);
        ui->m_dt_scrap_time->setDate(dt);
        ui->m_dt_scrap_time->setEnabled(false);
    }

    if(enable)
    {
        ui->m_edit_id->setEnabled(true);
        ui->m_edit_type->setEnabled(true);
        ui->m_edit_name->setEnabled(true);

        ui->m_dt_buy_time->setEnabled(true);
        ui->m_cb_scrap->setEnabled(true);
        if(ui->m_cb_scrap->checkState() == Qt::Checked)
            ui->m_dt_scrap_time->setEnabled(true);
        ui->m_ok->setEnabled(true);
    }
    else
    {
        ui->m_edit_id->setEnabled(false);
        ui->m_edit_type->setEnabled(false);
        ui->m_edit_name->setEnabled(false);

        ui->m_dt_buy_time->setEnabled(false);
        ui->m_cb_scrap->setEnabled(false);
        ui->m_dt_scrap_time->setEnabled(false);
        ui->m_ok->setEnabled(false);
    }
}

void Dialog::remove_all_table_data()
{
    int row_count = ui->m_table_widget->rowCount();

    for(int i = 0; i < row_count; i++)
    {
        ui->m_table_widget->removeRow(i);
    }

    ui->m_table_widget->setRowCount(0);

}

void Dialog::push_device_inf_to_table(DeviceInf &di)
{
    int row_count = ui->m_table_widget->rowCount();

    ui->m_table_widget->insertRow(row_count);

    ui->m_table_widget->setItem(row_count, 0, new QTableWidgetItem(di.id));
    ui->m_table_widget->setItem(row_count, 1, new QTableWidgetItem(di.type));
    ui->m_table_widget->setItem(row_count, 2, new QTableWidgetItem(di.name));
    ui->m_table_widget->setItem(row_count, 3, new QTableWidgetItem(di.buy_time));
    ui->m_table_widget->setItem(row_count, 4, new QTableWidgetItem(di.is_scrap));
    ui->m_table_widget->setItem(row_count, 5, new QTableWidgetItem(di.scrap_time));

    for(int i = 0; i < 6; i++)
    {
       ui->m_table_widget->item(row_count, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void Dialog::update_device_inf_to_table(DeviceInf &di)
{
    int cur_row = ui->m_table_widget->currentRow();
    if(cur_row<0) return ;

    ui->m_table_widget->item(cur_row,0)->setText(di.id);
    ui->m_table_widget->item(cur_row,1)->setText(di.type);
    ui->m_table_widget->item(cur_row,2)->setText(di.name);
    ui->m_table_widget->item(cur_row,3)->setText(di.buy_time);
    ui->m_table_widget->item(cur_row,4)->setText(di.is_scrap);
    ui->m_table_widget->item(cur_row,5)->setText(di.scrap_time);
}


void Dialog::on_m_btn_query_clicked()
{
    QString query_sql = "select * from devices ";
    if(ui->m_edit_query_type->text().length() != 0 ||
            ui->m_edit_query_name->text().length() != 0 )
    {
        query_sql += "where ";
    }

    if(ui->m_edit_query_name->text().length() > 0)
    {
        query_sql += "name='";
        query_sql += ui->m_edit_query_name->text();
        query_sql += "'";
    }

    if(ui->m_edit_query_type->text().length() > 0 )
    {
        if(ui->m_edit_query_name->text().length() > 0)
        {
            query_sql += "and type='";
        }
        else
        {
            query_sql += "type='";
        }

        query_sql += ui->m_edit_query_type->text();
        query_sql += "'";
    }

    QSqlQuery query(m_db);
    bool ret = query.exec(query_sql);

    int query_size = 0;
    if( ret )
    {
        remove_all_table_data();

        while(query.next())
        {
            DeviceInf di;
            di.id = query.value(0).toString();
            di.type = query.value(1).toString();
            di.name = query.value(2).toString();
            di.buy_time = query.value(3).toString();
            di.is_scrap = query.value(4).toString();
            di.scrap_time = query.value(5).toString();

            push_device_inf_to_table(di);
            query_size++;
        }
    }

    QString query_result;
    if(query_size>0)
    {
        query_result=QString("共查询( %1 )条记录").arg(query_size);
    }
    else
    {
        query_result = "没有查到结果";
    }

    ui->m_query_result->setText(query_result);

    m_state = state_query;
    reset_right_device_inf(false,false);
}

void Dialog::on_m_btn_add_clicked()
{
    m_state = state_add;

    ui->m_query_result->setText("");

    reset_right_device_inf();
}

void Dialog::on_m_btn_update_clicked()
{
    m_state = state_update;

    ui->m_query_result->setText("");

    reset_right_device_inf(false,true);
    ui->m_edit_id->setEnabled(false);
}

void Dialog::on_m_btn_del_clicked()
{
    m_state = state_del;

    ui->m_query_result->setText("");

    reset_right_device_inf(false, false);
    QSqlQuery query(m_db);
    QString del_sql("delete from devices where id=:id");
    query.prepare(del_sql);
    query.bindValue(":id",ui->m_edit_id->text());
    bool ret = query.exec();

    if( ret )
    {
        int cur_row = ui->m_table_widget->currentRow();
        ui->m_table_widget->removeRow(cur_row);

        QMessageBox msg_box;
        msg_box.setText("删除成功!");
        msg_box.exec();
    }
    else
    {
        QMessageBox msg_box;
        msg_box.setText("删除失败!");
        msg_box.exec();
    }
}

void Dialog::on_m_ok_clicked()
{
    DeviceInf di;
    if( ui->m_edit_id->text().length() == 0 )
    {
        QMessageBox msg_box;
        msg_box.setText("编号不能为空，请重试!");
        msg_box.exec();
        return ;
    }
    else
    {
        di.id = ui->m_edit_id->text();
    }

    if( ui->m_edit_type->text().length() == 0 )
    {
        QMessageBox msg_box;
        msg_box.setText("类型不能为空，请重试!");
        msg_box.exec();
        return ;
    }
    else
    {
        di.type = ui->m_edit_type->text();
    }

    if( ui->m_edit_name->text().length() == 0 )
    {
        QMessageBox msg_box;
        msg_box.setText("名称不能为空，请重试!");
        msg_box.exec();
        return ;
    }
    else
    {
        di.name = ui->m_edit_name->text();
    }

    di.buy_time = ui->m_dt_buy_time->date().toString("yyyy/MM/dd");

    if(ui->m_cb_scrap->checkState() == Qt::Checked)
    {
        di.is_scrap = "是";

        di.scrap_time = ui->m_dt_scrap_time->date().toString("yyyy/MM/dd");
    }

    if( m_state == state_add )
    {
        push_device_inf_to_table(di);

        QSqlQuery query(m_db);
        QString insert_sql("insert into devices(id,type,name,buy_time,is_scrap,scrap_time) values(:id,:type,:name,:buy_time,:is_scrap,:scrap_time)");

        query.prepare(insert_sql);
        query.bindValue(":id",di.id);
        query.bindValue(":type",di.type);
        query.bindValue(":name",di.name);
        query.bindValue(":buy_time",di.buy_time);
        query.bindValue(":is_scrap",di.is_scrap);
        query.bindValue(":scrap_time",di.scrap_time);
        bool ret = query.exec();

        if(ret )
        {
            QMessageBox msg_box;
            msg_box.setText("添加成功!");
            msg_box.exec();
        }
        else
        {
            QMessageBox msg_box;
            msg_box.setText("添加失败!");
            msg_box.exec();
        }


    }
    else if( m_state == state_update )
    {
        update_device_inf_to_table(di);

        QSqlQuery query(m_db);

        QString update_sql("update devices set type=:type,name=:name,buy_time=:buy_time,is_scrap=:is_scrap,scrap_time=:scrap_time \
                        where id=:id");

        query.prepare(update_sql);
        query.bindValue(":id",di.id);
        query.bindValue(":type",di.type);
        query.bindValue(":name",di.name);
        query.bindValue(":buy_time",di.buy_time);
        query.bindValue(":is_scrap",di.is_scrap);
        query.bindValue(":scrap_time",di.scrap_time);
        bool ret = query.exec();

        if(ret )
        {
            QMessageBox msg_box;
            msg_box.setText("更新成功!");
            msg_box.exec();
        }
        else
        {
            QMessageBox msg_box;
            msg_box.setText("更新失败!");
            msg_box.exec();
        }


    }

}


void Dialog::on_m_table_widget_itemSelectionChanged()
{
    int cur_row = ui->m_table_widget->currentRow();
    if(cur_row < 0 ) return ;

    DeviceInf di;
    di.id    = ui->m_table_widget->item(cur_row, 0)->text();
    di.type  = ui->m_table_widget->item(cur_row, 1)->text();
    di.name  = ui->m_table_widget->item(cur_row, 2)->text();
    di.buy_time  = ui->m_table_widget->item(cur_row, 3)->text();
    di.is_scrap  = ui->m_table_widget->item(cur_row, 4)->text();
    di.scrap_time  = ui->m_table_widget->item(cur_row, 5)->text();

    ui->m_edit_id->setText(di.id);
    ui->m_edit_type->setText(di.type);
    ui->m_edit_name->setText(di.name);

    QDate time = QDate::fromString(di.buy_time,"yyyy/MM/dd");
    ui->m_dt_buy_time->setDate(time);

    if(di.is_scrap=="是")
    {
        ui->m_cb_scrap->setChecked(true);

        time = QDate::fromString(di.scrap_time,"yyyy/MM/dd");
        ui->m_dt_scrap_time->setDate(time);
    }
    else
    {
        ui->m_cb_scrap->setChecked(false);
        ui->m_dt_scrap_time->setEnabled(false);
    }

    ui->m_query_result->setText("");

}

void Dialog::on_m_cb_scrap_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked)
    {
        if(m_state==state_add || m_state == state_update)
        {
            ui->m_dt_scrap_time->setEnabled(true);
        }
    }
    else
    {
        ui->m_dt_scrap_time->setEnabled(false);
    }
}
