#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <variant>
#include <any>
#include <string>
#include <strstream>

#include <vector>
#include <algorithm> // для sort

using namespace std;

struct Person {
    string fName;
    string sName;
    string oName = "";
};

bool operator < (const Person& p1, const Person& p2)
{
    return tie(p1.fName, p1.sName, p1.oName) <
        tie(p2.fName, p2.sName, p2.oName);
}

bool operator == (const Person& p1, const Person& p2)
{
    return tie(p1.fName, p1.sName, p1.oName) ==
        tie(p2.fName, p2.sName, p2.oName);
}

ostream& operator << (ostream& out, const Person& p)
{
    out.width(15);
    out << p.fName << " ";
    out.width(15);
    out << p.sName << " ";
    out.width(15);
    out << p.oName;
    return out;
}

struct PhoneNumber {
    uint kod_country;
    uint kod_city;
    string p_number;
    int add_number = -1;
};

bool operator < (const PhoneNumber& n1, const PhoneNumber& n2)
{
    return tie(n1.kod_country, n1.kod_city, n1.p_number, n1.add_number) <
        tie(n2.kod_country, n2.kod_city, n2.p_number, n2.add_number);
}

ostream& operator << (ostream& out, const PhoneNumber& n)
{
    out << "+" << n.kod_country;
    out << "(" << n.kod_city << ")";
    out << n.p_number;
    if (!(n.add_number<0)) out << " " << n.add_number;
    return out;
}

class PhoneBook {
  private:
    vector<pair<Person,PhoneNumber>> p_book;
  public:
    PhoneBook(ifstream& file) {
      file.open("/home/mike/qtc/dzS1/XXX");
      if (!file)
      {
         cerr << "\n Error open file";
         exit(1);
      }
      char sym;
      Person n;
      PhoneNumber nu;
      do {
       sym=file.peek();
       if (sym<'A' || sym>'z') break;
       file >> n.fName;
       file >> n.sName;
       n.oName="";
       do {
        sym=file.peek();
        if (sym>='A' && sym<='z') {
        file >>n.oName; break;
        }
        if (sym=='+') {
          file.ignore(1,EOF);
          break;
        }
        file.ignore(1,EOF);
       } while (file.peek()!=EOF);
       file >> nu.kod_country;
       file.ignore(1,EOF);
       file >> nu.kod_city;
       file.ignore(1,EOF);
       file >> nu.p_number;
       sym=file.peek();
       nu.add_number=-1;
       if (sym!='\n' && sym!=EOF) file >> nu.add_number;
       p_book.push_back(make_pair(n,nu));
       do {
        sym=file.get();
       }
       while (sym!='\n' && sym!=EOF);
      } while(sym!=EOF);
      file.close();
    };

    friend ostream& operator<<(ostream& out, const PhoneBook& p_book);

    void SortByName() {
        sort(p_book.begin(), p_book.end(),
          [](pair<Person,PhoneNumber> name1, pair<Person,PhoneNumber> name2)
             {
              return name1.first < name2.first;
             }
        );
    };

    void SortByPhone() {
        sort(p_book.begin(), p_book.end(),
          [](pair<Person,PhoneNumber> name1, pair<Person,PhoneNumber> name2)
             {
              return name1.second < name2.second;
             }
        );
    };

    pair<string,PhoneNumber> GetPhoneNumber(string find_surname) {
        int one=0;
        PhoneNumber n;
        for (const auto& [Person, PhoneNumber] : p_book) {
            if (Person.fName == find_surname) {
                n=PhoneNumber;++one;
            }
        }
        if (one==1) return { "", n };
        if (one>1) return { "found more than 1", n};
        return { "not found", n };
    };

    void ChangePhoneNumber(Person p,PhoneNumber n) {
        for (auto& [Person, PhoneNumber] : p_book) {
            if (Person == p) {
                PhoneNumber=n;
            }
        }
    };

};

ostream& operator<<(ostream& out, const PhoneBook& book)
{
    for (const auto& [name, phone] : book.p_book) {
        out << name << "   " << phone << endl;
    }
    return out;
}

int main() {
    ifstream file("/home/mike/qtc/XXX"); // путь к файлу PhoneBook.txt Без Open не работает на gcc
    PhoneBook book(file);
    cout << book;

    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;

    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;

    cout << "-----GetPhoneNumber-----" << endl;
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого
    //    человека, либо строку с ошибкой
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
            cout << endl;
    };

    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Petrov");
    print_phone_number("Sokolov");

    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" }, PhoneNumber{7, 123, "15344458"});
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" }, PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;
}
