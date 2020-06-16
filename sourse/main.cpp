#include <bits/stdc++.h>

#define MONTH_INVALID_ARG "Month value is invalid: "
#define DAY_INVALID_ARG "Day value is invalid: "
#define EVENT_NOT_FOUND "Event not found"
#define EVENT_DELETE_SUCCESS "Deleted successfully"
#define EVENTS_DELETED_ZERO "Deleted 0 events"

using namespace std;

// Year
class Year {
public:
  explicit Year() {
    year = 1;
  }

  explicit Year(int new_year) {
    year = new_year;
  }

  int getYear() const {
    return year;
  }

private:
  int year;
};

// Month
class Month {
public:
  explicit Month() {
    month = 1;
  }

  explicit Month(int new_month) {
    if (!isValid(new_month)) {
      throw invalid_argument(MONTH_INVALID_ARG + to_string(new_month));
    }
    month = new_month;
  }

  int getMonth() const {
    return month;
  }

private:
  int month;

  bool isValid(int value) const {
    return !(value < 1 || value > 12);
  }
};

// Day
class Day {
public:
  explicit Day() {
    day = 1;
  }

  explicit Day(int new_day) {
    if (!isValid(new_day)) {
      throw invalid_argument(DAY_INVALID_ARG + to_string(new_day));
    }
    day = new_day;
  }

  int getDay() const {
    return day;
  }

private:
  int day;

  bool isValid(int value) const {
    return !(value < 1 || value > 31);
  }
};

// Date
class Date {
public:
  explicit Date(Year new_year, Month new_month, Day new_day) {
    year = new_year;
    month = new_month;
    day = new_day;
  }

  string getDate() const {
    stringstream date;
    date << setw(4) << setfill('0') << to_string(year.getYear());
    date << '-';
    date << setw(2) << setfill('0') << to_string(month.getMonth());
    date << '-';
    date << setw(2) << setfill('0') << to_string(day.getDay());
    return date.str();
  }

  bool operator<(const Date &r_date) const {
    return this->getDate() < r_date.getDate();
  }

private:
  Year year;
  Month month;
  Day day;
};

// Event
class Event {
public:
  explicit Event(const string &event) {
    this->event = event;
  }

  string getEvent() const {
    return event;
  }

  bool operator<(const Event &r_event) const {
    return this->getEvent() < r_event.getEvent();
  }

private:
  string event;
};

// Database
class Database {
public:
  void AddEvent(const Date &date, const Event &event) {
    notebook[date].insert(event);
  }

  string DeleteEvent(const Date &date, const Event &event) {
    string response;
    if (notebook.count(date) == 0) {
      response = EVENT_NOT_FOUND;
    } else {
      auto &events = notebook[date];
      if (events.count(event) == 0) {
        response = EVENT_NOT_FOUND;
      } else {
        events.erase(event);
        response = EVENT_DELETE_SUCCESS;
      }
    }
    return response;
  }

  string DeleteEvents(const Date &date) {
    string response;
    int N = 0;
    if (notebook.count(date) == 0) {
      response = EVENTS_DELETED_ZERO;
    } else {
      auto &events = notebook[date];
      N = events.size();
      events.clear();
      notebook.erase(date);
      response = "Deleted " + to_string(N) + " events";
    }
    return response;
  }

  set<Event> FindEvents(const Date &date) const {
    if (notebook.count(date) == 0) {
      return {};
    }
    return notebook.at(date);
  }

  map<Date, set<Event>> GetAllEvents() const {
    return notebook;
  }

private:
  map<Date, set<Event>> notebook;
};

ostream &operator<<(ostream &os, const Event &event) {
  os << event.getEvent();
  return os;
}

ostream &operator<<(ostream &os, const Date &date) {
  os << date.getDate();
  return os;
}

void PrintEvents(const set<Event> &events) {
  for (const auto &event : events) {
    cout << event << '\n';
  }
}

void PrintAllEvents(const map<Date, set<Event>> &notebook) {
  for (const auto &item : notebook) {
    for (const auto &event : item.second) {
      cout << item.first << ' ' << event << '\n';
    }
  }
}

int main() {
  Database database;

  Date date1(Year(134), Month(1), Day(8));
  Date date2(Year(789), Month(12), Day(23));
  Date date3(Year(2000), Month(6), Day(24));

  cout << "@@@ Appending events @@@\n";

  database.AddEvent(date1, Event("date1-event1"));
  database.AddEvent(date1, Event("date1-event2"));
  database.AddEvent(date2, Event("date2-event1"));
  database.AddEvent(date3, Event("date3-event1"));
  database.AddEvent(date3, Event("date3-event2"));
  database.AddEvent(date3, Event("date3-event3"));

  cout << "@@@ Deleting event \"date3-event2\" @@@\n";

  database.DeleteEvent(date3, Event("date3-event2"));

  cout << "@@@ Printing events on date3 @@@\n";

  PrintEvents(database.FindEvents(date3));

  cout << "@@@ Printing all events @@@\n";

  PrintAllEvents(database.GetAllEvents());

  cout << "@@@ Deleting events on date1 @@@\n";

  database.DeleteEvents(date1);

  cout << "@@@ Printing all events @@@\n";

  PrintAllEvents(database.GetAllEvents());

  return 0;
}
