#include <ui.h>
#include <controller.h>
#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

UI::UI(Controller &ctrl): _ctrl(ctrl) {
    //ctor
}

void UI::run() {
    while(1) {
        cout << "\nChoose mode:\n";
        cout << "   1. Administrator\n";
        cout << "   2. User\n";
        cout << "   3. Exit\n";
        int op = _read_int("> ");
        system("clear");
        if(op == 1)
            _run_admin();
        else if(op == 2)
            _run_user();
        else if(op == 3)
            break ;
        else {
            cout << "Not an option!\n";
            run();
        }
    }
}

void UI::_run_admin() {
    while(1) {
        _show_admin_menu();
        int op = _read_int("> ");
        switch(op) {
        case 1:
            _admin_show_all();
            break;
        case 2:
            _admin_add_tutorial();
            break;
        case 3:
            _admin_remove_tutorial();
            break;
        case 4:
            _admin_update_tutorial();
            break;
        case 5:
            _ctrl.saveToFile("database.in");
            cout << "\nSuccessfully saved!\n";
            return ;
        default:
            cout << "Invalid operation!\n";
        }
    }
}

void UI::_run_user() {
    while(1) {
        _show_active_list();
        _show_user_menu();
        int op = _read_int("> ");
        string pres;
        vector <Tutorial> v;
        switch(op) {
        case 1:
            pres = _read_string("Presenter: ");
            this->_ctrl.filterActive(pres);
            break;
        case 2:
            /// add to watchlist
            if(this->_ctrl.getActiveList().size() == 0) {
                cout << "\nError - No tutorial selected\n";
                break;
            }
            if(!_ctrl.addToWatch(_ctrl.getActiveTutorial()))
                cout << "\nError - Already added to WatchList\n";
            else
                cout << "\nSuccessfully added\n";
            break;
        case 3:
            /// remove from watchlist
            if(this->_ctrl.getActiveList().size() == 0) {
                cout << "\nError - No tutorial selected\n";
                break;
            }
            if(!_ctrl.removeFromWatch(_ctrl.getActiveTutorial()))
                cout << "\nError - Not on the WatchList\n";
            else
                cout << "\nSuccessfully removed\n";
            cout << '\n';
            pres = _read_string("Would you like to rate one like? (yes/no)");
            if(pres == "yes")
                _ctrl.rate(_ctrl.getActiveTutorial());
            break;
        case 4:
            this->_ctrl.nextTutorial();
            break;
        case 5:
            /// print the watchlist
            this->printWatchList();
            v = _ctrl.getWatchList();
            break;
            for(int i = 0 ; i < v.size() ; ++ i)
                cout << v[i].repr() << '\n';
            cout << "\n";
            break;
        case 6:
            this->_ctrl.saveToFile("database.in");
            cout << "\nSuccessfully saved!\n";
            return ;
        default:
            cout << "Not an option!\n";
        }
    }
}

void UI::printWatchList() {
    vector <Tutorial> all = this->_ctrl.getWatchList();
    if(all.size() == 0) {
        cout << "\nThere are no tutorials here\n";
        return ;
    }
    cout << "|----------------Title----------------|-------------Presenter------------|------------------------------Link---------------------------|---Duration---|----Likes-----|\n";
    cout << setfill(' ');
    for(int i = 0 ; i < all.size() ; ++ i) {
        Tutorial it = all[i];
        cout << "|" << setw(37) << it.getTitle() << "|";
        cout << setw(34) << it.getPresenter() << "|";
        cout << setw(61) << it.getLink() << "|";
        cout << setw(14) << it.getDuration() << "|";
        cout << setw(14) << it.getLikes() << "|";
        cout << '\n';
    }
}

void UI::_show_active_list() {
    if(this->_ctrl.getActiveList().size() == 0)
        cout << "\nNo list selected\n\n";
    else {
        cout << "\n\n--------------------------------------------------------------------------ACTIVE TUTORIAL--------------------------------------------------------------------------\n";
        cout << this->_ctrl.getActiveTutorial().repr();
        cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
    }
}

void UI::_show_user_menu() {
    cout << "--- User mode ---\n";
    cout << "   1. Search for tutorials\n";
    cout << "   2. Add to Watch List\n";
    cout << "   3. Delete from Watch List\n";
    cout << "   4. Next active tutorial\n";
    cout << "   5. See the Watch List\n";
    cout << "   6. Exit\n";
}

void UI::_show_admin_menu() {
    cout << "\n";
    cout << "Administrator mode\n";
    cout << "   1. Show all tutorials\n";
    cout << "   2. Add a new tutorial\n";
    cout << "   3. Delete a tutorial\n";
    cout << "   4. Update a tutorial\n";
    cout << "   5. Exit\n";
    cout << "\n";
}

void UI::_admin_show_all() {
    vector <Tutorial> all = this->_ctrl.getAll();
    if(all.size() == 0) {
        cout << "\nThere are no tutorials here\n";
        return ;
    }
    cout << "|----------------Title----------------|-------------Presenter------------|------------------------------Link---------------------------|---Duration---|----Likes-----|\n";
    cout << setfill(' ');
    for(int i = 0 ; i < all.size() ; ++ i) {
        Tutorial it = all[i];
        cout << "|" << setw(37) << it.getTitle() << "|";
        cout << setw(34) << it.getPresenter() << "|";
        cout << setw(61) << it.getLink() << "|";
        cout << setw(14) << it.getDuration() << "|";
        cout << setw(14) << it.getLikes() << "|";
        cout << '\n';
    }
}

string UI::_read_string(string msg) {
    cout << msg;
    string text;
    getline(cin, text);
    return text;
}

int UI::_read_int(string msg, bool negative = false) {
    int value;
    bool been = false;
    do {
        if(been)
            cout << "The number must be positive!\n";
        cout << msg;
        cin >> value;
        while(cin.fail()) {
            cout << "Please input an integer!\n";
            cout << msg;
            cin.clear();
            cin.ignore(256, '\n');
            cin >> value;
        }
        cin.ignore();
        been = true;
    } while(negative && value <= 0);
    return value;
}

Tutorial UI::_read_tutorial(bool readTitle = 1, bool readPresenter = 1, bool readLink = 1, bool readDuration = 1, bool readLikes = 1) {
    string title, presenter, link;
    int duration, likes;
    if(readTitle)
        title = _read_string("Insert title: ");
    if(readPresenter)
        presenter = _read_string("Insert presenter: ");
    if(readLink)
        link = _read_string("Insert link: ");
    if(readDuration)
        duration = _read_int("Insert duration: ", true);
    if(readLikes)
        likes = _read_int("Insert likes: ");
    Tutorial t(title, presenter, link, duration, likes);
    return t;
}

void UI::_admin_add_tutorial() {
    this->_ctrl.addTutorial(this->_read_tutorial());
}

void UI::_admin_remove_tutorial() {
    this->_ctrl.removeTutorial(this->_read_tutorial(0, 0, 1, 0, 0));
}

void UI::_admin_update_tutorial() {
    this->_ctrl.updateTutorial(this->_read_tutorial());
}
