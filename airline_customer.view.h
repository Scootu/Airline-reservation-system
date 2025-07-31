#ifndef AIRLINE_CUSTOMER_VIEW_H_
#define AIRLINE_CUSTOMER_VIEW_H_

#include "airline_customer_mgr.h"

class CustomerView
{
private:
   CustomerManager &customer_manager;
   const Customer &customer;
   ItineraryReservation current_itinerary;

   void ReadFlightRequest(FlightRequest &request) const
   {
      string str;
      int val;

      cout << "Enter From: ";
      cin >> str;
      request.SetFrom(str);

      cout << "Enter From Date (dd-mm-yy): ";
      cin >> str;
      request.SetDateTimeFrom(str);

      cout << "Enter To: ";
      cin >> str;
      request.SetTo(str);

      cout << "Enter To Date (dd-mm-yy): ";
      cin >> str;
      request.SetDateTimeTO(str);

      cout << "Enter # of adults children (5-16) and infants: ";
      cin >> val;
      request.SetAdults(val);
      cin >> val;
      request.SetChildren(val);
      cin >> val;
      request.SetInfants(val);
   }

   int ChooseFlight(const vector<Flight> &flights) const
   {
      if (flights.size() == 0)
      {
         cout << "No trips for this request info\n";
         return -1;
      }
      for (const Flight &flight : flights)
      {
         cout << flight.ToString() << "\n";
      }
      return ReadInt(1, flights.size(), true);
   }

   void AddFlight()
   {
      FlightRequest request;
      Ticket ticket;
      ReadFlightRequest(request);

      vector<Flight> flights = customer_manager.FindFlights(request);
      ticket = customer_manager.GenerateTicketFlight(request);
      int flight_choice = ChooseFlight(flights);

      if (flight_choice == -1)
         return;
      Flight &flight = flights[flight_choice - 1];

      FlightReservation reservation(request, flight);
      current_itinerary.AddReservation(reservation);
      current_itinerary.AddReservation(ticket);
      customer_manager.SaveCustomerReservation(reservation, customer.GetName(), ticket); // Add flight reservation to the database
   }

   void PayItinerary()
   {
      vector<string> payment_choices = customer_manager.GetPaymentChoices();
      int payment_choice = ShowReadMenu(payment_choices, "Select a payment choice");
      PaymentCard *payment_card = customer.GetCards()[payment_choice - 1];

      bool status = customer_manager.MakeReservation(current_itinerary, *payment_card);

      if (status)
      {
         cout << "Itinerary reserved\n";
      }
      else
      {
         cout << "Failed to reserve the Itinerary\n";
      }
   }

public:
   CustomerView(CustomerManager &customer_manager) : customer_manager(customer_manager), customer(*customer_manager.GetCustomer())
   {
   }

   void Display()
   {
      cout << "\n\nHello " << customer.GetName() << " | User view\n";

      vector<string> menu;
      menu.push_back("View Profile");
      menu.push_back("Make flight");
      menu.push_back("List my flights");
      menu.push_back("Logout");

      while (true)
      {
         int choice = ShowReadMenu(menu);
         if (choice == 1)
            ViewProfile();
         else if (choice == 2)
            MakeFlight();
         else if (choice == 3)
            ListItineraries();
         else
            break;
      }
   }
   void ViewProfile() const
   {
      cout << "\n"
           << customer.ToString() << "\n";
   }
   void MakeFlight()
   {
      vector<string> menu;
      menu.push_back("Add flight");
      menu.push_back("Edit flight");
      menu.push_back("Delete flight");
      menu.push_back("Flight Leave And Arrive");
      menu.push_back("Done");
      menu.push_back("Cancel");

      while (true)
      {
         int choice = ShowReadMenu(menu);
         if (choice == 1)
            AddFlight();
         /*
         else if (choice == 0)
         // EditFlight();
         else if (choice == 3)
         //DeleteFlight();
         else if (choice == 4)
         //FlightLeaveArrive();
         */
         else if (choice == 2)
         {
            PayItinerary();
            current_itinerary.Clear();
            break;
         }
         else
         {
            current_itinerary.Clear();
            break;
         }
      }
   }
   void ListItineraries() const
   {
      vector<string> itineraries = customer_manager.GetItineraries();

      if (itineraries.size() == 0)
      {
         cout << "No itineraries so far!\n";
      }
      for (string &str : itineraries)
         cout << str << "\n";
   }
};

#endif