#include <expected>
#include <fstream>
#include <sstream>
#include <drogon/drogon.h>
#include "api/apis/userController.hpp"

using drogonRequestArgument = std::shared_ptr<drogon::HttpResponse>;
using function_result = std::expected<drogonRequestArgument, std::exception>;

// Функция для обработки ошибок
void handle_error(const std::exception& exc, const std::shared_ptr<drogon::HttpResponse>& response,
    const std::function<void(const drogon::HttpResponsePtr&)>& callback,
    const std::string& error_log = "An error occurred!") {
    LOG_ERROR << error_log << ": " << exc.what();
    callback(response);
}

// Функция для обработки запроса по предикату
auto process_user_request(const drogon::HttpRequestPtr& request,
    const std::function<void(const drogon::HttpResponsePtr&)>& callback,
    const std::string& header_to_check,
    std::function<function_result(std::string)>&& predicate,
    std::shared_ptr<drogon::HttpResponse>& default_response,
    const std::string& default_log = "Failed to process request!") {
    if (auto header = request->getHeader(header_to_check); !header.empty()) {
        auto result = predicate(header);

        auto res = result.and_then(
            [&](const drogonRequestArgument& response) -> std::expected<std::string, std::exception> {
                callback(response);
                return "All Okay";
            })
            .or_else([&](const std::exception& exc) -> std::expected<std::string, std::exception> {
                handle_error(exc, default_response, callback, "Processing user request error");
                return std::unexpected(exc);
                });

            if (res.has_value()) {
                LOG_INFO << res.value();
            }
            return;
    }

    LOG_WARN << default_log;
    callback(default_response);
}

// Пример реализации функции для получения информации о пользователе
void userController::get_user(const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("User not found");
    resp->setStatusCode(k404NotFound);

    process_user_request(request, callback, "user_id",
        [](std::string user_id) -> function_result {
            auto user_response = HttpResponse::newHttpResponse();
            user_response->setContentTypeCode(drogon::CT_APPLICATION_JSON);
            user_response->setBody("User data for ID: " + user_id);
            LOG_INFO << "Retrieved user data for user_id: " << user_id;
            return user_response;
        },
        resp);
}

void userController::upload_user(const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("User update failed");
    resp->setStatusCode(k400BadRequest);

    process_user_request(request, callback, "user_id",
        [](std::string user_id) -> function_result {
            // Логика обновления данных пользователя
            auto upload_response = HttpResponse::newHttpResponse();
            upload_response->setBody("User " + user_id + " updated successfully.");
            LOG_INFO << "User data updated for user_id: " << user_id;
            return upload_response;
        },
        resp, "Failed to update user information");
}

void userController::submit_user(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    auto username = req->getParameter("username");
    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::CT_TEXT_HTML);
    if (!username.empty()) {
        resp->setBody("<h1>Привет, " + username + "!</h1>");
    }
    else {
        resp->setBody("<h1>Имя пользователя не указано!</h1>");
    }
    callback(resp);
}


void userController::get_form(const drogon::HttpRequestPtr& request,
    std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    std::string path = "C:/Users/Aleks/Desktop/server/out/build/x64-Debug/form.html";

    std::ifstream file(path);
    if (!file.is_open()) {
        auto response = HttpResponse::newHttpResponse();
        response->setBody("File not found");
        response->setStatusCode(k404NotFound);
        callback(response);
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    auto response = HttpResponse::newHttpResponse();
    response->setBody(buffer.str());
    response->setContentTypeCode(drogon::CT_TEXT_HTML);
    callback(response);
}

void userController::hello_world(const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(k200OK);
    resp->setContentTypeCode(drogon::CT_TEXT_HTML);
    resp->setBody("<h1>Привет, мир!</h1>");

    callback(resp);
}