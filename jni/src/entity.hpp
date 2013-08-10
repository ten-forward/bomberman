#ifndef ENTITY_HPP
#define ENTITY_HPP

	struct Entity
	{
		int id;
		int dx;	// speed
		int dy;
		int x;	// tile position
		int y;
		int zlevel;
		int mx;	// sub-tile position
		int my;
		bool active; // is on the map?
		bool brakes; // brake on tile?
		int userdata;
	};

#endif