/**
 * @copyright 2020-2020 Uniontech Technology Co., Ltd.
 *
 * @file tabledatacontrol.h
 *
 * @brief 表格数据管理类
 *
 * @date 2020-06-10 17:55
 *
 * Author: zhaoyue  <zhaoyue@uniontech.com>
 *
 * Maintainer: zhaoyue  <zhaoyue@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TABLEDATACONTROL_H
#define TABLEDATACONTROL_H

#include <QObject>

class TableView;
struct S_Task;
namespace Global {
    struct DataItem;
    struct DelDataItem;
}

class tableDataControl : public QObject
{
    Q_OBJECT
public:
    explicit tableDataControl(TableView* pTableView, QObject *parent = nullptr);

    void initTabledata();

    /**
     * @brief aria2下载事件
     */
    void aria2MethodAdd(QJsonObject &json, QString &searchContent);

    /**
     * @brief aria2状态改变事件
     */
    void aria2MethodStatusChanged(QJsonObject &json, int iCurrentRow,  QString &searchContent);

    /**
     * @brief aria2关闭事件
     */
    void aria2MethodShutdown(QJsonObject &json);

    /**
     * @brief aria2获取文件事件
     */
    void aria2MethodGetFiles(QJsonObject &json, int iCurrentRow);

    /**
     * @brief aria2继续下载事件
     */
    void aria2MethodUnpause(QJsonObject &json, int iCurrentRow);

    /**
     * @brief aria2继续下载事件
     */
    void aria2MethodUnpauseAll(QJsonObject &json, int iCurrentRow);


    /**
     * @brief aria2强制删除事件
     */
    void aria2MethodForceRemove(QJsonObject &json);

    /**
     * @brief 退出之前保存
     */
    void saveDataBeforeClose();

    /**
     * @brief 查找的文本改变
    */
    void searchEditTextChanged(QString text);

    /**
     * @brief 删除ACtion槽函数
    */
    int onDelAction(int currentLab);

    /**
     * @brief 重新下载ACtion槽函数
    */
    void onRedownloadAction(int currentLab);

    /**
     * @brief 还原下载ACtion槽函数
    */
    void onReturnOriginAction();

    /**
     * @brief 打开文件ACtion槽函数
    */
    void onOpenFileAction();

    /**
     * @brief 打开文件目录ACtion槽函数
    */
    int onOpenFolderAction(int currentLab);

    /**
     * @brief 重命名文件ACtion槽函数
    */
    void onRenameAction();

    /**
     * @brief 清除回收站ACtion槽函数
    */
    void onClearRecyleAction();

    /**
     * @brief 复制URL ACtion槽函数
    */
    int onCopyUrlAction(int currentLab, QString &copyUrl);

    /**
     * @brief 永久删除ACtion槽函数
    */
    int onDeletePermanentAction(int currentLab);

    /**
     * @brief  获取删除窗口确定信号
     * @param ischecked 是否删除本地文件，true 删除本地文件；false 不删除
     * @param permanent 是否彻底删除，true彻底删除；false不彻底删除
     */
    void onDeleteConfirm(bool ischecked,bool permanent, int currentLab);

private:

    /**
     * @brief 获取url中的文件名称
     * @param url 下载地址
     */
    QString getFileName(const QString &url);

    /**
     * @brief 处理设置界面通知设置函数
     */
    void dealNotificaitonSettings(QString statusStr, QString fileName);

    /**
     * @brief 格式化文件大小 （1B1KB1MB1GB）
     */
    QString formatFileSize(long size);

    /**
     * @brief 从配置文件中获取下载路径
     */
    QString   getDownloadSavepathFromConfig();

    /**
     * @brief 格式化下载速度（1B1KB1MB1GB  /S）
     */
    QString formatDownloadSpeed(long size);

    /**
     * @brief 检查文件是否存在
     */
    bool checkFileExist(QString &filePath);

signals:
    /**
     * @brief 文件为bt，自动开始下载
     */
    void signalAutoDownloadBt(QString btFilePath);

    /**
     * @brief 文件为bt，自动开始下载
     */
    void signalRedownload(QString taskId, int rd);

public slots:

private:
    TableView *m_pTableView;
    QList<Global::DataItem*> m_pDeleteList;
    QList<Global::DelDataItem*> m_pRecycleDeleteList;
    QList<Global::DataItem*> m_reloadList;  /*已完成界面点击重新下载的数据列表*/
    QList<Global::DelDataItem*> m_recycleReloadList;  /*回收站界面点击重新下载的数据列表*/
};

#endif // TABLEDATACONTROL_H