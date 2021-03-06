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
#include "MockLowerLayer.h"

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>

#include <cassert>

using namespace openpal;
using namespace testlib;

namespace opendnp3
{

bool MockLowerLayer::HasNoData() const
{
    return sendQueue.empty();
}

size_t MockLowerLayer::NumWrites() const
{
    return sendQueue.size();
}

std::string MockLowerLayer::PopWriteAsHex()
{
    if (sendQueue.empty())
    {
        return "";
    }

    auto ret = sendQueue.front();
    sendQueue.pop();
    return ToHex(ret.payload);
}

bool MockLowerLayer::BeginTransmit(const Message& message)
{
    this->sendQueue.push(message);
    return true;
}

void MockLowerLayer::SendUp(const openpal::RSlice& data, const Addresses& addresses)
{
    if (pUpperLayer != nullptr)
    {
        pUpperLayer->OnReceive(Message(addresses, data));
    }
}

void MockLowerLayer::SendUp(const std::string& arHexData, const Addresses& addresses)
{
    HexSequence hs(arHexData);
    this->SendUp(hs.ToRSlice(), addresses);
}

void MockLowerLayer::SendComplete()
{
    if (pUpperLayer != nullptr)
    {
        pUpperLayer->OnTxReady();
    }
}

void MockLowerLayer::ThisLayerUp()
{
    if (pUpperLayer != nullptr)
    {
        pUpperLayer->OnLowerLayerUp();
    }
}
void MockLowerLayer::ThisLayerDown()
{
    if (pUpperLayer != nullptr)
    {
        pUpperLayer->OnLowerLayerDown();
    }
}

} // namespace opendnp3
