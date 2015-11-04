// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <iostream>
#include <iterator>

#include <nix/util/util.hpp>
#include "BaseTestEntity.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace std;
using namespace nix;


void BaseTestEntity::testId() {
    CPPUNIT_ASSERT(block.id().size() == 36);
    CPPUNIT_ASSERT(util::toId(block).compare(block.id()) == 0);
}


void BaseTestEntity::testName() {
    CPPUNIT_ASSERT(block.name() == "block_one");
    CPPUNIT_ASSERT(util::toName(block).compare(block.name()) == 0);
}


void BaseTestEntity::testType() {
    CPPUNIT_ASSERT(block.type() == "dataset");
    string typ = util::createId();
    block.type(typ);
    CPPUNIT_ASSERT(block.type() == typ);
}


void BaseTestEntity::testDefinition() {
    string def = util::createId();
    block.definition(def);
    CPPUNIT_ASSERT(*block.definition() == def);
    block.definition(nix::none);
    CPPUNIT_ASSERT(block.definition() == nix::none);
}


void BaseTestEntity::testCreatedAt() {
    CPPUNIT_ASSERT(block.createdAt() >= startup_time);
    time_t past_time = time(NULL) - 10000000;
    block.forceCreatedAt(past_time);
    CPPUNIT_ASSERT(block.createdAt() == past_time);
}


void BaseTestEntity::testUpdatedAt() {
    CPPUNIT_ASSERT(block.updatedAt() >= startup_time);
}




