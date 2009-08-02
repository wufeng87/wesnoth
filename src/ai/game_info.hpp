/* $Id$ */
/*
   Copyright (C) 2003 - 2009 by David White <dave@whitevine.net>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   or at your option any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

/**
 * @file ai/game_info.hpp
 * Game information for the AI
 */

#ifndef AI_GAME_INFO_HPP_INCLUDED
#define AI_GAME_INFO_HPP_INCLUDED

class gamemap;

#include "../game_display.hpp"
#include "../gamestatus.hpp"
#include "../tod_manager.hpp"
#include "../playturn.hpp"

struct paths;

/**
 * info is structure which holds references to all the important objects
 * that an AI might need access to, in order to make and implement its
 * decisions.
 */
namespace ai {

typedef int side_number;

/** The standard way in which a map of possible moves is recorded. */
typedef std::multimap<map_location,map_location> move_map;

/** The standard way in which a map of possible movement routes to location is recorded*/
typedef std::map<map_location,paths> moves_map;

class interface;

typedef boost::shared_ptr< interface > ai_ptr;

class attack_analysis;
typedef std::vector<attack_analysis> attacks_vector;

class aspect;
class candidate_action;
class engine;
class goal;
class known_aspect;
class stage;

template<typename T>
class typesafe_aspect;

template<typename T>
struct aspect_type {
        typedef boost::shared_ptr< typesafe_aspect<T> > typesafe_ptr;
};

template<typename T>
class typesafe_known_aspect;

template<typename T>
struct known_aspect_type {
        typedef boost::shared_ptr< typesafe_known_aspect<T> > typesafe_ptr;
};

typedef boost::shared_ptr< aspect > aspect_ptr;
typedef boost::shared_ptr< candidate_action > candidate_action_ptr;
typedef boost::shared_ptr< engine > engine_ptr;
typedef boost::shared_ptr< goal > goal_ptr;
typedef boost::shared_ptr< known_aspect > known_aspect_ptr;
typedef boost::shared_ptr< stage > stage_ptr;

typedef std::map<std::string, aspect_ptr > aspect_map;
typedef std::map<std::string, known_aspect_ptr > known_aspect_map;

class game_info {
public:

		game_info(game_display& disp, gamemap& map, unit_map& units,
			std::vector<team>& teams, tod_manager& tod_mng, class game_state& game_state)
			: disp(disp), map(map), units(units), teams(teams),
			   tod_manager_(tod_mng), game_state_(game_state)
		{}

		/** The display object, used to draw the moves the AI makes. */
		game_display& disp;

		/** The map of the game -- use this object to find the terrain at any location. */
		gamemap& map;

		/** The map of units. It maps locations -> units. */
		unit_map& units;

		/** A list of the teams in the game. */
		std::vector<team>& teams;

		/** Information about what turn it is, and what time of day. */
		tod_manager& tod_manager_;

		/** The global game state, because we may set the completion field. */
		class game_state& game_state_;
};

} //of namespace ai

#endif
