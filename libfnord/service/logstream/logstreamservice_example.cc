/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * FnordMetric is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License v3.0. You should have received a
 * copy of the GNU General Public License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include "fnord/net/http/httprouter.h"
#include "fnord/net/http/httpserver.h"
#include "fnord/json/jsonrpc.h"
#include "fnord/logging/logoutputstream.h"
#include "fnord/json/jsonrpchttpadapter.h"
#include "fnord/service/logstream/logstreamservice.h"
#include "fnord/service/logstream/logstreamserviceadapter.h"
#include "fnord/thread/eventloop.h"
#include "fnord/thread/threadpool.h"
#include "fnord/system/signalhandler.h"

using fnord::json::JSONRPC;
using fnord::json::JSONRPCHTTPAdapter;
using fnord::logstream_service::LogStreamService;
using fnord::logstream_service::LogStreamServiceAdapter;

int main() {
  fnord::system::SignalHandler::ignoreSIGHUP();
  fnord::system::SignalHandler::ignoreSIGPIPE();

  fnord::CatchAndAbortExceptionHandler ehandler;
  ehandler.installGlobalHandlers();

  fnord::log::LogOutputStream logger(fnord::io::OutputStream::getStderr());
  fnord::log::Logger::get()->setMinimumLogLevel(fnord::log::kDebug);
  fnord::log::Logger::get()->listen(&logger);

  JSONRPC rpc;
  JSONRPCHTTPAdapter rpc_http(&rpc);

  LogStreamService logstream_service;
  LogStreamServiceAdapter::registerJSONRPC(&logstream_service, &rpc);

  fnord::http::HTTPRouter http_router;
  http_router.addRouteByPrefixMatch("/rpc", &rpc_http);

  fnord::thread::EventLoop event_loop;
  fnord::thread::ThreadPool thread_pool;
  fnord::http::HTTPServer http_server(&http_router, &event_loop);
  http_server.listen(8080);

  event_loop.run();
  return 0;
}

