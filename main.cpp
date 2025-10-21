// volunteer_scheduler.cpp
#include <bits/stdc++.h>
using namespace std;

struct Volunteer {
    int id;
    string name;
    string phone;
};

struct Shift {
    int id;
    string date; // YYYY-MM-DD
    string start_time; // HH:MM
    string end_time;
    vector<int> volunteer_ids;
};

vector<Volunteer> volunteers;
vector<Shift> shifts;
int next_vol_id = 1, next_shift_id = 1;

void addVolunteer() {
    Volunteer v;
    v.id = next_vol_id++;
    cout << "Name: "; cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, v.name);
    cout << "Phone: "; getline(cin, v.phone);
    volunteers.push_back(v);
    cout << "Volunteer added with ID " << v.id << '\n';
}

void listVolunteers() {
    cout << "ID\tName\tPhone\n";
    for (auto &v: volunteers) cout << v.id << '\t' << v.name << '\t' << v.phone << '\n';
}

void createShift() {
    Shift s; s.id = next_shift_id++;
    cout << "Date (YYYY-MM-DD): "; cin >> s.date;
    cout << "Start time (HH:MM): "; cin >> s.start_time;
    cout << "End time (HH:MM): "; cin >> s.end_time;
    shifts.push_back(s);
    cout << "Shift created with ID " << s.id << '\n';
}

Shift* findShift(int id) {
    for (auto &s: shifts) if (s.id == id) return &s;
    return nullptr;
}

bool hasConflict(const Shift &s, int vol_id) {
    // simple check: same date and overlapping time (string compare is ok for HH:MM)
    for (auto &sh: shifts) {
        if (&sh == &s) continue;
        for (int vid: sh.volunteer_ids) {
            if (vid == vol_id && sh.date == s.date) {
                // overlap if start < other.end && other.start < end
                if (s.start_time < sh.end_time && sh.start_time < s.end_time) return true;
            }
        }
    }
    return false;
}

void assignVolunteer() {
    if (volunteers.empty() || shifts.empty()) { cout << "Need at least one volunteer and one shift.\n"; return; }
    listVolunteers();
    cout << "Enter volunteer ID: "; int vid; cin >> vid;
    cout << "Available shifts:\n";
    for (auto &s: shifts) cout << "Shift ID " << s.id << " on " << s.date << " " << s.start_time << "-" << s.end_time << '\n';
    cout << "Enter shift ID to assign: "; int sid; cin >> sid;
    Shift* s = findShift(sid);
    if (!s) { cout << "Shift not found.\n"; return; }
    if (hasConflict(*s, vid)) { cout << "Conflict detected: volunteer has overlapping assignment.\n"; return; }
    s->volunteer_ids.push_back(vid);
    cout << "Assigned volunteer " << vid << " to shift " << sid << ".\n";
}

void viewSchedule() {
    for (auto &s: shifts) {
        cout << "Shift " << s.id << ": " << s.date << " " << s.start_time << "-" << s.end_time << '\n';
        if (s.volunteer_ids.empty()) cout << "  No volunteers assigned.\n";
        else {
            cout << "  Volunteers: ";
            for (int id: s.volunteer_ids) cout << id << ' ';
            cout << '\n';
        }
    }
}

int main(){
    int ch;
    do {
        cout << "\n--- Volunteer Scheduler ---\n1) Add volunteer\n2) Create shift\n3) Assign volunteer to shift\n4) List volunteers\n5) View schedule\n6) Exit\nChoice: ";
        if (!(cin >> ch)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        switch(ch) {
            case 1: addVolunteer(); break;
            case 2: createShift(); break;
            case 3: assignVolunteer(); break;
            case 4: listVolunteers(); break;
            case 5: viewSchedule(); break;
            case 6: cout << "Bye.\n"; break;
            default: cout << "Invalid.\n";
        }
    } while (ch != 6);
    return 0;
}
