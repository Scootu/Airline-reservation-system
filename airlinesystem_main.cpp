#include <iostream>
#include <vector>
#include "db_connection.h"
using namespace std;

#include <sstream>
#include "airlines_user_mgr.h"
#include "airline_customer_mgr.h"
#include "airline_customer.view.h"

class AirlineFrontend
{
private:
    UsersManager *users_manager;
    AirlinesSystemBackend *airline_manager;
    CustomerManager *customer_manager = nullptr;

    void LoadDataBase()
    {
        users_manager->LoadDataBase();
    }

public:
    AirlineFrontend(const AirlineFrontend &) = delete;
    void operator=(const AirlineFrontend &) = delete;

    AirlineFrontend() : users_manager(new UsersManager()), airline_manager(new AirlinesSystemBackend())
    {
    }
    ~AirlineFrontend()
    {
        cout << "Destructor: AirlineFrontend\n";

        if (users_manager != nullptr)
        {
            delete users_manager;
            users_manager = nullptr;
        }
    }

    void Run()
    {
        LoadDataBase();

        while (true)
        {
            users_manager->AccessSystem();

            if (users_manager->GetCurrentUser()->GetType() == UserType::CustumerUser)
            {
                customer_manager = new CustomerManager(users_manager->GetCurrentUser(), *airline_manager);
                CustomerView view(*customer_manager);
                view.Display();
            }
            else
                assert(false);
        }
    }
};

// As a project
// Add more flight classes + later operator overloading something + Simulators + logging + integration/unit text + its exception hierarchy
// Singelton loading DB config for flights
// Fawy project maybe with adapters + cragslist

int main() {
    cout << " Program started." << endl;

    cout << "Connecting to database..." << endl;
     db_response::ConnectionFunction();  // Make sure this doesn't crash

    cout << "Checking DB connection..." << endl;
    if (!db_response::isConnected()) {
        cerr << " Cannot start application: Database connection failed." << endl;
        system("pause"); // <- Keeps the window open
        return 1;
    }

    cout << " DB connection successful!" << endl;

    AirlineFrontend site;
    site.Run();

    db_response::CloseConnection();

    cout << " Program ended normally." << endl;
    system("pause"); // <--- important to keep the console open

    return 0;
}