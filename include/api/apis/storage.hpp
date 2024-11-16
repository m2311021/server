#pragma once

#include <memory>
#include <iostream>
#include <expected>

#include <drogon/drogon.h>
#include <drogon/HttpSimpleController.h>

using namespace drogon;

class Files : public drogon::HttpController<Files> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(Files::get_file, "/file", Get);
    METHOD_ADD(Files::upload_file, "/file", Put);
    METHOD_ADD(Files::delete_file, "/file", Delete);
    METHOD_LIST_END

protected:
    void get_file(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void upload_file(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void delete_file(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);
};