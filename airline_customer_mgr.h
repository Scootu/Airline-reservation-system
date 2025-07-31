#ifndef AIRLINES_CUSTOMER_MGR_H_
#define AIRLINES_CUSTOMER_MGR_H_

#include "airlinessystem_backend.h"
#include "airline_payment_card.h"
#include "airline_common_reservation.h"
#include "airline_customer.h"
#include "airlineFlight.h"

class CustomerManager
{
private:
    Customer *customer;
    AirlinesSystemBackend &airlines_manager;

public:
    CustomerManager(User *user, AirlinesSystemBackend &airlines_manager) : customer(dynamic_cast<Customer *>(user)), airlines_manager(airlines_manager)
    {
        if (customer == nullptr)
        {
            cout << "Error: User is null pointer as input\n";
            assert(customer != nullptr);
        }
    }
    vector<Flight> FindFlights(const FlightRequest &request) const
    {
        return airlines_manager.FindFlights(request);
    }

    vector<string> GetPaymentChoices() const
    {
        vector<string> cards_info;

        for (PaymentCard *card : customer->GetCards())
            cards_info.push_back(card->ToString());
        return cards_info;
    }

    bool MakeReservation(const Reservation &reservation, PaymentCard &payment_card)
    {
        bool is_paid = airlines_manager.ChargeCost(reservation.TotalCost(), payment_card);

        if (is_paid)
        {
            cout << "Money withdraw successfully\n";

            bool is_confirmed = airlines_manager.ConfirmRerservation(reservation);

            if (is_confirmed)
            {
                cout << "Reservation confirmed\n";
                customer->AddReservation(reservation);
                // update reservation
                return true;
            }
            else
            {
                cout << "Failed to confirm some of your reservation items\n";
                cout << "Cancelling payment and whatever reserved\n";
                bool is_uncharged = airlines_manager.UnChargeCost(reservation.TotalCost(), payment_card);

                if (!is_uncharged)
                    cout << "Problems is returning back your money. Call us on 911\n";
            }
        }
        else
            cout << "Failed to withdraw Money for the reservation\n";
        return false;
    }
    Ticket GenerateTicketFlight(FlightRequest request)
    {
        Ticket ticket(request);
        ticket.ReadTicket(request);
        return ticket;
    }
    void SaveCustomerReservation(const Reservation &reservation, const string &username, const Ticket &ticket)
    {
        airlines_manager.SaveReservation(reservation, username, ticket);
    }
    vector<string> GetItineraries() const
    {
        const ReservationsSet &set = customer->GetReservations();
        vector<string> itineraries;
        for (const Reservation *reservation : set.GetReservations())
            itineraries.push_back(reservation->ToString());

        return itineraries;
    }

    const Customer *GetCustomer() const
    {
        return customer;
    }
};
#endif