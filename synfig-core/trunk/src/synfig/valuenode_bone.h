/* === S Y N F I G ========================================================= */
/*!	\file valuenode_bone.h
**	\brief Header file for implementation of the "Bone" valuenode conversion.
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**	Copyright (c) 2007 Chris Moore
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_VALUENODE_BONE_H
#define __SYNFIG_VALUENODE_BONE_H

/* === H E A D E R S ======================================================= */

#include "valuenode.h"

/* === M A C R O S ========================================================= */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace synfig {

class ValueNode_Bone : public LinkableValueNode
{
	ValueNode::RHandle name_;
	ValueNode::RHandle origin_;
	ValueNode::RHandle origin0_;
	ValueNode::RHandle angle_;
	ValueNode::RHandle angle0_;
	ValueNode::RHandle scale_;
	ValueNode::RHandle length_;
	ValueNode::RHandle strength_;
	ValueNode::RHandle parent_;

	ValueNode_Bone(const ValueBase &value);

public:

	typedef etl::handle<ValueNode_Bone> Handle;
	typedef etl::handle<const ValueNode_Bone> ConstHandle;
	typedef etl::loose_handle<ValueNode_Bone> LooseHandle;
	typedef std::map<synfig::GUID, LooseHandle> BoneMap;

	virtual ValueBase operator()(Time t)const;

	virtual ~ValueNode_Bone();

	virtual String get_name()const;
	virtual String get_local_name()const;


	virtual ValueNode::LooseHandle get_link_vfunc(int i)const;
	virtual int link_count()const;
	virtual String link_name(int i)const;

	virtual String link_local_name(int i)const;
	virtual int get_link_index_from_name(const String &name)const;

protected:
	LinkableValueNode* create_new()const;
	virtual bool set_link_vfunc(int i,ValueNode::Handle x);

	virtual void on_changed();

public:
	using synfig::LinkableValueNode::get_link_vfunc;

	using synfig::LinkableValueNode::set_link_vfunc;
	static bool check_type(ValueBase::Type type);
	static ValueNode_Bone* create(const ValueBase &x);
	static BoneMap::const_iterator map_begin();
	static BoneMap::const_iterator map_end();
	static ValueNode_Bone::Handle find(GUID guid);
	ValueNode_Bone::ConstHandle is_ancestor_of(ValueNode_Bone::ConstHandle bone, Time t)const;
#ifdef _DEBUG
	virtual void ref()const;
	virtual bool unref()const;
	virtual void rref()const;
	virtual void runref()const;
#endif

}; // END of class ValueNode_Bone

}; // END of namespace synfig

/* === E N D =============================================================== */

#endif
