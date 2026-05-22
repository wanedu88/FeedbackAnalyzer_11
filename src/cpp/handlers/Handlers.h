#pragma once

#include "httplib.h"

void handleGetRoot(const httplib::Request& req, httplib::Response& res);
void handlePostAnalyze(const httplib::Request& req, httplib::Response& res);
void handlePostUpload(const httplib::Request& req, httplib::Response& res);
void handlePostFilter(const httplib::Request& req, httplib::Response& res);
void handleGetDownload(const httplib::Request& req, httplib::Response& res);
