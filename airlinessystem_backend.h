#ifndef AIRLINESSYSTEM_BACKEND_H_
#define AIRLINESSYSTEM_BACKEND_H_

#include "airline_payment_api.h"
#include "airlineFlight.h"
#include "airline_payment_card.h"
#include "airlines_user.h"
#include "utils.h"
#include "airline_customer.h"
#include "db_connection.h"
class AirlinesSystemBackend
{
private:
    vector<IFlighsManager *> flights_manager;
    IPayment *paymnent_helper;

public:
    AirlinesSystemBackend(const AirlinesSystemBackend &) = delete;
    void operator=(const AirlinesSystemBackend &) = delete;

    AirlinesSystemBackend()
    {
        flights_manager = FlightsFactory::GetManagers();
        paymnent_helper = PaymentFactory::GetPaymentHelper();
    }
    vector<Flight> FindFlights(const FlightRequest &request) const
    {
        vector<Flight> flights;

        for (IFlighsManager *manager : flights_manager)
        {
            manager->SetFlightRequest(request);
            vector<Flight> more_flights = manager->SearchFlights();

            flights.insert(flights.end(), more_flights.begin(), more_flights.end());
        }
        cout << "flight db founded!\n";
        return flights;
    }
    bool SaveReservation(const Reservation &reservation, const std::string &user_name, const Ticket &ticket)
    {
        const FlightReservation &flight_res = dynamic_cast<const FlightReservation &>(reservation);
        const Flight &flight = flight_res.GetFlight();
        const FlightRequest &request = flight_res.GetRequest();

        SQLHSTMT hStmt;
        SQLRETURN ret;

        // 1. Allocate statement handle
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db_response::GethDbc(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << " Failed to allocate SQL statement handle." << std::endl;
            return false;
        }

        // 2. SQL Insert Statement
        const SQLWCHAR *sql =
            L"INSERT INTO flight_reservations "
            L"(username, ticket_id, airline_name, from_location, to_location, date_time_from, date_time_to, total_cost) "
            L"VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

        // 3. Prepare statement
        ret = SQLPrepareW(hStmt, (SQLWCHAR *)sql, SQL_NTS);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            std::cerr << " SQLPrepare failed." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }

        // 4. Convert strings to wide (SQLWCHAR)
        auto to_wide = [](const std::string &s) -> std::wstring
        {
            return std::wstring(s.begin(), s.end());
        };

        std::wstring userW = to_wide(user_name);
        std::wstring ticketW = to_wide(ticket.GetTicketId());
        std::wstring airlineW = to_wide(flight.GetAirlineName());
        std::wstring fromW = to_wide(request.GetFrom());
        std::wstring toW = to_wide(request.GetTo());
        std::wstring dtFromW = to_wide(flight.GetDateTimeFrom());
        std::wstring dtToW = to_wide(flight.GetDateTimeTo());
        double total_cost = flight.GetTotalCost();

        // 5. Bind parameters
        SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)userW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)ticketW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)airlineW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)fromW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, (SQLPOINTER)toW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_TYPE_TIMESTAMP, 0, 0, (SQLPOINTER)dtFromW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_TYPE_TIMESTAMP, 0, 0, (SQLPOINTER)dtToW.c_str(), 0, NULL);
        SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_FLOAT, 0, 0, &total_cost, 0, NULL);

        // 6. Execute the insert
        ret = SQLExecute(hStmt);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
        {
            std::cout << " Reservation confirmed and saved to database." << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return true;
        }
        else
        {
            SQLWCHAR sqlState[1024], message[1024];
            SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, sqlState, NULL, message, 1024, NULL);
            std::wcerr << L" Insert failed: " << message << L" (SQLState: " << sqlState << L")" << std::endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }
    }

    bool ChargeCost(double cost, PaymentCard &payment_card) const
    {
        CreditCard *creditCard = nullptr;
        DebitCard *debitCard = nullptr;

        if ((creditCard = dynamic_cast<CreditCard *>(&payment_card)))
            paymnent_helper->SetUserInfo(payment_card.GetOwnerName(), "");
        else if ((debitCard = dynamic_cast<DebitCard *>(&payment_card)))
        {
            paymnent_helper->SetUserInfo(payment_card.GetOwnerName(), debitCard->GetBillingAddress());
        }
        paymnent_helper->SetCardInfo(payment_card.GetCardNumber(), payment_card.GetExpiryDate(), payment_card.GetSecurityCode());
        bool payment_status = paymnent_helper->MakePayment(cost);

        if (!payment_status)
            return false; // Don't reserve this flight
        return true;
    }
    bool UnChargeCost(double cost, const PaymentCard &payment_card) const
    {
        return true;
    }

 

    bool ConfirmRerservation(const Reservation &reservation)
    {
        // we can think of factory of factories to maybe be able to make this generic
        // But let's make your life easier at this stage

        FlightReservation *flight = nullptr;
        Reservation *reservationCpy = reservation.Clone();

        if ((flight = dynamic_cast<FlightReservation *>(reservationCpy)))
        {
            string name = flight->GetFlight().GetAirlineName();
            IFlighsManager *mgr = FlightsFactory::GetManager(name);

            if (mgr != nullptr && mgr->ReserveFlight(*flight))
                return true;

            return false;
        }

        ItineraryReservation *itiniary = nullptr;
        if ((itiniary = dynamic_cast<ItineraryReservation *>(reservationCpy)))
        {
            vector<Reservation *> confirmed_reservations;

            for (Reservation *sub_reservation : itiniary->GetReservations())
            {
                bool is_confirmed = ConfirmRerservation(*sub_reservation);

                if (is_confirmed)
                    confirmed_reservations.push_back(sub_reservation);
                else
                {
                    // If failed to reserve, cancel all what is confirmed so far!
                    for (Reservation *conf_reservation : confirmed_reservations)
                        // CancelReservation(*conf_reservation);
                        return false;
                }
            }
        }
        else
            assert(false);

        // Intentional for education: there is memory leak
        // The above returs won't come here to remove this memory!
        // Always be sure you can return in middle of function or not. Better not if u have pointers

        delete reservationCpy;
        reservationCpy = nullptr;
        return true;
    }
    bool updateReservation(const Reservation &reservation, const Customer &customer, const PaymentCard &payment_card)
    {
        SQLHSTMT hStmt = nullptr;
        SQLRETURN ret;

        try
        {
            std::unique_ptr<Reservation> reservationCpy(reservation.Clone());
            FlightReservation *flight = dynamic_cast<FlightReservation *>(reservationCpy.get());
            Ticket *ticket = dynamic_cast<Ticket *>(reservationCpy.get());

            std::string reservation_id;
            if (flight)
                reservation_id = flight->GetFlight().GetFlightId();
            else if (ticket)
                reservation_id = ticket->GetTicketId();
            else
            {
                std::cerr << " Invalid reservation type." << std::endl;
                return false;
            }

            std::string username = customer.GetName();
            std::string owner_name = payment_card.GetOwnerName();
            std::string card_number = payment_card.GetCardNumber();
            std::string expiry_date = payment_card.GetExpiryDate();
            int security_code = payment_card.GetSecurityCode();

            // Convert strings to wide strings (UTF-16)
            auto to_wide = [](const std::string &s) -> std::wstring
            {
                return std::wstring(s.begin(), s.end());
            };

            std::wstring w_username = to_wide(username);
            std::wstring w_owner_name = to_wide(owner_name);
            std::wstring w_card_number = to_wide(card_number);
            std::wstring w_expiry_date = to_wide(expiry_date);
            std::wstring w_reservation_id = to_wide(reservation_id);

            // Allocate statement handle
            ret = SQLAllocHandle(SQL_HANDLE_STMT, db_response::GethDbc(), &hStmt);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
            {
                std::cerr << " Failed to allocate statement handle." << std::endl;
                return false;
            }

            // Use Unicode-aware SQL statement (SQLWCHAR[])
            const SQLWCHAR *sql = L"UPDATE confirmeReservations SET "
                                  L"username=?, owner_name=?, card_number=?, expiry_date=?, security_code=? "
                                  L"WHERE reservation_id=?";

            ret = SQLPrepareW(hStmt, (SQLWCHAR *)sql, SQL_NTS);
            if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
            {
                std::cerr << " SQLPrepareW failed." << std::endl;
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                return false;
            }

            // Bind parameters with proper length indicators (SQL_NTS for null-terminated)
            SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0,
                             (SQLPOINTER)w_username.c_str(), 0, nullptr);

            SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0,
                             (SQLPOINTER)w_owner_name.c_str(), 0, nullptr);

            SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 30, 0,
                             (SQLPOINTER)w_card_number.c_str(), 0, nullptr);

            SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0,
                             (SQLPOINTER)w_expiry_date.c_str(), 0, nullptr);

            SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0,
                             (SQLPOINTER)&security_code, 0, nullptr);

            SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 50, 0,
                             (SQLPOINTER)w_reservation_id.c_str(), 0, nullptr);

            ret = SQLExecute(hStmt);
            if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
            {
                SQLLEN rows = 0;
                SQLRowCount(hStmt, &rows);
                if (rows > 0)
                {
                    std::wcout << L" Reservation updated successfully." << std::endl;
                    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                    return true;
                }
                else
                {
                    std::wcout << L" No rows updated. Check reservation_id." << std::endl;
                    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
                    return false;
                }
            }
            else
            {
                SQLWCHAR state[6], message[1024];
                SQLINTEGER native_error;
                SQLSMALLINT msg_len;
                SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, state, &native_error, message, 1024, &msg_len);
                std::wcerr << L" SQL Error [" << state << L"]: " << message << std::endl;
            }

            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }
        catch (...)
        {
            std::cerr << " Exception thrown in updateReservation()" << std::endl;
            if (hStmt)
                SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return false;
        }
    }
};

#endif