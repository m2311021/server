#include <iostream>
#include <memory>
#include <string_view>
#include <drogon/drogon.h>
#include <userController.hpp>

import user;
import tools;

int main() {
    // Установить адрес и порт HTTP-слушателя
    drogon::app().addListener("0.0.0.0", 8080);

    // Загрузить файл конфигурации (если нужно)
    // drogon::app().loadConfigFile("../config.json");

    // Регистрация обработчика маршрута "/form"
    drogon::app().registerHandler("/form",
        [](const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
                // Создаем экземпляр контроллера и вызываем метод
                userController uc;
                uc.get_form(req, std::move(callback)); // Вызов метода get_form
        });

    // Запуск HTTP-фреймворка
    drogon::app().run();
}
