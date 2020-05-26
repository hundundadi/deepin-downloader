#include "dbinstance.h"

bool DBInstance::addTask(S_Task _task)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("insert into download_task values (?,?,?,?,?,?,?);");
    _sql.addBindValue(_task.m_task_id);
    _sql.addBindValue(_task.m_gid);
    _sql.addBindValue(_task.m_gid_index);
    _sql.addBindValue(_task.m_url);
    _sql.addBindValue(_task.m_download_path);
    _sql.addBindValue(_task.m_download_filename);
    _sql.addBindValue(_task.m_create_time);
    if(!_sql.exec())
    {
        qWarning()<<"Insert download_task table failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    _q.close();
    return true;
}

bool DBInstance::delTask(QString _taskId)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
     QSqlQuery _sql;
     QString _str = QString("delete from  download_task where task_id=%1;").arg(_taskId);
     _sql.prepare(_str);
     if(!_sql.exec())
     {
         QSqlError error = _sql.lastError();
         qWarning()<<"Delete download_task failed : " << error;
         _q.close();
         return  false;
     }
     _sql.clear();
     _sql.prepare("delete from  download_task_status where task_id=?;");
     _sql.addBindValue(_taskId);
     if(!_sql.exec())
     {
         QSqlError error = _sql.lastError();
         qWarning()<<"Delete download_task_status failed : " << error;
         _q.close();
         return false;
     }
     _sql.clear();
     _sql.prepare("delete from  url_info where task_id=?;");
     _sql.addBindValue(_taskId);
     if(!_sql.exec())
     {
         QSqlError error = _sql.lastError();
         qWarning()<<"Delete url_info failed : " << error;
         _q.close();
         return false;
     }
    _q.close();
    return true;
}

bool DBInstance::delAllTask()
{
   QSqlDatabase _q = DataBase::Instance().getDB();
   if(!_q.open())
   {
       qDebug()<<_q.lastError();
       return  false;
   }
   QSqlQuery _sql;
   _sql.prepare("delete from  download_task;");
   if(!_sql.exec())
   {
       qWarning()<<"Delete download_task failed : " << _sql.lastError();
       _q.close();
       return false;
   }
   _sql.clear();
   _sql.prepare("delete from download_task_status;");
   if(!_sql.exec())
   {
       qWarning()<<"Delete download_task failed : " << _sql.lastError();
       _q.close();
       return false;
   }
   _sql.clear();
   _sql.prepare("delete from url_info;");
   if(!_sql.exec())
   {
       qWarning()<<"Delete download_task failed : " << _sql.lastError();
       _q.close();
       return false;
   }

    return  true;
}

bool DBInstance::updateTaskByID(S_Task &_task)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("update  download_task set  gid=? , gid_index=? , url=? ,download_path=? , download_filename=? ,create_time=? where task_id= ?");
    _sql.addBindValue(_task.m_gid);
    _sql.addBindValue(_task.m_gid_index);
    _sql.addBindValue(_task.m_url);
    _sql.addBindValue(_task.m_download_path);
    _sql.addBindValue(_task.m_download_filename);
    _sql.addBindValue(_task.m_create_time);
    _sql.addBindValue(_task.m_task_id);

    if(!_sql.exec())
    {
        qWarning()<<"Update download_task table failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    _q.close();
    return true;
}

bool DBInstance::getTaskByID(QString _taskId, S_Task &_task)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("select * from download_task where task_id='?';");
    _sql.addBindValue(_taskId);
    if(!_sql.exec())
    {
        qDebug()<<"Select download_task table failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    while(_sql.next())
    {

        _task.m_task_id = _sql.value(0).toString();
        _task.m_gid = _sql.value(1).toString(); //下载gid
        _task.m_gid_index = _sql.value(2).toInt(); //位置index
        _task.m_url = _sql.value(3).toString(); //下载url地址
        _task.m_download_path  = _sql.value(4).toString(); //下载全路径包括文件名
        _task.m_download_filename = _sql.value(5).toString(); //下载文件名
        _task.m_create_time = _sql.value(6).toDateTime(); //任务创建时间
    }
    _q.close();
    return true;
}

bool DBInstance::getAllTask(QList<S_Task> & _taskList)
{

    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("select * from download_task;");
    if(!_sql.exec())
    {
        qDebug()<<"getAllTask download_task table failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    S_Task _task;
    while (_sql.next())
    {
        _task.m_task_id = _sql.value(0).toString();
        _task.m_gid = _sql.value(1).toString(); //下载gid
        _task.m_gid_index = _sql.value(2).toInt(); //位置index
        _task.m_url = _sql.value(3).toString(); //下载url地址
        _task.m_download_path  = _sql.value(4).toString(); //下载全路径包括文件名
        _task.m_download_filename = _sql.value(5).toString(); //下载文件名
        _task.m_create_time = _sql.value(6).toDateTime(); //任务创建时间
        _taskList.push_back(_task);
    }
    _q.close();
    return true;
}

bool DBInstance::isExistUrl(QString url, bool &ret)
{
    ret = false;
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    QString  select_all_sql ="select count(*)  from download_task,download_task_status where download_task.url='"+url+"' and download_task.task_id=download_task_status.task_id;";
    _sql.prepare(select_all_sql);
    if(!_sql.exec())
    {
        qWarning()<<"select download_task,download_task_status failed : " << _sql.lastError();
        return false;
    }
    while(_sql.next())
    {

       if(_sql.value(0).toInt() >=1)
       {
           ret = true;
       }
    }
    _q.close();
    return true;
}

bool DBInstance::addTaskStatus(S_Task_Status _task)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("insert into download_task_status values (?,?,?,?,?,?,?,?,?);");
    _sql.addBindValue(_task.m_task_id);
    _sql.addBindValue(_task.m_download_status);
    _sql.addBindValue(_task.m_modify_time);
    _sql.addBindValue(_task.m_compeletedLength);
    _sql.addBindValue(_task.m_download_speed);
    _sql.addBindValue(_task.m_totalLength);
    _sql.addBindValue(_task.m_percent);
    _sql.addBindValue(_task.m_totalFromSource);
    _sql.addBindValue(_task.m_finish_time);
    if(!_sql.exec())
    {
        QSqlError error = _sql.lastError();
        qWarning()<<"insert download_task_status failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    _q.close();
    return true;
}
bool DBInstance::updateTaskStatusById(S_Task_Status _task)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("update  download_task_status set  download_status=? , modify_time=? ,compeletedLength=? , download_speed=? , totalLength=? ,percent=? , totalFromSource=? ,finish_time=? where task_id= ?");
    _sql.addBindValue(_task.m_download_status);
    _sql.addBindValue(_task.m_modify_time);
    _sql.addBindValue(_task.m_compeletedLength);
    _sql.addBindValue(_task.m_download_speed);
    _sql.addBindValue(_task.m_totalLength);
    _sql.addBindValue(_task.m_percent);
    _sql.addBindValue(_task.m_totalFromSource);
    _sql.addBindValue(_task.m_finish_time);
    _sql.addBindValue(_task.m_task_id);
    if(!_sql.exec())
    {
        qWarning()<<"update download_task_status failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    _q.close();
    return true;
}
bool DBInstance::getTaskStatusById(QString _taskId, S_Task_Status &_task)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    QString select_all_sql = "select * from download_task_status where task_id='"+_taskId+"';";
    _sql.prepare(select_all_sql);
    if(!_sql.exec())
    {
        qWarning()<<"update download_task_status failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    while(_sql.next())
    {
        _task.m_task_id = _sql.value(0).toString();
        _task.m_download_status = _sql.value(1).toInt(); //下载状态
        _task.m_modify_time = _sql.value(2).toDateTime();
        _task.m_compeletedLength = _sql.value(3).toString();
        _task.m_download_speed = _sql.value(4).toString();
        _task.m_totalLength = _sql.value(5).toString();
        _task.m_percent = _sql.value(6).toInt();
        _task.m_totalFromSource = _sql.value(7).toInt();
        _task.m_finish_time = _sql.value(8).toDateTime();
    }
    _q.close();
    return true;
}
bool DBInstance::getAllTaskStatus(QList<S_Task_Status> & _taskList)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QString select_all_sql = "select * from download_task_status;";
    QSqlQuery _sql;
    _sql.prepare(select_all_sql);
    if(!_sql.exec())
    {
        qWarning()<<"get all download_task_status failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    S_Task_Status _task;
    while (_sql.next())
    {
        _task.m_task_id = _sql.value(0).toString();
        _task.m_download_status = _sql.value(1).toInt(); //下载状态
        _task.m_modify_time = _sql.value(2).toDateTime();
        _task.m_compeletedLength = _sql.value(3).toString();
        _task.m_download_speed = _sql.value(4).toString();
        _task.m_totalLength = _sql.value(5).toString();
        _task.m_percent = _sql.value(6).toInt();
        _task.m_totalFromSource = _sql.value(7).toInt();
        _task.m_finish_time = _sql.value(8).toDateTime();
        _taskList.push_back(_task);
    }
    _q.close();
    return true;
}

bool DBInstance::addUrl(S_Url_Info _url)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    _sql.prepare("insert into url_info values (?,?,?,?,?,?);");
    _sql.addBindValue(_url.m_task_id);
    _sql.addBindValue(_url.m_url);
    _sql.addBindValue(_url.m_download_type);
    _sql.addBindValue(_url.m_seedFile);
    _sql.addBindValue(_url.m_selectedNum);
    _sql.addBindValue(_url.m_infoHash);
    if(!_sql.exec())
    {
        qWarning()<<"insert url_info failed : " << _sql.lastError();
        _q.close();
        return false;
    }
    _q.close();
    return true;
}
bool DBInstance::updateUrlById(S_Url_Info _url)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
         _q.close();
        return false;
    }
    QSqlQuery _sql;
    _sql.prepare("update  url_info set url=?,download_type=?,seedFile=?,selectedNum=? ,infoHash=? where task_id= ?");
    _sql.addBindValue(_url.m_url);
    _sql.addBindValue(_url.m_download_type);
    _sql.addBindValue(_url.m_seedFile);
    _sql.addBindValue(_url.m_selectedNum);
    _sql.addBindValue(_url.m_infoHash);
    _sql.addBindValue(_url.m_task_id);

    _q.close();
   return true;
}
bool DBInstance::getUrlById(QString _urlId, S_Url_Info &_url)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    QString select_all_sql = "select * from url_info where task_id='"+_urlId+"';";
    _sql.prepare(select_all_sql);
    if(!_sql.exec())
    {
        qWarning()<<"select url_info failed : " << _sql.lastError();
         _q.close();
        return false;
    }
    while(_sql.next())
    {
        _url.m_task_id = _sql.value(0).toString(); //任务id
        _url.m_url = _sql.value(1).toString(); // url 下载地址
        _url.m_download_type = _sql.value(2).toString();//下载类型
        _url.m_seedFile = _sql.value(3).toString(); //种子文件
        _url.m_selectedNum = _sql.value(4).toString(); //选择的种子文件号码
        _url.m_infoHash = _sql.value(5).toString(); //种子文件hash值
    }
    _q.close();
    return true;
}
bool DBInstance::getAllUrl(QList<S_Url_Info> &_urlList)
{
    QSqlDatabase _q = DataBase::Instance().getDB();
    if(!_q.open())
    {
        qDebug()<<_q.lastError();
        return  false;
    }
    QSqlQuery _sql;
    QString select_all_sql = "select * from url_info;";
    _sql.prepare(select_all_sql);
    if(!_sql.exec())
    {
        qDebug() << _sql.lastError();
        _q.close();
        return false;
    }
    S_Url_Info _url;
    while(_sql.next())
    {
        _url.m_task_id = _sql.value(0).toString(); //任务id
        _url.m_url = _sql.value(1).toString(); // url 下载地址
        _url.m_download_type = _sql.value(2).toString();//下载类型
        _url.m_seedFile = _sql.value(3).toString(); //种子文件
        _url.m_selectedNum = _sql.value(4).toString(); //选择的种子文件号码
        _url.m_infoHash = _sql.value(5).toString(); //种子文件hash值
        _urlList.push_back(_url);
    }
    _q.close();
    return true;
}


