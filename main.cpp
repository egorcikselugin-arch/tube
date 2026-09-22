#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

// ---------- Структуры ----------
struct Pipe {
    string id;
    double length = 0;
    int diameter = 0;
    bool in_repair = false;
    bool created = false;
};

struct Station {
    string name;
    int total = 0;
    int active = 0;
    int cls = 0;
    bool created = false;
};


template <typename T>
T input(const string& prompt, T lo, T hi) 
{
    T v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= lo && v <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        cout << "Ошибка! Введите число от " << lo << " до " << hi << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string inputLine(const string& prompt) 
{
    string s;
    cout << prompt;
    getline(cin, s);
    while (s.empty()) {
        cout << "Не может быть пустым: ";
        getline(cin, s);
    }
    return s;
}

void addPipe(Pipe& p) 
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    p.id = inputLine("Название трубы: ");
    p.length = input("Длина (км): ", 0.1, 10000.0);
    p.diameter = input("Диаметр (мм): ", 10, 2500);
    p.in_repair = input("В ремонте? (1-да, 0-нет): ", 0, 1) == 1;
    p.created = true;
    cout << "Труба добавлена!\n";
}

void showPipe(const Pipe& p) 
{
    if (!p.created) { cout << "Труба не создана.\n"; return; }
    cout << "Труба: " << p.id << ", " << p.length << " км, "
         << p.diameter << " мм, "
         << (p.in_repair ? "в ремонте" : "в работе") << "\n";
}

void editPipe(Pipe& p) 
{
    if (!p.created) { cout << "Сначала добавь трубу!\n"; return; }
    p.in_repair = input("В ремонте? (1-да, 0-нет): ", 0, 1) == 1;
    cout << "Статус обновлён!\n";
}

// ---------- КС ----------
void addStation(Station& s) 
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    s.name = inputLine("Название КС: ");
    s.total = input("Всего цехов: ", 1, 100);
    s.active = input("В работе: ", 0, s.total);
    s.cls = input("Класс станции: ", 1, 5);
    s.created = true;
    cout << "КС добавлена!\n";
}

void showStation(const Station& s) 
{
    if (!s.created) { cout << "КС не создана.\n"; return; }
    cout << "КС: " << s.name << ", цехов " << s.active << "/" << s.total
         << ", класс " << s.cls << "\n";
}

void editStation(Station& s) 
{
    if (!s.created) { cout << "Сначала добавь КС!\n"; return; }
    cout << "Сейчас в работе " << s.active << " из " << s.total << "\n";
    cout << "1-запустить, 2-остановить, 0-отмена\n";
    int c = input("Выбор: ", 0, 2);
    if (c == 1 && s.active < s.total) { s.active++; cout << "Запущен!\n"; }
    else if (c == 2 && s.active > 0) { s.active--; cout << "Остановлен!\n"; }
    else if (c != 0) cout << "Нельзя!\n";
}

void save(const Pipe& p, const Station& s) 
{
    string f = inputLine("Имя файла: ");
    ofstream out(f);
    if (!out) { cout << "Не открыть файл!\n"; return; }

    out << p.created << "\n";
    if (p.created)
        out << p.id << "\n" << p.length << "\n" << p.diameter << "\n" << p.in_repair << "\n";

    out << s.created << "\n";
    if (s.created)
        out << s.name << "\n" << s.total << "\n" << s.active << "\n" << s.cls << "\n";

    cout << "Сохранено в " << f << "\n";
}

void load(Pipe& p, Station& s) 
{
    string f = inputLine("Имя файла: ");
    ifstream in(f);
    if (!in) { cout << "Файл не найден!\n"; return; }

    in >> p.created;
    if (p.created) {
        in.ignore();
        getline(in, p.id);
        in >> p.length >> p.diameter >> p.in_repair;
    }

    in >> s.created;
    if (s.created) {
        in.ignore();
        getline(in, s.name);
        in >> s.total >> s.active >> s.cls;
    }

    cout << "Загружено из " << f << "\n";
}

int main() 
{
    setlocale(LC_ALL, "Russian");
    Pipe p;
    Station s;

    while (true) 
    {
        cout << "\n===== МЕНЮ =====\n"
             << "1. Добавить трубу\n"
             << "2. Добавить КС\n"
             << "3. Показать всё\n"
             << "4. Изменить трубу\n"
             << "5. Изменить КС\n"
             << "6. Сохранить в файл\n"
             << "7. Загрузить из файла\n"
             << "0. Выход\n";

        int c = input("Выбор: ", 0, 7);

        if (c == 1) addPipe(p);
        else if (c == 2) addStation(s);
        else if (c == 3) { showPipe(p); showStation(s); }
        else if (c == 4) editPipe(p);
        else if (c == 5) editStation(s);
        else if (c == 6) save(p, s);
        else if (c == 7) load(p, s);
        else { cout << "Пока!\n"; break; }
    }
    return 0;
}