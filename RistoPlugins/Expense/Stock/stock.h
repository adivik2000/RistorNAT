#ifndef STOCK_H
#define STOCK_H

#include "plugininterface.h"

class Stock : public pluginInterface
{
    Q_OBJECT
    Q_INTERFACES(pluginInterface)
public:
    Stock(QWidget *parent = 0);

    /** @brief Name of the plugin
     *  @return Plugin name
     */
    virtual QString name() { return tr("Stock") ; }

    /** @brief Plugin's icon */
    QIcon icon() { return QIcon(":/fornitore48x48.png");  }

    QString family() { return "Expense"; }
};

#endif // STOCK_H
