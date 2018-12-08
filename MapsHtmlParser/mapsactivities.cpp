#include "mapsactivities.h"

// constructor
MapsActivity::MapsActivity() {
	type = SEARCH; // default type
	timestamp = -1LL;
}

// copy constructor
MapsActivity::MapsActivity(const MapsActivity& ma) {
	type = ma.type;
	name = ma.name;
	url = ma.url;
	current_location = ma.current_location;
	from_to_locations = ma.from_to_locations;
	timestamp = ma.timestamp;
}

// sets the activity name
void MapsActivity::SetName(const char* an) {
	name = new char[strlen(an)];
	strcpy(name, an);
}

// sets the activity URL
void MapsActivity::SetUrl(const char* au) {
	url = new char[strlen(au)];
	strcpy(url, au);
}

// sets the current location
void MapsActivity::SetCurLoc(const char* ac) {
	current_location = new char[strlen(ac)];
	strcpy(current_location, ac);
}

// sets the from to locations
void MapsActivity::SetFromToLoc(const char* from, const char* to) {
	char * from_loc = new char[strlen(from)];
	strcpy(from_loc, from);
	char * to_loc = new char[strlen(to)];
	strcpy(to_loc, to);
	from_to_locations.push_back(from_loc);
	from_to_locations.push_back(to_loc);
}

// sets the type of the Maps activity
void MapsActivity::SetType(Type at) {
	type = at;
}

// sets the timestamp
void MapsActivity::SetTimestamp(const char* time) {
	timestamp = Convert2Timestamp(time);
}

// gets the activity name
char* MapsActivity::GetName() const {
	return name;
}

// gets the type
MapsActivity::Type MapsActivity::GetType() const {
	return type;
}
// gets the timestamp
long long MapsActivity::GetTimestamp() const {
	return timestamp;
}


// ************************ MapsActivityList ********************

// adds a new activity to the list
void MapsActivityList::AddActivity(MapsActivity maps_activity) {
	activities.push_back(maps_activity);
}

// gets the latest activity
MapsActivity MapsActivityList::GetLatestActivity() const {
	return activities.back();
}

// gets the size of the list
unsigned int MapsActivityList::Size() const {
	return activities.size();
}