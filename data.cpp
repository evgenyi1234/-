#include "database.h"
#include "colorConsole.h"
#include "user.h" 
#include "exept.h"
#include <iostream>
#include <set>



Database::Database()
{
    std::cout << _BLUE << "Ѕаза данных запущена!" << _CLEAR << std::endl;
}

Database::~Database()
{

}

// ƒобавить пользовател - принимает и временные, и существующие объекты
void Database::setUser(std::shared_ptr<User> user) {
    this->usersInChat.insert(std::move(user));       // ѕеремещаем, чтобы избежать копировани€
}

// ѕолучить список всех пользователей
std::set<std::shared_ptr<User>> Database::getAllUsersInChat() const {
    return this->usersInChat;
}

// ѕолучить указатель на пользовател€ по логину (уникален дл€ каждого)
std::shared_ptr<User> Database::getOneUserByLogin(const std::string& login) const {
    //если список пользовватель пуст - nullptr
    if (this->usersInChat.empty())
    {
        return nullptr;
    }

    // ѕроверка, есть ли пользователь с таким логином, если да, то возвращаем указатель на него
    for (const auto& user : usersInChat) {
        if (user->getLogin() == login) {
            return user;
        }
    }

    // не нашли пользовател€ возвращаем nullptr
    return nullptr;
}

// регистраци€ пользовател€
std::shared_ptr<User> Database::regUser(
    const std::string& login,
    const std::string& password,
    const std::string& name
) {
    try {
        if (login.empty() || password.empty() || name.empty())
        {
            throw ErrorCreateUserData(); // исключение, пользователь не создан
        }
        else
        {
            if (this->getOneUserByLogin(login)) {  // если пользовател€ нет, вернет nullptr
                // ѕользователь с таким логином уже существует
                throw ErrorCreateUserExists();
            }
            else
            {
                // создаем пользовател€, все нормально
                // ѕытаемс€ создать пользовател€
                auto user = std::make_shared<User>(login, password, name);
                // ƒобавл€ем копию указател€ в базу данных
                this->usersInChat.insert(user);
                // ¬озвращаем указатель на созданного юзера
                return user;
            }

        }

    }
    catch (const ErrorCreateUserExists& e) {  //обрабатываем исключени€ от класса User, дополн€ их
        std::cerr << _RED << "( од ошибки 1) ќшибка регистрации: " << e.what() << _CLEAR << std::endl;
    }
    catch (const ErrorCreateUserData& e) {    // cerr стандартный поток дл€ вывода ошибок и диагностических сообщений
        std::cerr << _RED << "( од ошибки 2) ќшибка данных: " << e.what() << _CLEAR << std::endl;
    }
    return nullptr;
}


// ¬озвращает shared_ptr<User> при успехе или nullptr при ошибке
std::shared_ptr<User> Database::autorizUser(
    const std::string& login,
    const std::string& password
) {
    auto user = this->getOneUserByLogin(login); // нашли логин в базе данных
    if (user.get())                             // если не nullptr
    {
        if (user->getPass() == password)        //если пароль совпадает логин уже и так совпал, нашелс€ же пользователь
        {
            return user;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}