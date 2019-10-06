
#ifndef SCRIPT_ENGINE_MAP_HPP
#define SCRIPT_ENGINE_MAP_HPP

#include "all.hpp"
#include "script_engine/script_engine.hpp"
#include "map/map.hpp"

void map_register(lua_State * state);

void push_state_in_stack(lua_State * state, const s_map_state mstate, const int offset);

#endif

