/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include <google/protobuf/message.h>

class CDM_DECL PBUtils
{
public:

  static bool SerializeFromFile(const std::string& filename, google::protobuf::Message& dst, Logger* logger);
  static bool SerializeToFile(const google::protobuf::Message& src, const std::string& filename, Logger* logger);

  static bool SerializeFromString(const std::string& src, google::protobuf::Message& dst, eSerializationFormat m, Logger* logger);
  static bool SerializeToString(const google::protobuf::Message& src, std::string& output, eSerializationFormat m, Logger* logger);

  static void LogError(const std::string& method, const std::string& err, Logger* logger);
};