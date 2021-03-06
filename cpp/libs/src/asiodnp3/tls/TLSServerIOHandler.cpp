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

#include "asiodnp3/tls/TLSServerIOHandler.h"

#include "openpal/logging/LogMacros.h"

#include "asiopal/tls/TLSStreamChannel.h"

#include "opendnp3/LogLevels.h"

#include <utility>

using namespace opendnp3;
using namespace asiopal;

namespace asiodnp3
{

void TLSServerIOHandler::Server::AcceptStream(uint64_t /*sessionid*/,
                                              const std::shared_ptr<asiopal::Executor>& executor,
                                              std::shared_ptr<asio::ssl::stream<asio::ip::tcp::socket>> stream)
{
    this->callback(asiopal::TLSStreamChannel::Create(executor, stream));
}

TLSServerIOHandler::TLSServerIOHandler(const openpal::Logger& logger,
                                       ServerAcceptMode mode,
                                       const std::shared_ptr<IChannelListener>& listener,
                                       std::shared_ptr<asiopal::Executor> executor,
                                       asiopal::IPEndpoint endpoint,
                                       asiopal::TLSConfig config,
                                       std::error_code& /*ec*/)
    : IOHandler(logger, mode == ServerAcceptMode::CloseExisting, listener),
      executor(std::move(executor)),
      endpoint(std::move(endpoint)),
      config(std::move(config))
{
}

void TLSServerIOHandler::ShutdownImpl()
{
    if (this->server)
    {
        this->server->Shutdown();
        this->server.reset();
    }
}

void TLSServerIOHandler::BeginChannelAccept()
{
    auto callback = [self = shared_from_this(), this](const std::shared_ptr<asiopal::IAsyncChannel>& channel) {
        this->OnNewChannel(channel);
    };

    std::error_code ec;
    this->server = std::make_shared<Server>(this->logger, this->executor, this->endpoint, this->config, ec);

    if (ec)
    {
        SIMPLE_LOG_BLOCK(this->logger, flags::ERR, ec.message().c_str());
    }
    else
    {
        this->server->StartAcceptingConnection(callback, ec);
        FORMAT_LOG_BLOCK(this->logger, flags::ERR, "Unable to begin accepting connections: %s", ec.message().c_str());
    }
}

void TLSServerIOHandler::SuspendChannelAccept()
{
    if (this->server)
    {
        this->server->Shutdown();
        this->server.reset();
    }
}

} // namespace asiodnp3
