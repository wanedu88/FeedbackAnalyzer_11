#include "httplib.h"
#include "Constants.h"
#include "FileHandler.h"
#include "Logger.h"
#include "AppConfig.h"
#include "handlers/Handlers.h"
#include "services/TrendStore.h"

static FileHandler fileHandler;

int main() {
    Constants::init();
    TrendStore::loadAtStartup();

    httplib::Server svr;

    svr.Get("/", handleGetRoot);
    svr.Post("/analyze", handlePostAnalyze);
    svr.Post("/upload", handlePostUpload);
    svr.Post("/filter", handlePostFilter);
    svr.Get("/download", handleGetDownload);

    Logger::logInfo(u8"서버가 http://localhost:" + std::to_string(AppConfig::kServerPort) +
                    u8" 에서 시작됩니다.");
    svr.listen(AppConfig::kServerHost, AppConfig::kServerPort);

    return 0;
}
