/**
* @file itemDelegate.h
* @brief 列表代理类
* @author zhaoyue  <zhaoyue@uniontech.com>
* @version 1.0.0
* @date 2020-05-26 09:44
* @copyright 2020-2020 Uniontech Technology Co., Ltd.
*/


#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H


#include <QStyledItemDelegate>

class QProgressBar;
/**
 * @class ItemDelegate
 * @brief 列表代理类
*/
class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:    
    ItemDelegate(QObject *parent = nullptr,int Flag=0);
    ~ItemDelegate();
    /**
     * @brief 绘图事件
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /**
     * @brief 大小
     */
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    /**
     * @brief 开始编辑前调用
     */
    bool editorEvent(QEvent*event, QAbstractItemModel *model,  const QStyleOptionViewItem &option,const QModelIndex &index);
private:
    int Table_Flag;
     //QProgressBar *progressbar;
     int hoverRow;
     QPixmap *bg;
     QPixmap *front;
public slots:
     void slot_hoverChanged(const QModelIndex &index);

};

#endif
