/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

PUSH_PROTO_WARNINGS
#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
POP_PROTO_WARNINGS

class CDM_DECL PBUtils
{
public:

  static bool SerializeFromFile(const std::string& filename, google::protobuf::Message& dst, Logger* logger);
  static bool SerializeToFile(const google::protobuf::Message& src, const std::string& filename, Logger* logger);

  static bool SerializeFromString(const std::string& src, google::protobuf::Message& dst, eSerializationFormat m, Logger* logger);
  static bool SerializeToString(const google::protobuf::Message& src, std::string& output, eSerializationFormat m, Logger* logger);

  static google::protobuf::util::JsonPrintOptions PrintOpts(bool verbose=true);

  static void LogError(const std::string& method, const std::string& err, Logger* logger);
};