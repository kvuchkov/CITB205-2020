#ifndef LAB12_INVENTORY_H
#define LAB12_INVENTORY_H

#include "item.h"
#include <vector>

using std::vector;

template <class T>
class Inventory
{
public:
    virtual ~Inventory();
    void add(const T *obj, int quantity);
    bool remove(const T *obj, int quantity);
    vector<Item<T> *> all() const;

private:
    int find(const T *obj);
    vector<Item<T> *> items;
};

template <class T>
Inventory<T>::~Inventory()
{
    for (auto item : items)
    {
        delete item;
    }
}

template <class T>
void Inventory<T>::add(const T *obj, int quantity)
{
    auto idx = find(obj);
    if (idx < 0)
    {
        items.push_back(new Item<T>(obj, quantity));
    }
    else
    {
        auto item = items[idx];
        item->setQuantity(item->getQuantity() + quantity);
    }
}

template <class T>
bool Inventory<T>::remove(const T *obj, int quantity)
{
    int idx = find(obj);
    if (idx >= 0)
    {
        auto item = items[idx];
        int prevQty = item->getQuantity();
        int remaining = std::max(item->getQuantity() - quantity, 0);
        if (remaining > 0)
        {
            item->setQuantity(remaining);
        }
        else
        {
            items.erase(items.begin() + idx);
            delete item;
        }

        return prevQty >= quantity;
    }
    else
    {
        return false;
    }
}

template <class T>
int Inventory<T>::find(const T *obj)
{
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i]->get().getID() == obj->getID())
        {
            return i;
        }
    }
    return -1;
}

template <class T>
vector<Item<T> *> Inventory<T>::all() const
{
    return items;
}

#endif //LAB12_INVENTORY_H