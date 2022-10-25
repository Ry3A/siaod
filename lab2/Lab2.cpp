#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>
using namespace std;

struct Train{
    int number;
    int day;
    int month;
    char where[256];
    char from[256];
};

struct Line{
    Train mas = {};
    bool open = true;
    bool del = false;
};

struct Table{
    int m = 0;
    int n = 0;
    int d = 0;
    Line* lines{};
};

int hash1(int key, int m);
int hash2(int key, int m);
void rehash(Table& tab);
void insert(Train st, Table& tab);
int find(int key, Table tab);
void output_line(int i, Table tab);
void del(int key, Table tab);

int main()
{
    SetConsoleCP(1251);
    ///для вывода в консоль русских букв
    SetConsoleOutputCP(CP_UTF8);
    ofstream fout("Trains.bin", ios::binary|ios::app);
    Train st;
    cout << "Введите информацию о билете\n(Для прекращения ввода укажите номер поезда: -1)\n";
    for (int i = 0; true; i++)
    {
        cout << i + 1 << " билет\n";
        cout << "номер поезда: ";
        cin >> st.number;
        if (st.number == -1) break;
        cout << "день отправления: ";
        cin >> st.day;
        cout << "месяц отправления: ";
        cin >> st.month;
        cout << "пункт отправления:";
        cin.ignore();
        cin.getline(st.from, 256, '\n');
        cout << "пункт назначения:";
        cin.ignore();
        cin.getline(st.where, 256, '\n');
        fout.write((char*)&st, sizeof(Train));
        st = {};
    }
    fout.close();
    Table tab;
    tab.m = 10;
    tab.lines = new Line[10];
    ifstream fin("Trains.bin", ios::binary);
    Train st1;
    while (fin.read((char*)&st1, sizeof(Train)))
    {
        insert(st1, tab);
        if (double(tab.n + tab.d) > ((double)tab.m * 0.75))
        {
            rehash(tab);
            cout << "Выполнено рехеширование" << endl;
        }
        else{
            break;
        }
    }
    fin.close();
    int answer = 100;
    while (answer != 0){
        system("cls");
        cout << "Меню:" << endl;
        cout << "1) Вывести таблицу" << endl;
        cout << "2) Найти запись по ключу" << endl;
        cout << "3) Вставить новую запись в таблицу" << endl;
        cout << "4) Удалить запись из таблицы" << endl;
        cout << "0) Выход из программы" << endl;
        cout << "Ваш ответ: ";
        cin >> answer;
        system("cls");
        switch(answer){
            case 1:
            {
                cout << "Вывод таблицы" << endl;
                for (int i = 0; i < tab.m; i++){
                    output_line(i, tab);
                }
                system("pause");
                break;
            }
            case 2:
            {
                cout << "Поиск записи по ключу" << endl;
                int key;
                cout << "Введите номер поезда, билет на который нужно найти: ";
                cin >> key;
                int i = find(key, tab);
                if (i != -1){
                    output_line(i, tab);
                }
                else cout << "Записи с таким ключом нет" << endl;
                system("pause");
                break;
            }
            case 3:
            {
                cout << "Вставка новой записи в таблицу" << endl;
                Train st;
                cout << "Введите информацию о билете\n";
                cout << "номер поезда: ";
                cin >> st.number;
                cout << "день отправления: ";
                cin >> st.day;
                cout << "месяц отправления: ";
                cin >> st.month;
                cout << "пункт отправления:";
                cin.ignore();
                cin.getline(st.where, 256, '\n');
                cout << "пункт назначения:";
                cin.ignore();
                cin.getline(st.from, 256, '\n');
                insert(st, tab);
                if (double(tab.n + tab.d) > ((double)tab.m * 0.75)){
                    rehash(tab);
                    cout << "Выполнено рехеширование" << endl;
                }
                fout.open("Trains.bin", ios::binary | ios::app);
                fout.write((char*)&st, sizeof(Train));
                fout.close();
                st = {};
                system("pause");
                break;
            }
            case 4:
            {
                cout << "Удаление записи из таблицы" << endl;
                int key;
                cout << "Введите номер поезда, билет на который нужно удалить: ";
                cin >> key;
                del(key, tab);
                system("pause");
                break;
            }
        }
    }
}


int hash1(int key, int m){
    return key % m;
}

int hash2(int key, int m){
    return key / m;
}

void insert(Train st, Table& tab){
    int i = hash1(st.number, tab.m);
    int c = 0;
    while (!tab.lines[i].open or tab.lines[i].del){
        i = hash1(i + hash2(st.number, tab.m), tab.m);
        c++;
        if (c > tab.m){
            i++;
            c = 0;
        }
    }
    tab.lines[i].mas = st;
    tab.lines[i].open = false;
    tab.n++;
    cout << "Записан билет на поезд с номером " << st.number << " по индексу " << i << endl;
}

void rehash(Table& tab){
    tab.m *= 2;
    tab.n = 0;
    Line* lines_old = tab.lines;
    tab.lines = new Line[tab.m];
    for (int i = 0; i < tab.m / 2; i++){
        if (!lines_old[i].open and !lines_old[i].del){
            insert(lines_old[i].mas, tab);
        }
    }
}

int find(int key, Table tab){
    int i = hash1(key, tab.m);
    int c = 0;
    while (tab.lines[i].mas.number != key){
        if (!tab.lines[i].open or tab.lines[i].del){
            i = hash1(i + hash2(key, tab.m), tab.m);
            c++;
            if (c > tab.m){
                i++;
                c = 0;
            }
        }
        else return -1;
    }
    return i;
}

void output_line(int i, Table tab){
    cout << i << "Номер поезда: "<< tab.lines[i].mas.number << "   Дата отправления: " << tab.lines[i].mas.day << "." <<
    tab.lines[i].mas.month << "   Пункт отправления: " <<  tab.lines[i].mas.from << "   Пункт назначения: " <<"\t" << tab.lines[i].mas.where <<"\t" <<tab.lines[i].open <<"\t" << tab.lines[i].del << endl;
}

void del(int key, Table tab){
    int i = find(key, tab);
    if (i != -1){
        ofstream fout("new_Trains.bin", ios::binary);
        ifstream fin("Trains.bin", ios::binary);
        Train st1;
        while (fin.read((char*)&st1, sizeof(Train))){
            if (st1.number != tab.lines[i].mas.number){
                fout.write((char*)&st1, sizeof(Train));
                st1 = {};
            }
        }
        fin.close();
        fout.close();
        remove("Trains.bin");
        rename("new_Trains.bin", "Trains.bin");
        tab.lines[i].mas = {};
        tab.lines[i].open = true;
        tab.lines[i].del = true;
        tab.n--;
        tab.d++;
        cout << "Запись удалена" << endl;
    }
    else cout << "Записи с таким ключом нет" << endl;
}
