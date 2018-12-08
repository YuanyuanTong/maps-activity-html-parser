#include <iostream>
#include <vector>
#include "mapsactivities.h"

#include <rapidxml-1.13/rapidxml.hpp>
#include <rapidxml-1.13/rapidxml_utils.hpp>
#include <rapidxml-1.13/rapidxml_iterators.hpp>
#include <rapidxml-1.13/rapidxml_print.hpp>

using namespace std;
using namespace rapidxml;

string trim_whitespace(const char* input_string) {
	string str = input_string;
	str.erase(str.find_last_not_of(" \f\n\r\t\v") + 1);
	str.erase(0, str.find_first_not_of(" \f\n\r\t\v"));
	return str;
}


int main() {
	file<> xmlFile("new.html");
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());

	// the out layer <div> node
	xml_node<> *pRoot = doc.first_node()->first_node("body")->first_node("div");

	// a list of all Maps activities
	MapsActivityList maps_activity_list;

	// each pNode represents a Maps usage activity
	int i = 0;
	int x = 0;
	for (xml_node<> *pNode = pRoot->first_node("div"); pNode; pNode = pNode->next_sibling())
	{
		MapsActivity new_activity;
		xml_node<> *pSubNode = pNode->first_node("div")->first_node("div");
		while (strcmp(pSubNode->first_attribute("class")->value(),
			"content-cell mdl-cell mdl-cell--6-col mdl-typography--body-1")) {
			pSubNode = pSubNode->next_sibling();
		}
		xml_node<> *pRefNode = pSubNode->first_node();

		string str = trim_whitespace(pRefNode->value());

		// discard useless type (uninformative one)
		if (str.find("Used&nbsp;Maps") != string::npos) {
			continue;
		}

		// DIRECTION type
		else if (str.find("Directions to") != string::npos) {
			new_activity.SetType(MapsActivity::DIRECTION);

			pRefNode = pRefNode->next_sibling();
			new_activity.SetName(trim_whitespace(pRefNode->value()).data());

			new_activity.SetUrl(pRefNode->first_attribute("href")->value());

			pRefNode = pRefNode->next_sibling()->next_sibling();
			string from_str = trim_whitespace(pRefNode->value());
			pRefNode = pRefNode->next_sibling()->next_sibling();
			string to_str = trim_whitespace(pRefNode->value());
			new_activity.SetFromToLoc(from_str.data(), to_str.data());
		}

		// VIEW type
		else if (str.find("Viewed area around") != string::npos) {
			new_activity.SetType(MapsActivity::VIEW);

			str.erase(0, 19);
			new_activity.SetName(str.data());

			new_activity.SetUrl(pRefNode->first_attribute("href")->value());
		}
		// USED_ON type
		else if (str.find("Used a map on") != string::npos) {
			new_activity.SetType(MapsActivity::USED_ON);

			pRefNode = pRefNode->next_sibling();
			new_activity.SetName(trim_whitespace(pRefNode->value()).data());

			new_activity.SetUrl(pRefNode->first_attribute("href")->value());
		}
		// SEARCH type
		else if (str.find("Searched for") != string::npos) {
			new_activity.SetType(MapsActivity::SEARCH);

			pRefNode = pRefNode->next_sibling();
			new_activity.SetName(trim_whitespace(pRefNode->value()).data());

			new_activity.SetUrl(pRefNode->first_attribute("href")->value());
		}
		else {
			new_activity.SetType(MapsActivity::SEARCH);
			new_activity.SetName(str.data());
			new_activity.SetUrl(pRefNode->first_attribute("href")->value());
		}

		// set the timestamp
		pRefNode = pRefNode->next_sibling()->next_sibling();
		new_activity.SetTimestamp(trim_whitespace(pRefNode->value()).data());

		while (strcmp(pSubNode->first_attribute("class")->value(),
			"content-cell mdl-cell mdl-cell--12-col mdl-typography--caption")) {
			pSubNode = pSubNode->next_sibling();
		}
		pRefNode = pSubNode->last_node("b");
		if (strcmp(pRefNode->value(), "Locations:") == 0) {
			pRefNode = pSubNode->first_node("a");
			new_activity.SetCurLoc(pRefNode->first_attribute("href")->value());
		}

		// ignore the repetitive item caused by internet delay ???
		if (maps_activity_list.Size() > 1) {
			MapsActivity last_activity = maps_activity_list.GetLatestActivity();
			if (last_activity.GetType() == new_activity.GetType()
				&& strcmp(last_activity.GetName(), new_activity.GetName()) == 0 ) {
				long long time_diff = abs(last_activity.GetTimestamp() - new_activity.GetTimestamp());
				if (time_diff < 30000000) {
					continue;
				}
			}
		}

		maps_activity_list.AddActivity(new_activity);
	}

	system("pause");
	return 0;

}