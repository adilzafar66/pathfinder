#ifndef NODE_H
#define NODE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

namespace interface
{
    class ClickableVertexItem : public QGraphicsEllipseItem
    {
    public:
        ClickableVertexItem(unsigned int position, Qt::GlobalColor color = Qt::red, QGraphicsItem *parent = nullptr) : QGraphicsEllipseItem(parent), vertex_pos(position), label(nullptr)
        {
            setRect(-8, -8, 16, 16);
            setBrush(color);
            setFlag(QGraphicsItem::ItemIsSelectable);
        }

    protected:
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override
        {
            QGraphicsEllipseItem::mouseReleaseEvent(event);
            toggle_label();
        }
        QVariant itemChange(GraphicsItemChange change, const QVariant &value) override
        {
            if (change == QGraphicsItem::ItemSelectedChange && scene())
            {
                // Prevent the item from being selected
                return QVariant();
            }
            return QGraphicsEllipseItem::itemChange(change, value);
        }

    private:
        void toggle_label()
        {
            if (!label)
            {
                QPointF point = this->pos();
                label = scene()->addText(QString::number(vertex_pos));
                label->setPos(point);
            }
            else
            {
                scene()->removeItem(label);
                delete label;
                label = nullptr;
            }
        }

        unsigned int vertex_pos;
        QGraphicsTextItem *label;
    };
} // namespace interface

#endif // NODE_H