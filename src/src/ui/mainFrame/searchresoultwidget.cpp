/**
 * @copyright 2021-2021 Uniontech Technology Co., Ltd.
 *
 * @file searchresoultwidget.cpp
 *
 * @brief 搜索框类
 *
 * @date 2021-01-08 15:22
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
 **/


#include "searchresoultwidget.h"
#include <QListWidgetItem>
#include <QWidget>
#include <dplatformwindowhandle.h>
#include "global.h"

SearchResoultWidget::SearchResoultWidget(QWidget *parent)
    : QListWidget(parent)
{
    setMinimumWidth(380);
    setFixedHeight(200);
    DPlatformWindowHandle handle(this);
    handle.setWindowRadius(18);
}

void SearchResoultWidget::setData(QList<QString> &taskIDList,
                                  QList<int> &taskStatusList, QList<QString> &tasknameList)
{
    this->clear();

    for(int i = 0; i< taskIDList.count(); i++){
        QListWidgetItem *item = new QListWidgetItem;
        QString text;
        if(taskStatusList.at(i) == Global::Complete) {
            item->setIcon(QIcon::fromTheme("dcc_print_done"));
            text += tr("   Completed");
        } else if(taskStatusList.at(i) == Global::Removed) {
            item->setIcon(QIcon::fromTheme("dcc_list_delete"));
            text += tr("   Trash");
        } else {
            item->setIcon(QIcon::fromTheme("dcc_list_downloading"));
            text += tr("   Downloading");
        }

        item->setText(text + "  -->  " + tasknameList.at(i));
        item->setData(Qt::WhatsThisRole, taskIDList.at(i));
        addItem(item);
    }
}

void SearchResoultWidget::focusOutEvent(QFocusEvent *event)
{
    this->hide();
}