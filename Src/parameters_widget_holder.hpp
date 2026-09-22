#pragma once

#include <QEvent>
#include <QObject>
#include <QWidget>

template <std::derived_from<QWidget> ParametersWidgetType>
class ParametersWidgetHolder : public QObject {
public:
    ParametersWidgetHolder() : parameters_widget_{new ParametersWidgetType{&holder_}}
    {
        parameters_widget_->installEventFilter(this);
    };

    ParametersWidgetType* parametersWidget() const
    {
        return parameters_widget_;
    }

protected:
    bool eventFilter(QObject* object, QEvent* event) override
    {
        if (object == parameters_widget_ && event->type() == QEvent::ParentChange) {
            if (!object->parent()) {
                parameters_widget_->setParent(&holder_);
            }
        }

        return QObject::eventFilter(object, event);
    }

private:
    QWidget               holder_;
    ParametersWidgetType* parameters_widget_ = nullptr;
};