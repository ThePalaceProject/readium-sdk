//
//  spine.cpp
//  ePub3
//
//  Created by Jim Dovey on 2012-11-29.
//  Copyright (c) 2012 Kobo Inc. All rights reserved.
//

#include "spine.h"
#include "package.h"

EPUB3_BEGIN_NAMESPACE

SpineItem::SpineItem(xmlNodePtr node, Package * owner) : _idref(), _owner(owner), _linear(true)
{
    _idref = _getProp(node, "idref");
    
    std::string str = _getProp(node, "linear");
    if ( str.empty() )
        return;
    
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if ( str == "false" )
        _linear = false;
}
SpineItem::SpineItem(SpineItem&& o) : _idref(std::move(o._idref)), _owner(o._owner), _linear(o._linear), _prev(o._prev), _next(o._next)
{
    o._owner = nullptr;
    o._prev = nullptr;
    o._next = nullptr;
}
SpineItem::~SpineItem()
{
    if ( _prev != nullptr )
        _prev->_next = nullptr;
    if ( _next != nullptr )
        delete _next;
}
const ManifestItem * SpineItem::ManifestItem() const
{
    return _owner->ManifestItemWithID(Idref());
}
SpineItem* SpineItem::NextStep()
{
    SpineItem* n = Next();
    while ( n != nullptr && n->Linear() == false )
        n = n->Next();
    return n;
}
const SpineItem* SpineItem::NextStep() const
{
    return const_cast<SpineItem*>(this)->NextStep();
}
SpineItem* SpineItem::PriorStep()
{
    SpineItem *p = Previous();
    while ( p != nullptr && p->Linear() == false )
        p = p->Previous();
    return p;
}
const SpineItem* SpineItem::PriorStep() const
{
    return const_cast<SpineItem*>(this)->PriorStep();
}

EPUB3_END_NAMESPACE