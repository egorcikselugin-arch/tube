#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

struct Pipe
{
    string id;
    double length = 0;
    int diameter = 0;
    bool in_repair = false;
};

struct Station
{
    string name;
    int total = 0;
    int active = 0;
    int cls = 0;
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
    p.id = inputLine("Название трубы: ");
    p.length = input("Длина (км): ", 0.1, 10000.0);
    p.diameter = input("Диаметр (мм): ", 10, 2500);
    p.in_repair = input("В ремонте? (1-да, 0-нет): ", 0, 1) == 1;
    cout << "Труба добавлена!\n";
}

void showPipe(const Pipe& p)
{
    if (p.id.empty()) { cout << "Труба не создана.\n"; return; }
    cout << "Труба: " << p.id << ", " << p.length << " км, "
         << p.diameter << " мм, "
         << (p.in_repair ? "в ремонте" : "в работе") << "\n";
}

void editPipe(Pipe& p)
{
    if (p.id.empty()) { cout << "Сначала добавь трубу!\n"; return; }
    p.in_repair = input("В ремонте? (1-да, 0-нет): ", 0, 1) == 1;
    cout << "Статус обновлён!\n";
}

void addStation(Station& s)
{
    s.name = inputLine("Название КС: ");
    s.total = input("Всего цехов: ", 1, 100);
    s.active = input("В работе: ", 0, s.total);
    s.cls = input("Класс станции: ", 1, 5);
    cout << "КС добавлена!\n";
}

void showStation(const Station& s)
{
    if (s.name.empty()) { cout << "КС не создана.\n"; return; }
    cout << "КС: " << s.name << ", цехов " << s.active << "/" << s.total
         << ", класс " << s.cls << "\n";
}

void editStation(Station& s)
{
    if (s.name.empty()) { cout << "Сначала добавь КС!\n"; return; }
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

    out << (p.id.empty() ? 0 : 1) << "\n";
    if (!p.id.empty()) {
        out << p.id << "\n"
            << p.length << "\n"
            << p.diameter << "\n"
            << p.in_repair << "\n";
    }

    out << (s.name.empty() ? 0 : 1) << "\n";
    if (!s.name.empty()) {
        out << s.name << "\n"
            << s.total << "\n"
            << s.active << "\n"
            << s.cls << "\n";
    }

    if (!out) { cerr << "Ошибка записи в файл\n"; return; }
    cout << "Сохранено в " << f << "\n";
}

void load(Pipe& p, Station& s)
{
    string f = inputLine("Имя файла: ");
    ifstream in(f);
    if (!in) { cout << "Файл не найден!\n"; return; }

    int hasPipe = 0;
    if (!(in >> hasPipe)) { cerr << "Ошибка чтения\n"; return; }
    if (hasPipe) {
        in.ignore();
        getline(in, p.id);
        in >> p.length >> p.diameter >> p.in_repair;
        if (!in) { cerr << "Ошибка чтения трубы\n"; return; }
    } else {
        p = Pipe{};
    }

    int hasStation = 0;
    if (!(in >> hasStation)) { cerr << "Ошибка чтения\n"; return; }
    if (hasStation) {
        in.ignore();
        getline(in, s.name);
        in >> s.total >> s.active >> s.cls;
        if (!in) { cerr << "Ошибка чтения КС\n"; return; }
    } else {
        s = Station{};
    }

    cout << "Загружено из " << f << "\n";
}

void fillAuto(Pipe& p, Station& s)
{
    p.id = "Северная";
    p.length = 100.0;
    p.diameter = 500;
    p.in_repair = false;

    s.name = "КС-1";
    s.total = 5;
    s.active = 3;
    s.cls = 2;
}

void showMenu()
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
}

void choice(Pipe& p, Station& s)
{
    while (true) {
        showMenu();
        int c = input("Выбор: ", 0, 7);

        switch (c) {
            case 1: addPipe(p); break;
            case 2: addStation(s); break;
            case 3: showPipe(p); showStation(s); break;
            case 4: editPipe(p); break;
            case 5: editStation(s); break;
            case 6: save(p, s); break;
            case 7: load(p, s); break;
            case 0:
                cout << "Пока!\n";
                return;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    Pipe p;
    Station s;

    fillAuto(p, s);

    choice(p, s);

    return 0;
}