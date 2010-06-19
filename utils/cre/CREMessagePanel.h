#ifndef _CREMESSAGEPANEL_H
#define	_CREMESSAGEPANEL_H

#include <QObject>
#include <QBrush>
#include "CREPanel.h"

class MessageFile;
class MessageRule;
class QLineEdit;
class QTreeWidget;
class QTextEdit;
class QTreeWidgetItem;
class CRERulePanel;
class MessageManager;

class CREMessagePanel : public CREPanel
{
    Q_OBJECT

    public:
        CREMessagePanel(const MessageManager* manager);
        virtual ~CREMessagePanel();

        void setMessage(MessageFile* message);
        virtual void commitData();

    private:
        MessageFile* myMessage;
        QLineEdit* myPath;
        QLineEdit* myLocation;
        QTreeWidget* myRules;
        QBrush myDefaultBackground;
        CRERulePanel* myRulePanel;

        void fillRuleItem(QTreeWidgetItem* item, MessageRule* rule);
    private slots:
        void currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
        void currentRuleModified();
        void onAddRule(bool);
        void onDeleteRule(bool);
        void onMoveUp(bool);
        void onMoveDown(bool);
        void onDuplicate(bool);
};

#endif	/* _CREMESSAGEPANEL_H */

