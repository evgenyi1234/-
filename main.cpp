// разное 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include "database.h"
#include "colorConsole.h"
#include "user.h"
#include "chatPrivate.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include "interactionChatPrvate.h"
#include "interactionChatHared.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <limits> //Для  cin.ignore(numeric_limits<streamsize>::max(), '\n');
#include <clocale> //для правильной лолокализации
#include <locale>
#include "hashPass.h"


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN  // Уменьшает количество включаемых заголовков Windows
#include <windows.h>        // Используется для настройки консоли
#endif

using namespace std;




void chatStart(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatHared>& haredChat) {
    char menu = '9';

    while (menu != '0')
    {
        cout << _GREY_BG << "\n\n\t\tЗдраствуйте - " << userAuthorization->getName() << "\n" << _CLEAR << endl << endl;
        cout << "Для выбора введите значение." << endl
            << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Выход в главное меню;" << endl
            << "1 - Открыть список приватных чатов - написать пользователю;" << endl
            << "2 - Открыть общий чат" << endl
            << "Ведите значение" << endl;
        // Обработка ввода
        if (!(cin >> menu)) {
            cin.clear(); // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            menu = '9'; // Предотвращаем выход из цикла
            continue;
        }

        switch (menu)
        {
        case '0':
            cout << _GREEN << "Возврат в главное меню!" << _CLEAR;
            return;
        case '1':
            menuChatPrivate(db, userAuthorization);
            break;
        case '2':
            openChatHared(db, userAuthorization, haredChat);
            break;
        default:
            cout << _YELLOW << "Не верно введено значение, попробуй еще раз" << _CLEAR << endl;
            break;
        }
    }

}

void authorization(shared_ptr<Database>& db, shared_ptr<ChatHared>& haredChat) {
    cout << _GREY_BG << "\n\n\t\tАвторизация\n" << _CLEAR << endl << endl;
    string login;
    string pass;

    cin.clear(); // Сбрасывает флаги ошибок
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищает буфер до новой строки

    cout << "Введите логин" << endl;
    getline(cin, login);

    cout << "Введите пароль" << endl;
    getline(cin, pass);

    auto userAuthorization = db->autorizUser(login, hashPassword(pass));
    if (!userAuthorization)
    {
        cout << _MAGENTA << "Не удалось авторизоваться, возврат в главное меню" << _CLEAR << endl;
        return;
    }
    else
    {
        cout << _GREEN << "Авторизация выполнена!" << _CLEAR;
        chatStart(db, userAuthorization, haredChat);
    }

}

void reg(shared_ptr<Database>& db, shared_ptr<ChatHared>& haredChat) {
    cout << _GREY_BG << "\n\n\t\tРегистрация\n" << _CLEAR << endl << endl;
    string login;
    string pass;
    string name;
    cin.clear(); // Сбрасывает флаги ошибок
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищает буфер до новой строки

    cout << "Введите логин" << endl;
    getline(cin, login);

    cout << "Введите пароль" << endl;
    getline(cin, pass);

    cout << "Введите свое имя" << endl;
    getline(cin, name);
    auto userAuthorization = db->regUser(login, hashPassword(pass), name);
    if (!userAuthorization)
    {
        cout << _MAGENTA << "Не удалось зарегистрироваться, возврат в главное меню" << _CLEAR;
        return;
    }
    else
    {
        cout << name << _GREEN << " - Вы успешно зарегистрировались" << _CLEAR;
        chatStart(db, userAuthorization, haredChat);
    }
}


int main(int argc, char const* argv[])
{

    // Универсальная настройка локали
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // Настройки для Windows
    // исправляет не коректную запись в string русских символов в консоле
    // распространяет локализацию на весь проект 
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Для Linux/Mac
    // был ли определен SET_GLOBAL_LOCALE_LINUX ? да (в cmake) или нет (в cmake)
#ifdef SET_GLOBAL_LOCALE_LINUX
    std::locale::global(std::locale("ru_RU.UTF-8"));
#endif

    char menu = '9';
    shared_ptr<Database> database(new Database());
    shared_ptr<ChatHared> haredChat(new ChatHared("Общий чат"));
    cout << _GREEN << "Готов к работе" << _CLEAR << std::endl;

    while (menu != '0')
    {
        cout << _GREY_BG << "\n\n\t\tДобо пожаловать в чат!\n" << _CLEAR << endl << endl;
        cout << "Для выбора введите значение." << endl
            << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Закрыть приложение;" << endl
            << "1 - Авторизация;" << endl
            << "2 - Регистрация" << endl
            << "Ведите значение" << endl;

        // Обработка ввода
        if (!(cin >> menu)) {
            cin.clear();                                                // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');        // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            menu = '9';                                                 // Предотвращаем выход из цикла
            continue;
        }

        switch (menu)
        {
        case '0':
            cout << _GREEN << "Досвидания!" << _CLEAR;
            return 0;
        case '1':
            authorization(database, haredChat);
            break;
        case '2':
            reg(database, haredChat);
            break;
        default:
            cout << _YELLOW << "Не верно введено значение, попробуй еще раз" << _CLEAR << endl;
            break;
        }
    }
    return 0;
}