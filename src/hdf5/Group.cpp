// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/Group.hpp>
#include <boost/multi_array.hpp>

namespace nix {
namespace hdf5 {


Group::Group()
    : h5group()
{}


Group::Group(H5::Group h5group)
    : h5group(h5group)
{}


Group::Group(const Group &group)
    : h5group(group.h5group)
{}


Group& Group::operator=(const Group &group)
{
    h5group = group.h5group;
    return *this;
}


bool Group::hasAttr(const std::string &name) const {
    return H5Aexists(h5group.getId(), name.c_str());
}


void Group::removeAttr(const std::string &name) const {
    h5group.removeAttr(name);
}


bool Group::hasObject(const std::string &name) const {
    // empty string should return false, not exception (which H5Lexists would)
    if(name.empty()) {
        return false;
    }
    htri_t res = H5Lexists(h5group.getLocId(), name.c_str(), H5P_DEFAULT);
    return res;
}


size_t Group::objectCount() const {
    return h5group.getNumObjs();
}


std::string Group::objectName(size_t index) const {
    std::string str_name;
    // check whether name is found by index
    ssize_t name_len = H5Lget_name_by_idx(h5group.getLocId(), 
                                                  ".", 
                                                  H5_INDEX_NAME, 
                                                  H5_ITER_NATIVE, 
                                                  (hsize_t) index, 
                                                  NULL, 
                                                  0, 
                                                  H5P_DEFAULT);
    if(name_len > 0) {
        char* name = new char[name_len+1];
        name_len = H5Lget_name_by_idx(h5group.getLocId(), 
                                      ".", 
                                      H5_INDEX_NAME, 
                                      H5_ITER_NATIVE, 
                                      (hsize_t) index, 
                                      name, 
                                      name_len+1, 
                                      H5P_DEFAULT);
        str_name = name;
        delete [] name;
    } else {
        str_name = "";
    }
    
    return str_name;
}


bool Group::hasData(const std::string &name) const {
    if (hasObject(name)) {
        H5G_stat_t info;
        h5group.getObjinfo(name, info);
        if (info.type == H5G_DATASET) {
            return true;
        }
    }
    return false;
}


void Group::removeData(const std::string &name) {
    if (hasData(name))
        h5group.unlink(name);
}


DataSet Group::openData(const std::string &name) const {
    H5::DataSet ds5 = h5group.openDataSet(name);
    return DataSet(ds5);
}


bool Group::hasGroup(const std::string &name) const {
    if (hasObject(name)) {
        H5G_stat_t info;
        h5group.getObjinfo(name, info);
        if (info.type == H5G_GROUP) {
            return true;
        }
    }
    return false;
}


Group Group::openGroup(const std::string &name, bool create) const {
    Group g;
    if (hasGroup(name)) {
        g = Group(h5group.openGroup(name));
    } else if (create) {
        g = Group(h5group.createGroup(name));
    } else {
        throw std::runtime_error("Unable to open group with name '" + name + "'!");
    }
    return g;
}


void Group::removeGroup(const std::string &name) {
    if (hasGroup(name))
        h5group.unlink(name);
}


void Group::renameGroup(const std::string &old_name, const std::string &new_name) {
    if (hasGroup(old_name)) {
        h5group.move(old_name, new_name);
    }
}


bool Group::operator==(const Group &group) const {
    return h5group.getLocId() == group.h5group.getLocId();
}


bool Group::operator!=(const Group &group) const {
    return h5group.getLocId() != group.h5group.getLocId();
}


H5::Group Group::h5Group() const {
    return h5group;
}


Group::~Group() {
    h5group.close();
}

void Group::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data) {
    attr.read(mem_type, data);
}

 void Group::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data) {
    StringWriter writer(size, data);
    attr.read(mem_type, *writer);
    writer.finish();
    H5::DataSet::vlenReclaim(*writer, mem_type, attr.getSpace()); //recycle space?
}

void Group::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data) {
    attr.write(mem_type, data);
}
void Group::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data) {
    StringReader reader(size, data);
    attr.write(mem_type, *reader);
}

bool Group::createLink(const Group &target, const Group &link_base, const std::string link_name) {
    herr_t err = H5Lcreate_hard(target.h5group.getLocId(), ".",
            link_base.h5group.getLocId(), link_name.c_str(), H5L_SAME_LOC, H5L_SAME_LOC);
    return err == 0;
}

} // namespace hdf5
} // namespace nix
