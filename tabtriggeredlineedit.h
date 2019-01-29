#ifndef TABTRIGGEREDLINEEDIT_H
#define TABTRIGGEREDLINEEDIT_H

#include <QLineEdit>

class TabTriggeredLineEdit : public QLineEdit
{
  public:
    explicit TabTriggeredLineEdit(QWidget* parent = nullptr);

  private:
    void mouseDoubleClickEvent(QMouseEvent* e) override;
};

#endif // TABTRIGGEREDLINEEDIT_H
