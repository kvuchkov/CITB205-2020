#include <iostream>
using std::cerr;

#ifndef LAB1_ITEM_H
#define LAB1_ITEM_H

template <class T>
class Item
{
public:
    Item(const T *obj, int quantity);
    ~Item();
    int getQuantity() const;
    void setQuantity(int qty);
    const T &get() const;

private:
    const T *obj;
    int quantity;
};

template <class T>
Item<T>::Item(const T *obj, int quantity) : obj(obj)
{
    this->quantity = quantity;
}

template <class T>
Item<T>::~Item()
{
    std::cerr << "~Item" << std::endl;
    delete obj;
}

template <class T>
int Item<T>::getQuantity() const
{
    return quantity;
}

template <class T>
void Item<T>::setQuantity(int qty)
{
    quantity = qty;
}

template <class T>
const T &Item<T>::get() const
{
    return *obj;
}

#endif //LAB1_ITEM_H
