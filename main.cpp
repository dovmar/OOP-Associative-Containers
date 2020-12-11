#include "Header.h";

void rastiURL(set<string>& urls, string eilute) {
    // Randa nuorodas pateiktoje eiluteje
    string zodis;
    istringstream iss(eilute);
    while (iss >> zodis) {
        if (std::regex_match(zodis, std::regex("[(http(s)?):\\/\\/(www\\.)?a-zA-Z0-9@:%._\\+~#=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)")))
        {
            urls.insert(zodis);
        }
    }
}


string supaprastintiEilute(set<string> urls,string eilute) {
    // Is eilutes istrina skaicius ir kitus ne raidinius simbolius
    for (auto url : urls) {
        eilute = std::regex_replace(eilute, std::regex(url), "");
    }
    int ilgis = eilute.size();
    for (int i = 0; i <= ilgis; i++) {
        if (ispunct(eilute[i]) || isdigit(eilute[i])) {
            eilute.erase(i,1);
            ilgis = eilute.size();
        }
        else {
            eilute[i] = std::tolower(eilute[i]);
        }
    }
    return eilute;
}



bool zodisZodyne(map<string, vector<int>>& zodynas, string& zodis) {
    // Patikrina ar zodyne jau yra tam tikras zodis
    auto iter = zodynas.begin();
    while (iter != zodynas.end()) {
        if (iter->first == zodis) {
            return true;
        }
        iter++;
    }
    return false;
}


void papildytiZodyna(map<string, vector<int>>& zodynas, string& zodis, int index) {
    // Atnaujina informacija apie zodzius kurie jau yra zodyne
    auto iter = zodynas.find(zodis);
    iter->second.push_back(index);

}


void nuskaitytiTeksta(set<string>& urls, map<string, vector<int>>& zodynas, string failoPav) {
    // Nuskaito teksta is nurodyto failo
    string tekstas;
    string eilute;
    string zodis;
    ifstream skait;
    skait.open(failoPav);
    for (int i = 0; !skait.eof(); ++i) {
        getline(skait, eilute);
        rastiURL(urls,eilute);
        eilute = supaprastintiEilute(urls,eilute);
        istringstream iss(eilute);
        while (iss >> zodis) {
            if (!zodisZodyne(zodynas,zodis)) {
                zodynas.insert(std::make_pair(zodis, vector<int>({i+1})));
            }
            else {
                papildytiZodyna(zodynas, zodis, i+1);
            }
        }
    }
    skait.close();
}


void isvestiPasikartojancius(map<string, vector<int>>& zodynas, string failoPav) {
    // Isveda pasikartojancius zodzius, kiek kartu pasikartojo ir kokiose eilutese
    ofstream isved;
    auto iter = zodynas.begin();
    isved.open(failoPav);
    while (iter != zodynas.end()) {
        isved << setw(20) << left << iter->first << " ";
        isved << setw(5) << left << iter->second.size() << " ";
        for (auto eilnr : iter->second) {
            isved << eilnr << " ";
        }
        isved << "\n";
        iter++;
    }
    isved.close();
}


void isvestiURL(set<string>& urls) {
    // Isveda tekste rastas nuorodas
    for (string url : urls) {
        cout << url << "\n";
    }
}

void isvestiURL(set<string>& urls, string failoPav) {
    // Isveda tekste rastas nuorodas i faila
    ofstream isved;
    isved.open(failoPav);
    for (string url : urls) {
        isved << url << "\n";
    }
}

int main()
{
    int n;
    string failoPav;
    set<string> urls;
    map<string, vector<int>> zodynas;
    cout << "Iveskite norimo nuskaityti failo pavadinima (iskaitant.txt)" << "\n";
    cin >> failoPav;
    nuskaitytiTeksta(urls, zodynas, failoPav);
    while (true) {
        cout << "Kaip isvesti faile rastas nuorodas: " << "\n";
        cout << "1. Isvesti i atskira faila" << "\n";
        cout << "2. Isvesti i ekrana" << "\n";
        cin >> n;
        if (n != 1 && n != 2) {
            cout << "Tokio pasirinkimo nera" << "\n";
        }
        if (n == 1) {
            isvestiURL(urls, "nuorodos.txt");
            break;
        }
        if (n == 2) {
            isvestiURL(urls);
            break;
        }
    }
}