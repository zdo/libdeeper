#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QSharedPointer>

namespace deeper {

class Serializable {
public:
    virtual ~Serializable() {}

    virtual QJsonObject serializeToJson() const = 0;
    virtual void deserializeFromJson(const QJsonObject &jsonRaw) = 0;

    template <typename T>
    static QJsonArray toArraySimple(const QVector<T> &src) {
        QJsonArray result;
        for (const T &v : src) {
            result.append(v);
        }
        return result;
    }

    template <typename T>
    static QJsonArray toArray(const QVector<T> &src) {
        QJsonArray result;
        for (const T &v : src) {
            result.append(v.serializeToJson());
        }
        return result;
    }

    template <typename T>
    static QJsonArray toArrayPtr(const QVector<T> &src) {
        QJsonArray result;
        for (const T &v : src) {
            result.append(v->serializeToJson());
        }
        return result;
    }

    static QVector<int> fromArrayInt(const QJsonArray &src) {
        QVector<int> result;
        for (const QJsonValue &v : src) {
            result.append(v.toInt());
        }
        return result;
    }

    static QVector<QString> fromArrayString(const QJsonArray &src) {
        QVector<QString> result;
        for (const QJsonValue &v : src) {
            result.append(v.toString());
        }
        return result;
    }

    template <typename T>
    static QVector<T> fromArray(const QJsonArray &src) {
        QVector<T> result;
        for (const QJsonValue &v : src) {
            T t;
            t.deserializeFromJson(v.toObject());
            result.append(t);
        }
        return result;
    }

    template <typename T>
    static QVector<QSharedPointer<T>> fromArraySharedPointer(const QJsonArray &src) {
        QVector<QSharedPointer<T>> result;
        for (const QJsonValue &v : src) {
            auto t = QSharedPointer<T>::create();
            t->deserializeFromJson(v.toObject());
            result.append(t);
        }
        return result;
    }
};

}

#endif // SERIALIZABLE_HPP
