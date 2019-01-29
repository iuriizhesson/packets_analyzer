#include "tabtriggeredlineedit.h"

TabTriggeredLineEdit::TabTriggeredLineEdit(QWidget* parent)
  : QLineEdit(parent)
{
}

void TabTriggeredLineEdit::mouseDoubleClickEvent(QMouseEvent* e)
{
    const auto policy(focusPolicy());
    setFocusPolicy(((policy & Qt::TabFocus) ? Qt::ClickFocus : Qt::TabFocus));
    QLineEdit::mouseDoubleClickEvent(e);
}
