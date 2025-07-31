
#ifndef AIRLINE_CUSTOMER_H_
#define AIRLINE_CUSTOMER_H_
#include "airlines_user.h"
#include "airline_common_reservation.h"
#include "airline_payment_card.h"


class Customer : public User {
    protected:
    vector<PaymentCard*> cards;
    ReservationsSet reservations;

    public:
    Customer(const Customer&) = delete;
    void operator=(const Customer&) = delete;
    
    Customer(){
        SetUserType(UserType::CustumerUser);
    }
    ~Customer(){
        for(PaymentCard* card: cards){
            delete card;
        }
        cards.clear();
    }

    void AddPaymentCard(const PaymentCard& card){
        cards.push_back(card.Clone());
    }

    void AddReservation(const Reservation& reservation){
        reservations.AddReservation(*reservation.Clone());
    }
    const vector<PaymentCard*>& GetCards() const {
        return cards;
    }
    const ReservationsSet& GetReservations() const {
        return reservations;
    }
    
};

#endif