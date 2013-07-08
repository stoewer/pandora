// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <kellner@bio.lmu.de>, Jan Grewe <jan.grewe@g-node.org>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/NamedEntityWithSources.hpp>
#include <string>

#ifndef PANDORA_DATA_ARRAY_H
#define PANDORA_DATA_ARRAY_H

namespace pandora {

class DataArray : public NamedEntityWithSources{

public:
  DataArray(File parentFile, Group thisGroup, std::string identifier);
  DataArray(const DataArray &other);

  std::string label() const;
  void label(const std::string &value);

  std::string unit() const;
  void unit(const std::string &value);

  void scaling(double scaling);
  double scaling()const;

  void offset(double offset);
  double offset() const;

  DataSet data();
};


} //namespace pandora

#endif // PANDORA_DATA_ARRAY_H
