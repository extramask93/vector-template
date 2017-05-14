#ifndef VECTOR_H
#define VECTOR_H
#include<algorithm>
#include<initializer_list>
#include<exception>
#include<stdexcept>
#include <memory>

using size_type = unsigned long;

template<typename T, typename A = std::allocator<T>>
class vector_
{
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator =const T*;
private:
    A alloc;
    size_type sz;
    size_type space;
    T *elems;
public:
    vector_();
    explicit vector_(size_type size,T val = T()); //explicit supresses implicit conversions from int to vec, example: vector v = 10 <== vector(10)
    vector_(std::initializer_list<T> ls);
    vector_(const vector_ &v);
    vector_& operator=(const vector_& v);
    vector_(vector_ &&v);
    vector_& operator=(vector_ &&v);
    T& operator[](const size_type i);
    T const& operator [](const size_type i) const;
    void reserve(size_type s);
    size_type capacity() const;
    size_type size() const;
    void resize(size_type newsize, T val = T());
    void pushBack(T elem);
    void pushFront(T elem);
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    ~vector_();
};

template<typename T, typename A>
vector_<T,A>::vector_():sz{0},space{0},elems{nullptr}{}
template<typename T, typename A>
vector_<T,A>::vector_(size_type size,T val):sz{size},space{size},elems{new T[sz]}
{

}
template<typename T, typename A>
vector_<T,A>::vector_(std::initializer_list<T> ls): sz{ls.size()},space{ls.size()},elems{new T[sz]}
{
    copy(ls.begin(),ls.end(),elems);
}
template<typename T, typename A>
vector_<T,A>::vector_(const vector_ &v)
{
    sz=v.sz;
    space=sz;
    elems = new T[sz];

    copy(v.elems,v.elems+v.sz,elems);
}
template<typename T, typename A>
vector_<T,A> &vector_<T,A>::operator=(const vector_ &v)
{
    if(this==&v) return *this;
    if(v.sz<=space)
    {
        copy(v.elems,v.elems+v.sz,elems);
    }
    T* nelems=new T[v.sz];
    copy(v.elems,v.elems+v.sz,nelems);
    delete[] elems;
    elems=nelems;
    sz=space=v.sz;
    return *this;
}
template<typename T, typename A>
vector_<T,A>::vector_(vector_ &&v):sz{v.sz},space{v.space},elems{v.elems}
{
    v.sz=0;
    v.space=0;
    v.elems=nullptr;
}
template<typename T, typename A>
vector_<T,A> &vector_<T,A>::operator=(vector_ &&v)
{
    delete[] elems;
    elems=v.elems;
    sz=v.sz;
    space=v.space;
    v.sz=0;
    v.elems=nullptr;
    return *this;
}
template<typename T, typename A>
T &vector_<T,A>::operator[](const size_type i)
{
    if(i<sz)
        return elems[i];
    else
        throw std::out_of_range{""};
}
template<typename T, typename A>
const T &vector_<T,A>::operator [](const size_type i) const
{
    if(i<sz)
        return elems[i];
    else
        throw std::out_of_range("");
}
template<typename T, typename A>
void vector_<T,A>::reserve(size_type s)
{
    if(this->space<s)
    {
        T *temp = alloc.allocate(s);
        for(size_type i=0;i<sz;i++)
            alloc.construct(&temp[i],elems[i]);//can leak
        for(size_type i=0;i<sz;++i)
            alloc.destroy(&elems[i]);
        alloc.deallocate(elems,space);
        elems=temp;
        space=s;
    }
}
template<typename T, typename A>
size_type vector_<T,A>::capacity() const
{
    return space;
}
template<typename T, typename A>
size_type vector_<T,A>::size() const
{
    return sz;
}
template<typename T, typename A>
void vector_<T,A>::resize(size_type newsize,T val)
{
    reserve(newsize);
    for(size_type i=sz;i<newsize;i++)
        alloc.construct(&elems[i],val);
    for(size_type i=newsize;i<sz;++i)
        alloc.destroy(&elems[i]);
    sz=newsize;
}
template<typename T, typename A>
void vector_<T,A>::pushBack(T elem)
{
    if(!space)
        reserve(8);
    if(sz==space)
        reserve(2*space);
    alloc.construct(&elems[sz],elem);
    ++sz;
}
template<typename T, typename A>
void vector_<T,A>::pushFront(T elem)
{
    if(!space)
        reserve(8);
    if(sz==space)
        reserve(2*space);
    T *newelems = new T[sz+1];
    newelems[0]=elem;
    ++sz;
    for(size_type i=1;i<sz;i++)
    {
        newelems[i]=elems[i-1];
    }
    delete[] elems;
    elems=newelems;
}
template<typename T, typename A>
typename vector_<T,A>::iterator vector_<T,A>::begin()
{
    return elems;
}
template<typename T, typename A>
typename vector_<T,A>::iterator vector_<T,A>::end()
{
    return elems+sz;
}
template<typename T, typename A>
vector_<T,A>::~vector_()
{
    delete[] elems;
}
#endif // VECTOR_H
