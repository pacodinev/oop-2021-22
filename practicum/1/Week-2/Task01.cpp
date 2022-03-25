#include <iostream>
#include <cstdlib>
#include <cassert>

struct direction
{ 
    int dy, dx;
};

constexpr direction dir[8] = {
    {-1, 0},    // UP
    {-1, +1},   // UP & RIGHT
    {0, +1},    // RIGHT
    {+1, +1},    // DOWN & RIGHT
    {+1, 0},    // DOWN
    {+1, -1},    // DOWN & LEFT
    {0, -1},    // LEFT
    {-1, -1},    // UP & LEFT
}; 

class battle_arena
{
public:
    
    // utility things

    enum class ship_type
    {
        boat,
        small_ship,
        large_ship
    };

    bool ship_type_validate (ship_type ship) const
    {
        return (int)ship_type::boat <= (int)ship && (int)ship <= (int)ship_type::large_ship;
    }

    // strongly suggested for this (and others) function to be made static
    const char* ship_type_to_str (ship_type ship) const
    {
        switch (ship) 
        {
        case ship_type::boat:
            return "boat";
        case ship_type::small_ship:
            return "small ship";
        case ship_type::large_ship:
            return "large ship";
        default:
            assert(false);
        }
    }

    int ship_type_lenght (ship_type ship) const
    {
        switch (ship) {
        case ship_type::boat:
            return 2;
        case ship_type::small_ship:
            return 3;
        case ship_type::large_ship:
            return 4;
        default:
            assert(false);
        }
    }

private:

    enum class map_cell
    {
        free = 0,
        attacked,
        boat,
        small_ship,
        large_ship
    };

    ship_type map_cell_to_ship_type (map_cell cell) const
    {
        assert(map_cell::boat <= cell && cell <= map_cell::large_ship);
        return (ship_type)((int)cell - (int)map_cell::boat + (int)ship_type::boat);
    }

    map_cell ship_type_to_map_cell (ship_type ship) const
    {
        assert(ship_type::boat <= ship && ship <= ship_type::large_ship);
        return (map_cell)((int)ship - (int)ship_type::boat + (int)map_cell::boat);
    }

    // level 0:
    map_cell map[12][12] = {{}};
    int n, m;//, count_ship[3]={0};
    int remainging_to_place_ships[3]={0};
    int count_ship_cells=0;

    // level 1:
    
    // TODO: check invariants or some invariants
    // void check_invariants();
    
    bool validate_cordinates (int y, int x) const
    {
        return 0 <= y && y < n && 0 <= x && x < m;
    }

    bool is_attacked (int y, int x) const
    {
        assert(validate_cordinates(y, x));
        
        return map[y][x] == map_cell::attacked;
    }

    bool is_free (int y, int x) const
    {
        assert(validate_cordinates(y, x));
        
        return map[y][x] == map_cell::free;
    }

    bool is_ship (int y, int x) const
    {
        assert(validate_cordinates(y, x));

        return !is_free(y, x) && !is_attacked(y, x);
    }

    ship_type get_ship (int y, int x) const
    {
        assert(validate_cordinates(y, x));

        return map_cell_to_ship_type(map[y][x]);
    }

    bool are_all_ships_placed () const
    {
        return remainging_to_place_ships[0] == 0 &&
               remainging_to_place_ships[1] == 0 &&
               remainging_to_place_ships[2] == 0;
    }

    void attack_cell (int y, int x)
    {
        assert(validate_cordinates(y, x));
        if(is_ship(y, x))
        {
            --count_ship_cells; 
        }
        map[y][x] = map_cell::attacked;
    }

    void place_ship_private (ship_type ship, int y, int x, int direction)
    {
        assert(ship_type_validate(ship));
        assert(validate_cordinates(y, x));
        assert(0<=direction && direction <= 7);
        assert(!are_all_ships_placed());

        --remainging_to_place_ships[(int)ship];

        int len = ship_type_lenght(ship);
        for(int i=0; i<len; i++)
        {
            assert(is_free(y, x));

            map[y][x] = ship_type_to_map_cell(ship);

            y += dir[direction].dy;
            x += dir[direction].dx;
        }
    }

    // level 2:
    
    bool check_ship_in_bound(ship_type ship, int y, int x, int direction) const
    {
        assert(ship_type_validate(ship));
        assert(validate_cordinates(y, x));
        assert(0<=direction && direction <= 7);

        int len = ship_type_lenght(ship);
        if(!validate_cordinates(y, x))
            return false;

        int ey = y, ex = x;
        ey += dir[direction].dy*len;
        ex += dir[direction].dx*len;
        return validate_cordinates(ey, ex);
    }

    bool check_ship_free (ship_type ship, int y, int x, int direction) const
    {
        assert(ship_type_validate(ship));
        assert(validate_cordinates(y, x));
        assert(0<=direction && direction <= 7);

        int len = ship_type_lenght(ship);
        for(int i=0; i<len; i++)
        {
            if(!is_free(y, x))
                return false;

            y += dir[direction].dy;
            x += dir[direction].dx;
        }

        return true;
    }

    
public:

    // level 1
    battle_arena(int _n, int _m, int _count_boat, int _count_small_ship, int _count_large_ship)
    {
        if(_n > 12 || _m > 12)
        {
            // TBD
            // fail state
            n = 0;
            m = 0;
            return;
        }
        n = _n;
        m = _m; 
        remainging_to_place_ships[0] /*= count_ship[0] */= _count_boat;
        remainging_to_place_ships[1] /*= count_ship[1] */= _count_small_ship;
        remainging_to_place_ships[2] /*= count_ship[2] */= _count_large_ship;
        count_ship_cells = _count_boat * ship_type_lenght(ship_type::boat) +
                           _count_small_ship * ship_type_lenght(ship_type::small_ship) +
                           _count_large_ship * ship_type_lenght(ship_type::large_ship);
    }

    int get_height() const { return n; }
    int get_width() const { return m; }

    // level 2
    void print_map() const
    {
        for(int i=0; i<get_height(); i++)
        {
            for(int j=0; j<get_width(); j++)
            {
                if(is_attacked(i, j))
                {
                    std::cout << 'X';
                }
                else if(is_free(i, j))
                {
                    std::cout << '.';
                }
                else
                {
                    ship_type ship = get_ship(i, j);
                    switch(ship)
                    {
                    case ship_type::boat:
                        std::cout << 'B'; break;
                    case ship_type::small_ship:
                        std::cout << 'S'; break;
                    case ship_type::large_ship:
                        std::cout << 'L'; break;
                    default:
                        assert(false);
                    }
                }
            }
            std::cout << '\n';
        }
    }

    // level 3:
    // direction == 0 -> up
    // direction == 1 -> up & right
    // ...
    bool place_ship(ship_type ship, int y, int x, int direction)
    {
        if(!ship_type_validate(ship))
        {
            std::clog << "ship_type is not a ship!\n";
            return false;
        }
        if(!(0<=direction && direction <= 7))
        {
            std::clog << "diretion is not a direction!\n";
            return false;
        }
        if(are_all_ships_placed())
        {
            std::clog << "All ships are already placed!\n";
            return false;
        }

        if(!check_ship_in_bound(ship, y, x, direction))
        {
            std::clog << "Ship does not fit in the playable area!\n";
            return false;
        }

        if(!check_ship_free(ship, y, x, direction))
        {
            std::clog << "There are already placed ships, preventing ship placement\n";
            return false;
        }

        // we can place it, now
        place_ship_private(ship, y, x, direction);

        //successfully added the ship
        return true;
    }

    struct attack_res
    {
        ship_type ship_hit_type;
        bool ship_hit;
        bool success;
    };

    attack_res attack(int y, int x)
    {
        if(!are_all_ships_placed())
        {
            std::clog << "Not every ship is placed!\n";
            return {ship_type::boat, false, false}; // dummy ship_type
        }
        if(!validate_cordinates(y, x))
        {
            std::clog << "Cordinates are not in the playable area!\n";
            return {ship_type::boat, false, false}; // dummy ship_type
        }
        if(is_attacked(y, x))
        {
            std::clog << "This cell is already hit!\n";
            return {ship_type::boat, false, false}; // dummy ship_type
        }

        attack_res res{};
        res.success = true;

        if(is_free(y, x))
        {
            res.ship_hit = false;
        }
        else 
        {
            res.ship_hit = true;
            res.ship_hit_type = get_ship(y, x);
        }

        attack_cell(y, x);
        
        return res;
    }

    // level 1
    bool ships_left() const
    {
        return count_ship_cells != 0;
    }

};

void read_player_ship (battle_arena &ba, battle_arena::ship_type ship_type, int count)
{
    while(count>0)
    {
        int y, x, direction;
        std::cout << "Enter " << ba.ship_type_to_str(ship_type) << " position (y, x) and direction: ";
        std::cin >> y >> x >> direction;
        if(ba.place_ship(ship_type, y, x, direction))
            --count;
    }
}

void get_player_ships (battle_arena &ba, int count_boat, int count_small_ship, int count_large_ship)
{
    read_player_ship(ba, battle_arena::ship_type::boat, count_boat);
    read_player_ship(ba, battle_arena::ship_type::small_ship, count_small_ship);
    read_player_ship(ba, battle_arena::ship_type::large_ship, count_large_ship);
}

void print_players_maps(const battle_arena &p1, const battle_arena &p2)
{
    std::cout << "Player 1 map:--------\n";
    p1.print_map();
    std::cout << "Player 2 map:--------\n";
    p2.print_map();
    std::cout << "---------------------\n";
}

int main()
{
    int n, m;
    std::cin >> n >> m;
    int count_boat, count_small_ship, count_large_ship;
    std::cin >> count_boat >> count_small_ship >> count_large_ship;

    battle_arena p1(n, m, count_boat, count_small_ship, count_large_ship);
    battle_arena p2(n, m, count_boat, count_small_ship, count_large_ship);

    std::cout << "Player 1, enter your ship positions:\n";
    get_player_ships(p1, count_boat, count_small_ship, count_large_ship);
    std::cout << "Player 2, enter your ship positions:\n";
    get_player_ships(p2, count_boat, count_small_ship, count_large_ship);

    print_players_maps(p1, p2);

    while(p1.ships_left() && p2.ships_left())
    {
        int y, x;
        battle_arena::attack_res res{};

        do {
            std::cout << "Player 1, enter attack cord (y, x): ";
            std::cin >> y >> x;
            res = p2.attack(y, x);
        } while (!res.success);
        if(res.ship_hit)
            std::cout << "Player 1 has struck down a " << p2.ship_type_to_str(res.ship_hit_type) << '\n';
        else
            std::cout << "Player 1 has not struck down anything\n";

        if(!p2.ships_left())
            break;

        do {
            std::cout << "Player 2, enter attack cord (y, x): ";
            std::cin >> y >> x;
            res = p1.attack(y, x);
        } while (!res.success);
        if(res.ship_hit)
            std::cout << "Player 2 has struck down a " << p1.ship_type_to_str(res.ship_hit_type) << '\n';
        else
            std::cout << "Player 2 has not struck down anything\n";

        print_players_maps(p1, p2);
    }

    if(!p1.ships_left())
        std::cout << "Player 2 won!\n";
    if(!p2.ships_left())
        std::cout << "Player 1 won!\n";
    
    
    return 0;
}
