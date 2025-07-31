#ifndef AIRLINEFLIGHT_H_
#define AIRLINEFLIGHT_H_

#include "airline_common_reservation.h"

enum class SeatType
{
    economic = 0,
    bussiness = 1
};
enum class TicketType
{
    economic = 0,
    VIB = 1
};
class AirCanadaCustomerInfo
{
};

class AirCanadaFlight
{
public:
    double price;
    string date_time_from;
    string date_time_to;
};

class AirCanadaOnlineAPI
{
public:
    static vector<AirCanadaFlight> GetFlights(const string &from, const string &from_date,
                                              const string &to, const string &to_date,
                                              int adults, int children)
    {
        vector<AirCanadaFlight> flights;

        if (!db_response::isConnected())
        {
            cerr << " Not connected to database!" << endl;
            return flights;
        }
        cout << "Start getflights "<<endl;
        SQLHSTMT hStmt;
        SQLRETURN ret;

        // Allocate statement handle
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db_response::GethDbc(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cerr << " Failed to allocate SQL statement handle." << endl;
            return flights;
        }

        // Prepare SQL query
        const wchar_t *query = L"SELECT date_time_from, date_time_to, price FROM aircanada_flights";

        ret = SQLExecDirectW(hStmt, (SQLWCHAR *)query, SQL_NTS);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            SQLWCHAR state[1024], message[1024];
            SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, state, NULL, message, 1024, NULL);
            wcerr << L" Query failed: " << message << L" (SQLState: " << state << L")" << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return flights;
        }

        // Bind columns
        SQLWCHAR date_from[64], date_to[64];
        SQLDOUBLE price;

        SQLBindCol(hStmt, 1, SQL_C_WCHAR, date_from, sizeof(date_from), NULL);
        SQLBindCol(hStmt, 2, SQL_C_WCHAR, date_to, sizeof(date_to), NULL);
        SQLBindCol(hStmt, 3, SQL_C_DOUBLE, &price, 0, NULL);

        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            AirCanadaFlight flight;
            flight.date_time_from = WCharToString(date_from);
            flight.date_time_to = WCharToString(date_to);
            flight.price = price;
            flights.push_back(flight);
        }

        // Cleanup
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return flights;
    }

    static bool ReserveFlight(const AirCanadaFlight &flight, const AirCanadaCustomerInfo &info)
    {
        return true;
    }
};

class TurkishFlight
{
public:
    double cost;
    string datetime_from;
    string datetime_to;
};

class TurkishCustomerInfo
{
};

class TurkishAirlinesOnlineAPI
{
public:
    void SetFromToInfo(string datetime_from, string from, string datetime_to, string to)
    {
    }
    void SetPassengersInfo(int infants, int childern, int adults)
    {
    }

    vector<TurkishFlight> GetAvailableFlights() const
    {
        vector<TurkishFlight> flights;

        if (!db_response::isConnected())
        {
            cerr << "❌ Not connected to database!" << endl;
            return flights;
        }

        SQLHSTMT hStmt;
        SQLRETURN ret;

        // Allocate statement
        ret = SQLAllocHandle(SQL_HANDLE_STMT, db_response::GethDbc(), &hStmt);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            cerr << "❌ Failed to allocate SQL statement handle." << endl;
            return flights;
        }

        const SQLWCHAR *query = L"SELECT date_time_from, date_time_to, price FROM turkishairlines_flights";

        // Execute query
        ret = SQLExecDirectW(hStmt, (SQLWCHAR *)query, SQL_NTS);
        if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
        {
            SQLWCHAR state[1024], message[1024];
            SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, state, NULL, message, 1024, NULL);
            wcerr << L"❌ Query failed: " << message << L" (SQLState: " << state << L")" << endl;
            SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
            return flights;
        }

        // Output variables
        SQLWCHAR datetime_from[64], datetime_to[64];
        SQLDOUBLE price;

        // Bind columns
        SQLBindCol(hStmt, 1, SQL_C_WCHAR, datetime_from, sizeof(datetime_from), NULL);
        SQLBindCol(hStmt, 2, SQL_C_WCHAR, datetime_to, sizeof(datetime_to), NULL);
        SQLBindCol(hStmt, 3, SQL_C_DOUBLE, &price, 0, NULL);

        // Fetch results
        while (SQLFetch(hStmt) == SQL_SUCCESS)
        {
            TurkishFlight flight;
            flight.datetime_from = WCharToString(datetime_from);
            flight.datetime_to = WCharToString(datetime_to);
            flight.cost = price;
            flights.push_back(flight);
        }

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return flights;
    }

    static bool ReserveFlight(const TurkishCustomerInfo &info, const TurkishFlight &flight)
    {
        return false;
    }
};

class Seat
{
private:
    static int seat_number;
    double cost = 0;
    SeatType type = SeatType::economic;

public:
    Seat()
    {
        seat_number++;
    }
    SeatType GetSeatType() const
    {
        return type;
    }
    void SetSeatType(SeatType type)
    {
        this->type = type;
    }
    int GetSeatNumber() const
    {
        return seat_number;
    }
    void SetSeatNumber(int seatNumber)
    {
        this->seat_number = seatNumber;
    }
};
int Seat::seat_number = 1;
class Flight
{
private:
    string airline_name;
    double total_cost = 0;
    string date_time_from;
    string date_time_to;
    string flight_id = "PCK_LM";

public:
    string ToString() const
    {
        ostringstream oss;
        oss << "Airline :" << airline_name << " Cost: " << total_cost << " Departue Date " << date_time_from << " Arrival date " << date_time_to;
        return oss.str();
    }
    const string &GetDateTimeFrom() const
    {
        return date_time_from;
    }
    const string &GetFlightId() const
    {
        return flight_id;
    }
    int GetTotalCost() const
    {
        return 100;
    }
    void SetDateTimeFrom(const string &datetime_from)
    {
        date_time_from = datetime_from;
    }
    const string &GetDateTimeTo() const
    {
        return date_time_to;
    }
    void SetDateTimeTo(const string &datetime_to)
    {
        date_time_to = datetime_to;
    }

    void SetTotalCost(int val)
    {
        this->total_cost = val;
    }
    const string &GetAirlineName() const
    {
        return airline_name;
    }
    void SetAirlineName(const string &airlineName)
    {
        airline_name = airlineName;
    }
};

class FlightRequest
{
protected:
    string datetime_from;
    string from;
    string datetime_to;
    string to;
    int infants;
    int children;
    int adults;

public:
    string GetDatePartFrom() const
    {
        return datetime_from;
    }
    string GetDatePartTo() const
    {
        return datetime_to;
    }
    virtual int GetAdults() const
    {
        return adults;
    }
    virtual void SetAdults(int adults)
    {
        this->adults = adults;
    }
    virtual int GetChildren() const
    {
        return children;
    }
    virtual void SetChildren(int children)
    {
        this->children = children;
    }
    virtual int GetInfants() const
    {
        return infants;
    }
    virtual void SetInfants(int infants)
    {
        this->infants = infants;
    }
    virtual const string &GetDateTimeFrom() const
    {
        return datetime_from;
    }
    virtual void SetDateTimeFrom(const string &datetimefrom)
    {
        datetime_from = datetimefrom;
    }
    virtual const string &GetDateTimeTo() const
    {
        return datetime_to;
    }
    virtual void SetDateTimeTO(const string &datetimeto)
    {
        this->datetime_to = datetimeto;
    }
    virtual const string &GetFrom() const
    {
        return from;
    }
    virtual void SetFrom(const string &from)
    {
        this->from = from;
    }
    virtual const string &GetTo() const
    {
        return to;
    }
    virtual void SetTo(const string &to)
    {
        this->to = to;
    }

    int GetTotalSeat() const
    {
        return infants + children + adults;
    }
};

class Ticket : public Reservation
{
private:
    FlightRequest request;
    string ticket_id;
    TicketType type = TicketType::economic;
    double cost = 1;
    static int ticketNumber;
    Seat seat;

public:
    Ticket()
    {
    }
    Ticket(FlightRequest request) : request(request)
    {
        ++ticketNumber;
    }
    TicketType GetTicketType() const
    {
        return type;
    }
    void SetTicketType(TicketType type)
    {
        this->type = type;

        switch (type)
        {
        case TicketType::economic:
            seat.SetSeatType(SeatType::economic);
            break;

        case TicketType::VIB:
            seat.SetSeatType(SeatType::bussiness);
            break;

        // Optional: handle invalid input (if ever extended)
        default:
            throw std::invalid_argument("Unknown TicketType");
        }
    }
    const string &GetTicketId() const
    {
        return ticket_id;
    }
    double GetTicketCost() const
    {
        return cost;
    }
    void SetTicketCost(int cost)
    {
        this->cost = cost;
    }
    void SetTicketId(const string &ticketId)
    {
        ticket_id = ticketId;
    }
    void ReadTicket(FlightRequest request)
    {
        int val;
        cout << "Information about Ticket";
        cout << "Ticket class economic (0) or VIB(1) ?";
        cin >> val;
        type = TicketType(0);
        if (type == TicketType::economic)
        {
            SetTicketCost(50);
        }
        else
            SetTicketCost(10);
        SetTicketId(request.GetFrom() + to_string(ticketNumber) + request.GetTo());
    }
    virtual Ticket *Clone() const override
    {
        return new Ticket(*this);
    }

    virtual double TotalCost() const override
    {
        return cost;
    }
    virtual string ToString() const override
    {
        ostringstream oss;
        oss << "Ticket id:" << ticket_id << ": From " << request.GetFrom() << " on " << request.GetDateTimeFrom() << "\n";

        oss << "\tTo " << request.GetTo() << " on " << request.GetDateTimeTo() << "\n";
        oss << "\t" << "Adults: " << request.GetAdults() << " children: " << request.GetChildren() << " infants: " << request.GetInfants() << "\n";
        oss << "\tTotal Cost:" << TotalCost() << "\n";
        return oss.str();
    }
};

;
int Ticket::ticketNumber = 1;
class FlightReservation : public Reservation
{
private:
    FlightRequest request;
    Flight flight;

public:
    FlightReservation(const FlightRequest &request, const Flight &flight) : request(request), flight(flight) {}

    virtual FlightReservation *Clone() const override
    {
        return new FlightReservation(*this);
    }
    virtual double TotalCost() const override
    {
        return flight.GetTotalCost();
    }
    virtual string ToString() const override
    {
        ostringstream oss;
        oss << "Airline reservation with reservation : " << flight.GetAirlineName() << ": From " << request.GetFrom() << " on " << request.GetDateTimeFrom() << "\n";
        oss << "\tTo " << request.GetTo() << " on " << request.GetDateTimeTo() << "\n";
        oss << "\t" << "Adults: " << request.GetAdults() << " children: " << request.GetChildren() << " infants: " << request.GetInfants() << "\n";
        oss << "`\tTotal Cost:" << TotalCost() << "\n";
        return oss.str();
    }
    const Flight &GetFlight() const
    {
        return flight;
    }
    const FlightRequest &GetRequest() const
    {
        return request;
    }
};

class IFlighsManager
{
protected:
    FlightRequest request;

public:
    virtual void SetFlightRequest(const FlightRequest &request_)
    {
        request = request_;
    }
    virtual vector<Flight> SearchFlights() const = 0;
    virtual bool ReserveFlight(const FlightReservation &reservation) const = 0;
    virtual string GetName() const = 0;
    virtual ~IFlighsManager() {}
};

class AirCanadaFlighsManager : public IFlighsManager
{
public:
    virtual string GetName() const override
    {
        return "AirCanada Airlines";
    }
    virtual vector<Flight> SearchFlights() const override
    {
        vector<AirCanadaFlight> flights_aircanada = AirCanadaOnlineAPI::GetFlights(request.GetFrom(), request.GetDatePartFrom(), request.GetTo(), request.GetDatePartTo(), request.GetAdults(), request.GetChildren());
        vector<Flight> flights;

        for (auto &flight_aircanada : flights_aircanada)
        {
            Flight flight;
            flight.SetAirlineName(GetName());
            flight.SetDateTimeFrom(flight_aircanada.date_time_from);
            flight.SetDateTimeTo(flight_aircanada.date_time_to);
            flight.SetTotalCost(flight_aircanada.price);

            flights.push_back(flight);
        }
        return flights;
    }

    virtual bool ReserveFlight(const FlightReservation &reservation) const
    {
        // Just dummy. we should map reservation to the agency api
        return AirCanadaOnlineAPI::ReserveFlight(AirCanadaFlight(), AirCanadaCustomerInfo());
    }
};

class TurkishFlighsManager : public IFlighsManager
{
private:
public:
    virtual string GetName() const override
    {
        return "Turkish Airlines";
    }
    virtual vector<Flight> SearchFlights() const
    {
        TurkishAirlinesOnlineAPI api;
        api.SetFromToInfo(request.GetDateTimeFrom(), request.GetFrom(), request.GetDateTimeTo(), request.GetTo());
        api.SetPassengersInfo(request.GetInfants(), request.GetChildren(), request.GetAdults());

        vector<TurkishFlight> flights_turkey = api.GetAvailableFlights();
        vector<Flight> flights;

        for (auto &flight_turkey : flights_turkey)
        {
            Flight flight;
            flight.SetAirlineName("Turkish Airlines");
            flight.SetDateTimeFrom(flight_turkey.datetime_from);
            flight.SetDateTimeTo(flight_turkey.datetime_to);
            flight.SetTotalCost(flight_turkey.cost);

            flights.push_back(flight);
        }
        return flights;
    }

    virtual bool ReserveFlight(const FlightReservation &reservation) const
    {
        return TurkishAirlinesOnlineAPI::ReserveFlight(TurkishCustomerInfo(), TurkishFlight());
    }
};

class FlightsFactory
{
public:
    static vector<IFlighsManager *> GetManagers()
    {
        vector<IFlighsManager *> flights_managers;

        flights_managers.push_back(new TurkishFlighsManager());
        flights_managers.push_back(new AirCanadaFlighsManager());

        return flights_managers;
    }
    static IFlighsManager *GetManager(string name)
    {
        for (IFlighsManager *mgr : FlightsFactory::GetManagers())
        {
            if (mgr->GetName() == name)
                return mgr;
        }
        return nullptr;
    }
};

#endif