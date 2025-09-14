#include "interactionChatHared.h"
#include "database.h"
#include "user.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include "colorConsole.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits> //ƒл€  cin.ignore(numeric_limits<streamsize>::max(), '\n');
using namespace std;


//открывает общий чат, получает от пользовател€ сообщен€ при "0" - выход, передать базу данных, залогированнного пользовател€, общий чат
void openChatHared(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatHared>& chatP) {
    cout << _GREEN << "«ашли в общий чат" << _CLEAR << endl;
    string userInput;                                                       // ¬водимое пользователем знначение
    while (userInput != "0")
    {
        cout << _GREY_BG << "\n\n\t\t" << chatP->getNameChat() << " - всего пользователей " << db->getAllUsersInChat().size() << _GREY_BG << "\n" << _CLEAR << endl;
        cout << chatP->getAllMessage(userAuthorization) << endl << endl;
        cout << _CYAN << "Ќапишите сообщение:" << _CLEAR << endl
            << "0 - отправте \"0\" чтобы выйти" << endl;
        std::getline(std::cin >> std::ws, userInput);                       // „итаем всю строку
        if (userInput != "0")                                               // при выходе пропускаем запись
        {
            chatP->addMessage(userAuthorization, userInput);
        }
    }
