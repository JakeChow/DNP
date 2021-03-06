/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __LINK_PARSER_TEST_H_
#define __LINK_PARSER_TEST_H_

#include <opendnp3/LogLevels.h>
#include <opendnp3/link/LinkLayerParser.h>

#include <dnp3mocks/MockFrameSink.h>

#include <testlib/BufferHelpers.h>
#include <testlib/MockLogHandler.h>

#include <assert.h>

#include <cstring>

namespace opendnp3
{

class LinkParserTest
{
public:
    LinkParserTest(bool aImmediate = false) : log(), sink(), parser(log.logger) {}

    void WriteData(const openpal::RSlice& input)
    {
        auto buff = parser.WriteBuff();
        assert(input.Size() <= buff.Size());
        input.CopyTo(buff);
        parser.OnRead(input.Size(), sink);
    }

    void WriteData(const std::string& hex)
    {
        testlib::HexSequence hs(hex);
        auto buff = parser.WriteBuff();
        assert(hs.Size() <= buff.Size());
        memcpy(buff, hs, hs.Size());
        parser.OnRead(hs.Size(), sink);
    }

    testlib::MockLogHandler log;
    MockFrameSink sink;
    LinkLayerParser parser;
};

} // namespace opendnp3

#endif
