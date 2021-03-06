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

package com.automatak.dnp3.enums;
/**
* Enumerates possible remote operations on users
*/
public enum UserOperation
{
  OP_ADD(0x1),
  OP_DELETE(0x2),
  OP_CHANGE(0x3),
  OP_UNDEFINED(0xFF);

  private final int id;

  public int toType()
  {
    return id;
  }

  UserOperation(int id)
  {
    this.id = id;
  }

  public static UserOperation fromType(int arg)
  {
    switch(arg)
    {
      case(0x1):
        return OP_ADD;
      case(0x2):
        return OP_DELETE;
      case(0x3):
        return OP_CHANGE;
      default:
        return OP_UNDEFINED;
    }
  }
}
