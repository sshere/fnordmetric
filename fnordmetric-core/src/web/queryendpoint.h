/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2011-2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef _FNORDMETRIC_QUERYENDPOINT_H
#define _FNORDMETRIC_QUERYENDPOINT_H
#include <memory>
#include <fnordmetric/http/httphandler.h>
#include <fnordmetric/http/httprequest.h>
#include <fnordmetric/http/httpresponse.h>

namespace fnordmetric {
namespace web {

class QueryEndpoint : public http::HTTPHandler {
public:

  static std::unique_ptr<http::HTTPHandler> getHandler();

  bool handleHTTPRequest(
      http::HTTPRequest* request,
      http::HTTPResponse* response) override;

};

}
}
#endif