#include <iostream>
#include <memory>
#include <string_view>
#include <drogon/drogon.h>
#include <userController.hpp>

import user;
import tools;

int main() {
    // ���������� ����� � ���� HTTP-���������
    drogon::app().addListener("0.0.0.0", 8080);

    // ��������� ���� ������������ (���� �����)
    // drogon::app().loadConfigFile("../config.json");

    // ����������� ����������� �������� "/form"
    drogon::app().registerHandler("/form",
        [](const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                // ������� ��������� ����������� � �������� �����
                userController uc;
                uc.get_form(req, std::move(callback)); // ����� ������ get_form
        });

    // ������ HTTP-����������
    drogon::app().run();
}
