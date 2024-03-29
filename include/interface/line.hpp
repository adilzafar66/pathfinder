#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPointF>
#include <QPen>
#include <cmath>

namespace interface
{
    class ClickableLineItem : public QGraphicsLineItem
    {
    public:
        ClickableLineItem(double cost, Qt::GlobalColor color, int thickness, QGraphicsItem *parent = nullptr) : QGraphicsLineItem(parent), edge_cost(cost), label(nullptr)
        {
            this->setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap));
            setFlag(QGraphicsItem::ItemIsSelectable);
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override
        {
            QGraphicsLineItem::mousePressEvent(event);
            toggle_label();
        }
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
        {
            if (change == QGraphicsItem::ItemSelectedChange && scene())
            {
                // Prevent the item from being selected
                return QVariant();
            }
            return QGraphicsLineItem::itemChange(change, value);
        }

    private:
        void toggle_label()
        {
            if (!label)
            {
                QPointF mid_point = (line().p1() + line().p2()) / 2.0;
                label = scene()->addText(QString::number(edge_cost));
                label->setPos(mid_point);
            }
            else
            {
                scene()->removeItem(label);
                delete label;
                label = nullptr;
            }
        }

        double edge_cost;
        QGraphicsTextItem *label;
    };
} // namespace interface
