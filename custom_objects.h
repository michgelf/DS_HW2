#ifndef DS_WET2_WINTER_2024_2025_CUSTOM_OBJECTS_H
#define DS_WET2_WINTER_2024_2025_CUSTOM_OBJECTS_H


struct Team;

struct Jockey {
    int id;
    Team* team;
    int record;

    Jockey(int id, Team* team) : id(id), team(team), record(0) {}
};

struct Team {
    int id;
    int num_jockeys;
    Team* merged_to;
    int record;

    Team(int id) : id(id), num_jockeys(0), merged_to(nullptr), record(0) {}

    bool is_active() {
        return merged_to == nullptr;
    }
};


#endif //DS_WET2_WINTER_2024_2025_CUSTOM_OBJECTS_H
