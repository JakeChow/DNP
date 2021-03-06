//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "opendnp3/gen/UserOperation.h"

namespace opendnp3 {

uint8_t UserOperationToType(UserOperation arg)
{
  return static_cast<uint8_t>(arg);
}
UserOperation UserOperationFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return UserOperation::OP_ADD;
    case(0x2):
      return UserOperation::OP_DELETE;
    case(0x3):
      return UserOperation::OP_CHANGE;
    default:
      return UserOperation::OP_UNDEFINED;
  }
}
char const* UserOperationToString(UserOperation arg)
{
  switch(arg)
  {
    case(UserOperation::OP_ADD):
      return "OP_ADD";
    case(UserOperation::OP_DELETE):
      return "OP_DELETE";
    case(UserOperation::OP_CHANGE):
      return "OP_CHANGE";
    default:
      return "OP_UNDEFINED";
  }
}

}
