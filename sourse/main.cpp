#include <bits/stdc++.h>

#define MONTH_INVALID_ARG "Month value is invalid: "
#define DAY_INVALID_ARG "Day value is invalid: "
#define DATE_INVALID_STRING "Wrong date format: "
#define COMMAND_INVALID_STRING "Unknown command: "
#define EVENT_NOT_FOUND "Event not found"
#define EVENT_DELETE_SUCCESS "Deleted successfully"
#define EVENTS_DELETED_ZERO "Deleted 0 events"
#define DATE_SEPARATOR '-'

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
  Date() {
    year = Year();
    month = Month();
    day = Day();
  }

  Date(Year new_year, Month new_month, Day new_day) {
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
  Event() {}

  Event(const string &event) {
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

istream &operator>>(istream &is, Event &event) {
  string input;
  is >> input;
  event = Event(input);
  return is;
}

istream &operator>>(istream &is, Year &year) {
  int new_year;
  is >> new_year;
  year = Year(new_year);
  return is;
}

istream &operator>>(istream &is, Month &month) {
  int new_month;
  is >> new_month;
  month = Month(new_month);
  return is;
}

istream &operator>>(istream &is, Day &day) {
  int new_day;
  is >> new_day;
  day = Day(new_day);
  return is;
}

istream &operator>>(istream &is, Date &date) {
  int new_year, new_month, new_day;
  char sep;
  bool is_valid_date = true;
  string date_str;
  is >> date_str;

  istringstream stream(date_str);
  stream >> new_year;
  stream >> sep;
  is_valid_date &= sep == DATE_SEPARATOR;
  stream >> new_month;
  stream >> sep;
  is_valid_date &= sep == DATE_SEPARATOR;
  stream >> new_day;
  if (!stream || stream >> sep || !is_valid_date) {
    throw runtime_error(DATE_INVALID_STRING + date_str);
  }

  Year year(new_year);
  Month month(new_month);
  Day day(new_day);
  date = Date(year, month, day);
  return is;
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
  Database db;

  string line, command;
  Date date;
  Event event;
  while(getline(cin, line)) {
    if (line.empty()) {
      continue;
    }

    try {
      istringstream input(line);
      input >> command;

      if (command == "Add") {
        input >> date >> event;
        db.AddEvent(date, event);
      } else if (command == "Find") {
        input >> date;
        PrintEvents(db.FindEvents(date));
      } else if (command == "Print") {
        PrintAllEvents(db.GetAllEvents());
      } else if (command == "Del") {
        input >> date;
        if (input >> event) {
          cout << db.DeleteEvent(date, event) << '\n';
        } else {
          cout << db.DeleteEvents(date) << '\n';
        }
      } else {
        throw runtime_error(COMMAND_INVALID_STRING + command);
      }
    } catch (exception &ex) {
      cout << ex.what() << '\n';
      return 0;
    }
  }

  return 0;
}