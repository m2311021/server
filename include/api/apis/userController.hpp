#include <drogon/drogon.h>
#include <drogon/HttpSimpleController.h>

import user;

using namespace drogon;

class userController : public HttpController<userController> {
public:
    METHOD_LIST_BEGIN
    METHOD_ADD(userController::get_user, "/userController", Get);
    METHOD_ADD(userController::upload_user, "/userController", Put);
    METHOD_ADD(userController::submit_user, "/submit", Post);
    METHOD_ADD(userController::get_form, "/getform", Get);
    METHOD_ADD(userController::hello_world, "/hello", Get);
    METHOD_LIST_END
    void get_form(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);
    void submit_user(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);
    void hello_world(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);
protected:
    void get_user(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);

    void upload_user(const HttpRequestPtr&,
        std::function<void(const HttpResponsePtr&)>&& callback);
};