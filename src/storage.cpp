#include <expected>

#include "api\apis\storage.hpp"

using drogonRequestArgument = std::shared_ptr<drogon::HttpResponse>;
using function_result = std::expected<drogonRequestArgument, std::exception>;

auto process_request_by_predicate(const drogon::HttpRequestPtr& request,
    const std::function<void(const drogon::HttpResponsePtr&)>& callback,
    const std::string& header_to_check,
    std::function<function_result(std::string)>&& predicate,
    std::shared_ptr<drogon::HttpResponse>& default_response,
    const std::string& default_log = "Failed to process request!") {
    if (auto header = request->getHeader(header_to_check); !header.empty()) {
        auto result = predicate(header);

        auto res =
            predicate(header)
            .and_then(
                [&](const drogonRequestArgument& response) -> std::expected<std::string, std::exception> {
                    callback(response);
                    return "All Okay";
                })
            .or_else([&](std::exception exc) -> std::expected<std::string, std::exception> {
                    callback(default_response);
                    return std::unexpected(exc);
                });
                if (res.has_value()) {
                    LOG_INFO << res.value();
                }
                else {
                    LOG_ERROR << res.error().what();
                }

                return;
    }
    LOG_WARN << default_log;

    callback(default_response);
}

void Files::get_file(const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("No Such File");
    resp->setStatusCode(k404NotFound);

    process_request_by_predicate(request,
        callback,
        "file",
        [](std::string header) -> function_result {
            auto file_response = HttpResponse::newFileResponse(header);
            LOG_INFO << "Sending file: " << header;
            return file_response;
        }, resp);
}

void Files::delete_file(const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {


    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("No Such File");
    resp->setStatusCode(k404NotFound);

    process_request_by_predicate(request,
        callback,
        "file",
        [](std::string header) -> function_result {

            auto resp = HttpResponse::newHttpResponse();
            auto file_path = std::filesystem::path(header);

            if (exists(file_path)) {
                LOG_INFO << "Deleting file: " << header;

                if (!std::filesystem::remove(file_path)) {
                    LOG_WARN << "Failed to delete file!";
                }
            }
            else {
                resp->setBody("No Such File. Ending deleting without any execution");
                resp->setStatusCode(k404NotFound);
            }

            return resp;
        }, resp, "File Not Found..");
}


void Files::upload_file(const drogon::HttpRequestPtr& req,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    MultiPartParser fileUpload;
    if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1) {
        auto resp = HttpResponse::newHttpResponse();

        resp->setBody("Must only be one file");
        resp->setStatusCode(k403Forbidden);

        callback(resp);

        return;
    }

    auto& file = fileUpload.getFiles()[0];
    auto md5 = file.getMd5();
    auto resp = HttpResponse::newHttpResponse();

    resp->setBody(
        "The server has calculated the file's MD5 hash to be " + md5);
    file.save();
    LOG_INFO << "The uploaded file has been saved to the ./uploads "
        "directory";
    callback(resp);
}
