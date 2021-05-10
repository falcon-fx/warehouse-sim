#include "pod.h"

/*!
 * \brief Pod::Pod
 * Create a new pod object holding the specified products
 * at the specified position
 * \param prods The products
 * \param origin_x The x coordinate
 * \param origin_y The y coordinate
 */
Pod::Pod(QSet<int> prods, int origin_x, int origin_y)
{
    this->products = prods;
    this->pos.setX(origin_x);
    this->pos.setY(origin_y);
    this->originalPos.setX(origin_x);
    this->originalPos.setY(origin_y);
    this->pickedUp = false;
    this->reserved = false;
}

/*!
 * \brief Pod::Pod
 * Create a new displaced pod object holding the specified
 * products at the specified position and different origin
 * position.
 * \param prods The products
 * \param x The x coordinate
 * \param y The y coordinate
 * \param origin_x The origin x coordinate
 * \param origin_y The origin y coordinate
 */
Pod::Pod(QSet<int> prods, int x, int y, int origin_x, int origin_y)
{
    this->products = prods;
    this->pos.setX(x);
    this->pos.setY(y);
    this->originalPos.setX(origin_x);
    this->originalPos.setY(origin_y);
    this->pickedUp = false;
    this->reserved = false;
}

/*!
 * \brief Pod::hasProduct
 * Check if the pod has the specified product.
 * \param prodNum The product number
 * \return
 */
bool Pod::hasProduct(int prodNum)
{
    return this->products.contains(prodNum);
}

/*!
 * \brief Pod::addProduct
 * Add the specified product to the pod.
 * \param prodNum The product number
 */
void Pod::addProduct(int prodNum)
{
    this->products.insert(prodNum);
}

/*!
 * \brief Pod::removeProduct
 * Remove the specified product from the pod.
 * \param prodNum THe product number
 */
void Pod::removeProduct(int prodNum)
{
    this->products.remove(prodNum);
}

/*!
 * \brief Pod::getProducts
 * Get a set of all the products on the pod.
 * \return
 */
QSet<int> Pod::getProducts()
{
    return this->products;
}

/*!
 * \brief Pod::getPosition
 * Get the position of the pod.
 * \return
 */
QPoint Pod::getPosition()
{
    return this->pos;
}

/*!
 * \brief Pod::setPosition
 * Set the position of the pod.
 * \param pos
 */
void Pod::setPosition(QPoint pos)
{
    this->pos.setX(pos.x());
    this->pos.setY(pos.y());
}

/*!
 * \brief Pod::getOriginalPosition
 * Get the original position of the pod.
 * \return
 */
QPoint Pod::getOriginalPosition()
{
    return this->originalPos;
}

/*!
 * \brief Pod::setPickedUp
 * Set the picked up state of the pod.
 * \param picked Whether the pod is picked up.
 */
void Pod::setPickedUp(bool picked)
{
    this->pickedUp = picked;
}

/*!
 * \brief Pod::isPickedUp
 * Check if the pod is picked up or not.
 * \return
 */
bool Pod::isPickedUp()
{
    return this->pickedUp;
}

/*!
 * \brief Pod::setReserved
 * Set the resered state of the pod.
 * \param picked Whether the pod is reserved.
 * \param reserved
 */
void Pod::setReserved(bool reserved)
{
    this->reserved = reserved;
}

/*!
 * \brief Pod::isReserved
 * Check if the pod is reserved or not.
 * \return
 */
bool Pod::isReserved()
{
    return this->reserved;
}
