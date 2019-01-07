#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QSharedPointer>

namespace deeper {

class Serializable {
public:
    virtual ~Serializable() {}

    virtual QJsonObject serializeToJson() const = 0;
    virtual void deserializeFromJson(const QJsonObject &jsonRaw) = 0;

    template <typename T>
    static QJsonArray toArraySimple(const QList<T> &src) {
        QJsonArray result;
        for (const T &v : src) {
            result.append(v);
        }
        return result;
    }

    template <typename T>
    static QJsonArray toArray(const QList<T> &src) {
        QJsonArray result;
        for (const T &v : src) {
            result.append(v.serializeToJson());
        }
        return result;
    }

    template <typename T>
    static QJsonArray toArrayPtr(const QList<T> &src) {
        QJsonArray result;
        for (const T &v : src) {
            result.append(v->serializeToJson());
        }
        return result;
    }

    static QList<int> fromArrayInt(const QJsonArray &src) {
        QList<int> result;
        for (const QJsonValue &v : src) {
            result.append(v.toInt());
        }
        return result;
    }

    static QList<QString> fromArrayString(const QJsonArray &src) {
        QList<QString> result;
        for (const QJsonValue &v : src) {
            result.append(v.toString());
        }
        return result;
    }

    template <typename T>
    static QList<T> fromArray(const QJsonArray &src) {
        QList<T> result;
        for (const QJsonValue &v : src) {
            T t;
            t.deserializeFromJson(v.toObject());
            result.append(t);
        }
        return result;
    }

    template <typename T>
    static QList<QSharedPointer<T>> fromArraySharedPointer(const QJsonArray &src) {
        QList<QSharedPointer<T>> result;
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
