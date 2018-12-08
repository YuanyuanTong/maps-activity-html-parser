# maps-activity-html-parser
A parser to deal with the Google Maps MyActivity.html from the Google takeout website. It is based on RapidXML parser.

It can record the following information linked with each Maps usage activity.
* Time.
* URL linked with the activity.
* Activity type
  * SEARCH: searching for some place
  * USED_ON: Maps was used on a thid-party website
  * DIRECTION: looking for a route between two places
  * VIEW: viewing aread around some place
* Location name(s) if available
