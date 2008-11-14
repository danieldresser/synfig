/* === S Y N F I G ========================================================= */
/*!	\file valuenode_blinecalctangent.cpp
**	\brief Implementation of the "BLine Tangent" valuenode conversion.
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007, 2008 Chris Moore
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "valuenode_blinecalctangent.h"
#include "valuenode_bline.h"
#include "valuenode_const.h"
#include "valuenode_composite.h"
#include "general.h"
#include "exception.h"
#include <ETL/hermite>
#include <ETL/calculus>

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

ValueNode_BLineCalcTangent::ValueNode_BLineCalcTangent(const ValueBase::Type &x):
	LinkableValueNode(x)
{
	if(x!=ValueBase::TYPE_ANGLE && x!=ValueBase::TYPE_REAL && x!=ValueBase::TYPE_VECTOR)
		throw Exception::BadType(ValueBase::type_local_name(x));

	ValueNode_BLine* value_node(new ValueNode_BLine());
	set_link("bline",value_node);
	set_link("loop",ValueNode_Const::create(bool(false)));
	set_link("amount",ValueNode_Const::create(Real(0.5)));
	set_link("offset",ValueNode_Const::create(Angle::deg(0)));
	set_link("scale",ValueNode_Const::create(Real(1.0)));
	set_link("fixed_length",ValueNode_Const::create(bool(false)));
}

LinkableValueNode*
ValueNode_BLineCalcTangent::create_new()const
{
	return new ValueNode_BLineCalcTangent(get_type());
}

ValueNode_BLineCalcTangent*
ValueNode_BLineCalcTangent::create(const ValueBase &x)
{
	return new ValueNode_BLineCalcTangent(x.get_type());
}

ValueNode_BLineCalcTangent::~ValueNode_BLineCalcTangent()
{
	unlink_all();
}

ValueBase
ValueNode_BLineCalcTangent::operator()(Time t, Real amount)const
{
	const std::vector<ValueBase> bline((*bline_)(t));
	handle<ValueNode_BLine> bline_value_node(bline_);
	const bool looped(bline_value_node->get_loop());
	int size = bline.size(), from_vertex;
	bool loop((*loop_)(t).get(bool()));
	Angle offset((*offset_)(t).get(Angle()));
	Real scale((*scale_)(t).get(Real()));
	bool fixed_length((*fixed_length_)(t).get(bool()));
	BLinePoint blinepoint0, blinepoint1;

	if (!looped) size--;
	if (size < 1)
		switch (get_type())
		{
			case ValueBase::TYPE_ANGLE:  return Angle();
			case ValueBase::TYPE_REAL:   return Real();
			case ValueBase::TYPE_VECTOR: return Vector();
			default: assert(0); return ValueBase();
		}
	if (loop)
	{
		amount = amount - int(amount);
		if (amount < 0) amount++;
	}
	else
	{
		if (amount < 0) amount = 0;
		if (amount > 1) amount = 1;
	}

	vector<ValueBase>::const_iterator iter, next(bline.begin());

	iter = looped ? --bline.end() : next++;
	amount = amount * size;
	from_vertex = int(amount);
	if (from_vertex > size-1) from_vertex = size-1;
	blinepoint0 = from_vertex ? *(next+from_vertex-1) : *iter;
	blinepoint1 = *(next+from_vertex);

	etl::hermite<Vector> curve(blinepoint0.get_vertex(),   blinepoint1.get_vertex(),
							   blinepoint0.get_tangent2(), blinepoint1.get_tangent1());
	etl::derivative< etl::hermite<Vector> > deriv(curve);

	switch (get_type())
	{
		case ValueBase::TYPE_ANGLE:  return deriv(amount-from_vertex).angle() + offset;
		case ValueBase::TYPE_REAL:
		{
			if (fixed_length) return scale;
			return deriv(amount-from_vertex).mag() * scale;
		}
		case ValueBase::TYPE_VECTOR:
		{
			Vector tangent(deriv(amount-from_vertex));
			Angle angle(tangent.angle() + offset);
			Real mag = fixed_length ? scale : (tangent.mag() * scale);
			return Vector(Angle::cos(angle).get()*mag,
						  Angle::sin(angle).get()*mag);
		}
		default: assert(0); return ValueBase();
	}
}

ValueBase
ValueNode_BLineCalcTangent::operator()(Time t)const
{
	Real amount((*amount_)(t).get(Real()));
	return (*this)(t, amount);
}

String
ValueNode_BLineCalcTangent::get_name()const
{
	return "blinecalctangent";
}

String
ValueNode_BLineCalcTangent::get_local_name()const
{
	return _("BLine Tangent");
}

bool
ValueNode_BLineCalcTangent::set_link_vfunc(int i,ValueNode::Handle value)
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
	case 0: CHECK_TYPE_AND_SET_VALUE(bline_,		ValueBase::TYPE_LIST);
	case 1: CHECK_TYPE_AND_SET_VALUE(loop_,			ValueBase::TYPE_BOOL);
	case 2: CHECK_TYPE_AND_SET_VALUE(amount_,		ValueBase::TYPE_REAL);
	case 3: CHECK_TYPE_AND_SET_VALUE(offset_,		ValueBase::TYPE_ANGLE);
	case 4: CHECK_TYPE_AND_SET_VALUE(scale_,		ValueBase::TYPE_REAL);
	case 5: CHECK_TYPE_AND_SET_VALUE(fixed_length_,	ValueBase::TYPE_BOOL);
	}
	return false;
}

ValueNode::LooseHandle
ValueNode_BLineCalcTangent::get_link_vfunc(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return bline_;
		case 1: return loop_;
		case 2: return amount_;
		case 3: return offset_;
		case 4: return scale_;
		case 5: return fixed_length_;
	}

	return 0;
}

int
ValueNode_BLineCalcTangent::link_count()const
{
	return 6;
}

String
ValueNode_BLineCalcTangent::link_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return "bline";
		case 1: return "loop";
		case 2: return "amount";
		case 3: return "offset";
		case 4: return "scale";
		case 5: return "fixed_length";
	}
	return String();
}

String
ValueNode_BLineCalcTangent::link_local_name(int i)const
{
	assert(i>=0 && i<link_count());

	switch(i)
	{
		case 0: return _("BLine");
		case 1: return _("Loop");
		case 2: return _("Amount");
		case 3: return _("Offset");
		case 4: return _("Scale");
		case 5: return _("Fixed Length");
	}
	return String();
}

int
ValueNode_BLineCalcTangent::get_link_index_from_name(const String &name)const
{
	if (name=="bline")		  return 0;
	if (name=="loop")		  return 1;
	if (name=="amount")		  return 2;
	if (name=="offset")		  return 3;
	if (name=="scale")		  return 4;
	if (name=="fixed_length") return 5;
	throw Exception::BadLinkName(name);
}

bool
ValueNode_BLineCalcTangent::check_type(ValueBase::Type type)
{
	return (type==ValueBase::TYPE_ANGLE ||
			type==ValueBase::TYPE_REAL  ||
			type==ValueBase::TYPE_VECTOR);
}